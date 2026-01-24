studio.menu.addMenuItem({
    name: "DRD\\Sync to Obsidian...",
    execute: function () {
        var t0 = Date.now();

        // -------------------------
        // Helpers (reused from CSV Exporter)
        // -------------------------
        function safeGet(obj, keyPath) {
            try {
                var parts = keyPath.split(".");
                var cur = obj;
                for (var i = 0; i < parts.length; i++) {
                    if (!cur) return "";
                    cur = cur[parts[i]];
                }
                return (cur === undefined || cur === null) ? "" : cur;
            } catch (e) {
                return "";
            }
        }

        function tryCall(fn, fallback) {
            try { return fn(); } catch (e) { return fallback; }
        }

        function isLikelySpatialiserEffect(effectObj) {
            var t = "";
            try { t = effectObj && effectObj.type ? ("" + effectObj.type) : ""; } catch (e) { }
            if (!t) {
                try { t = effectObj && effectObj.isOfExactType ? "" : ""; } catch (e) { }
            }

            try {
                if (effectObj && effectObj.isOfExactType) {
                    if (effectObj.isOfExactType("SpatialiserEffect")) return true;
                    if (effectObj.isOfExactType("ObjectSpatialiserEffect")) return true;
                }
            } catch (e) { }

            var name = "";
            try { name = effectObj && effectObj.name ? ("" + effectObj.name) : ""; } catch (e) { }
            var typeName = "";
            try { typeName = effectObj && effectObj.type ? ("" + effectObj.type) : ""; } catch (e) { }

            var blob = (name + " " + typeName).toLowerCase();
            return (blob.indexOf("spatial") >= 0);
        }

        function detectSpace(e) {
            try {
                if (e.masterTrack && e.masterTrack.mixerGroup && e.masterTrack.mixerGroup.relationships.effectChain) {
                    var chain = e.masterTrack.mixerGroup.relationships.effectChain.destinations[0];
                    if (chain && chain.relationships.effects) {
                        var effects = chain.relationships.effects.destinations;
                        for (var i = 0; i < effects.length; i++) {
                            if (isLikelySpatialiserEffect(effects[i])) return "3D";
                        }
                    }
                }

                if (e.relationships && e.relationships.returnTracks && e.relationships.returnTracks.destinations) {
                    var rts = e.relationships.returnTracks.destinations;
                    for (var r = 0; r < rts.length; r++) {
                        var rt = rts[r];
                        if (rt && rt.mixerGroup && rt.mixerGroup.relationships.effectChain) {
                            var rchain = rt.mixerGroup.relationships.effectChain.destinations[0];
                            if (rchain && rchain.relationships.effects) {
                                var reffects = rchain.relationships.effects.destinations;
                                for (var j = 0; j < reffects.length; j++) {
                                    if (isLikelySpatialiserEffect(reffects[j])) return "3D";
                                }
                            }
                        }
                    }
                }

                return "2D";
            } catch (err) {
                return "Unknown";
            }
        }

        function detectLoopType(e) {
            try {
                if (e.timeline && e.timeline.relationships.markers) {
                    var markers = e.timeline.relationships.markers.destinations;
                    for (var i = 0; i < markers.length; i++) {
                        if (markers[i].isOfExactType && markers[i].isOfExactType("LoopRegion")) {
                            return "Loop";
                        }
                    }
                }
            } catch (e) { }
            return "One-shot";
        }

        function getFolderPathFromEventPath(eventPath) {
            var cleanPath = (eventPath || "").replace("event:/", "");
            var parts = cleanPath.split("/");
            if (parts.length <= 1) return "";
            parts.pop();
            return parts.join("/");
        }

        function getBankNames(e) {
            try {
                if (e.relationships.banks && e.relationships.banks.destinations && e.relationships.banks.destinations.length > 0) {
                    var names = e.relationships.banks.destinations.map(function (b) { return b.name; }).filter(Boolean);
                    names.sort();
                    return names;
                }
            } catch (err) { }
            return [];
        }

        function getUserProperties(e) {
            var props = [];
            try {
                if (!e.userProperties || e.userProperties.length === 0) return props;
                for (var i = 0; i < e.userProperties.length; i++) {
                    var p = e.userProperties[i];

                    var key = tryCall(function () { return p.name; }, "");
                    var typ = tryCall(function () { return p.type; }, "");
                    var val = "";

                    val = tryCall(function () { return p.value; }, "");
                    if (val === "" || val === undefined || val === null) {
                        val = tryCall(function () { return p.stringValue; }, "");
                    }
                    if (val === "" || val === undefined || val === null) {
                        val = tryCall(function () { return p.intValue; }, "");
                    }
                    if (val === "" || val === undefined || val === null) {
                        val = tryCall(function () { return p.floatValue; }, "");
                    }
                    if (val === "" || val === undefined || val === null) {
                        val = tryCall(function () { return p.boolValue; }, "");
                    }

                    props.push({ name: key, type: typ, value: val });
                }
                props.sort(function(a, b) { return a.name < b.name ? -1 : 1; });
            } catch (err) { }
            return props;
        }

        function getNotesString(e) {
            var candidates = ["notes", "note", "comment", "comments", "description"];
            for (var i = 0; i < candidates.length; i++) {
                var v = safeGet(e, candidates[i]);
                if (v !== "" && v !== null && v !== undefined) return v;
            }
            return "";
        }

        function getParameterDetails(p) {
            var name = tryCall(function () { return (p.presetOwner) ? p.presetOwner.name : p.name; }, "");
            if (!name) name = tryCall(function () { return p.name; }, "");

            var preset = tryCall(function () { return p.preset || (p.presetOwner ? p.presetOwner.preset : null) || p; }, p);

            var typ = tryCall(function () { return preset.type; }, "");
            var min = tryCall(function () { return preset.min; }, "");
            var max = tryCall(function () { return preset.max; }, "");
            var init = tryCall(function () { return preset.initialValue; }, "");
            var labels = tryCall(function () { return preset.enumerationLabels; }, "");

            var labelStr = "";
            if (labels && labels.length && typeof labels.join === "function") {
                labelStr = labels.join(", ");
            }

            return {
                name: name,
                type: typ,
                min: min,
                max: max,
                initial: init,
                labels: labelStr
            };
        }

        function collectEventParameters(e) {
            var byName = {};

            function addParam(p) {
                var details = getParameterDetails(p);
                if (!details.name || details.name === "undefined") return;
                byName[details.name] = details;
            }

            try {
                if (typeof e.getParameterPresets === "function") {
                    e.getParameterPresets().forEach(addParam);
                }
            } catch (err) { }

            var rels = ["parameters", "userParameters"];
            rels.forEach(function (relName) {
                try {
                    if (e.relationships && e.relationships[relName] && e.relationships[relName].destinations) {
                        e.relationships[relName].destinations.forEach(function (proxy) {
                            var pObj = null;
                            try { if (proxy.relationships && proxy.relationships.parameter) pObj = proxy.relationships.parameter.destinations[0]; } catch (e) { }
                            try { if (!pObj && proxy.relationships && proxy.relationships.preset) pObj = proxy.relationships.preset.destinations[0]; } catch (e) { }
                            if (pObj) addParam(pObj);
                        });
                    }
                } catch (err) { }
            });

            var names = Object.keys(byName).sort();
            return names.map(function (n) { return byName[n]; });
        }

        // -------------------------
        // New helpers for Obsidian sync
        // -------------------------
        function createDirectory(path) {
            // Use shell command to create directory (works on macOS)
            try {
                // Check if already exists
                var dirFile = studio.system.getFile(path);
                if (dirFile.exists()) {
                    return true;
                }

                studio.system.start("/bin/mkdir", ["-p", path]);

                // Wait for directory to exist (time-based polling)
                var startTime = Date.now();
                var maxWaitMs = 2000; // 2 second timeout
                while ((Date.now() - startTime) < maxWaitMs) {
                    dirFile = studio.system.getFile(path);
                    if (dirFile.exists()) {
                        return true;
                    }
                    // Busy wait a tiny bit to not spin too fast
                    var waitUntil = Date.now() + 10;
                    while (Date.now() < waitUntil) { }
                }
                return studio.system.getFile(path).exists();
            } catch (e) {
                console.log("Failed to create directory: " + path + " :: " + e);
                return false;
            }
        }

        function ensureDirectoryForFile(filePath, createdDirsTracker) {
            // Extract directory from file path and ensure it exists
            var lastSlash = filePath.lastIndexOf("/");
            if (lastSlash <= 0) return true;
            var dirPath = filePath.substring(0, lastSlash);
            var dirFile = studio.system.getFile(dirPath);
            if (dirFile.exists()) return true;
            var result = createDirectory(dirPath);
            if (result && createdDirsTracker) {
                createdDirsTracker[dirPath] = true;
            }
            return result;
        }

        function writeMarkdownFile(filePath, content, createdDirsTracker) {
            // Ensure directory exists and write file
            ensureDirectoryForFile(filePath, createdDirsTracker);
            var file = studio.system.getFile(filePath);
            if (file.open(studio.system.openMode.WriteOnly)) {
                file.writeText(content);
                file.close();
                return true;
            }
            return false;
        }

        function sanitizeFilename(name) {
            var result = name;
            var badChars = ["<", ">", ":", "/", "|", "?", "*", '"'];
            for (var c = 0; c < badChars.length; c++) {
                result = result.split(badChars[c]).join("-");
            }
            result = result.split("\\").join("-");
            result = result.replace(/\s+/g, "_");
            result = result.replace(/-+/g, "-");
            result = result.replace(/^-|-$/g, "");
            return result;
        }

        function pad2(n) { return (n < 10 ? "0" : "") + n; }

        function getISOTimestamp() {
            var now = new Date();
            return now.getFullYear() + "-" + pad2(now.getMonth() + 1) + "-" + pad2(now.getDate()) +
                   "T" + pad2(now.getHours()) + ":" + pad2(now.getMinutes()) + ":" + pad2(now.getSeconds());
        }

        function yamlEscape(val) {
            if (val === null || val === undefined) return '""';
            var s = "" + val;
            // Quote if contains special YAML characters
            if (s.indexOf(":") >= 0 || s.indexOf("#") >= 0 || s.indexOf("'") >= 0 ||
                s.indexOf('"') >= 0 || s.indexOf("{") >= 0 || s.indexOf("}") >= 0 ||
                s.indexOf("[") >= 0 || s.indexOf("]") >= 0 || s.indexOf("&") >= 0 ||
                s.indexOf("*") >= 0 || s.indexOf("!") >= 0 || s.indexOf("|") >= 0 ||
                s.indexOf(">") >= 0 || s.indexOf("%") >= 0 || s.indexOf("@") >= 0 ||
                s.indexOf("`") >= 0 || s.trim() !== s || s === "") {
                return '"' + s.replace(/\\/g, "\\\\").replace(/"/g, '\\"') + '"';
            }
            return s;
        }

        function parseYamlFrontmatter(content) {
            // Simple YAML frontmatter parser
            var result = { properties: {}, bodyStart: 0 };

            if (!content || content.indexOf("---") !== 0) {
                result.bodyStart = 0;
                return result;
            }

            var endMatch = content.indexOf("\n---", 3);
            if (endMatch < 0) {
                result.bodyStart = 0;
                return result;
            }

            var yamlBlock = content.substring(4, endMatch);
            result.bodyStart = endMatch + 4; // Skip past closing ---\n

            var lines = yamlBlock.split("\n");
            var currentKey = null;
            var currentIndent = 0;
            var arrayValues = [];
            var inArray = false;

            for (var i = 0; i < lines.length; i++) {
                var line = lines[i];
                var trimmed = line.trim();

                if (trimmed === "" || trimmed.indexOf("#") === 0) continue;

                // Check for array item
                if (trimmed.indexOf("- ") === 0) {
                    if (inArray && currentKey) {
                        arrayValues.push(trimmed.substring(2).trim());
                    }
                    continue;
                }

                // Check for key: value
                var colonIdx = trimmed.indexOf(":");
                if (colonIdx > 0) {
                    // Save previous array if we had one
                    if (inArray && currentKey && arrayValues.length > 0) {
                        result.properties[currentKey] = arrayValues;
                    }

                    currentKey = trimmed.substring(0, colonIdx).trim();
                    var val = trimmed.substring(colonIdx + 1).trim();

                    if (val === "" || val === "|" || val === ">") {
                        // Could be array or multiline - check next line
                        inArray = true;
                        arrayValues = [];
                    } else {
                        inArray = false;
                        // Remove quotes if present
                        if ((val.indexOf('"') === 0 && val.lastIndexOf('"') === val.length - 1 && val.length > 1) ||
                            (val.indexOf("'") === 0 && val.lastIndexOf("'") === val.length - 1 && val.length > 1)) {
                            val = val.substring(1, val.length - 1);
                        }
                        result.properties[currentKey] = val;
                    }
                }
            }

            // Save final array if we had one
            if (inArray && currentKey && arrayValues.length > 0) {
                result.properties[currentKey] = arrayValues;
            }

            return result;
        }

        function extractUserSections(content, bodyStart) {
            // Extract user-added sections that should be preserved
            // These are sections NOT managed by the sync (not Parameters, Notes, User Properties)
            var body = content.substring(bodyStart);
            var sections = {};

            // Find all ## headers and their content
            var lines = body.split("\n");
            var currentSection = null;
            var currentContent = [];
            var managedSections = ["parameters", "notes", "user properties"];

            for (var i = 0; i < lines.length; i++) {
                var line = lines[i];
                if (line.indexOf("## ") === 0) {
                    // Save previous section if user-added
                    if (currentSection && managedSections.indexOf(currentSection.toLowerCase()) < 0) {
                        sections[currentSection] = currentContent.join("\n").trim();
                    }
                    currentSection = line.substring(3).trim();
                    currentContent = [];
                } else if (currentSection) {
                    currentContent.push(line);
                }
            }

            // Save final section if user-added
            if (currentSection && managedSections.indexOf(currentSection.toLowerCase()) < 0) {
                sections[currentSection] = currentContent.join("\n").trim();
            }

            return sections;
        }

        function generateMarkdown(eventData, existingContent) {
            var existing = existingContent ? parseYamlFrontmatter(existingContent) : { properties: {}, bodyStart: 0 };
            var userSections = existingContent ? extractUserSections(existingContent, existing.bodyStart) : {};

            // Merge properties: FMOD-managed properties override, user properties preserved
            var fmodProperties = [
                "status", "guid", "banks", "folder_path", "full_path",
                "loop_type", "space", "max_voices", "parameters", "last_synced"
            ];

            var mergedProps = {};

            // First, copy all existing user properties
            for (var key in existing.properties) {
                if (fmodProperties.indexOf(key) < 0) {
                    mergedProps[key] = existing.properties[key];
                }
            }

            // Then add/override FMOD properties
            mergedProps.status = "exists";
            mergedProps.guid = eventData.guid;
            if (eventData.banks.length > 0) {
                mergedProps.banks = eventData.banks;
            }
            mergedProps.folder_path = eventData.folderPath;
            mergedProps.full_path = eventData.fullPath;
            mergedProps.loop_type = eventData.loopType;
            mergedProps.space = eventData.space;
            if (eventData.maxVoices !== "") {
                mergedProps.max_voices = eventData.maxVoices;
            }
            if (eventData.parameters.length > 0) {
                mergedProps.parameters = eventData.parameters.map(function(p) { return p.name; });
            }
            mergedProps.last_synced = getISOTimestamp();

            // Build YAML frontmatter
            var yaml = "---\n";

            // Output properties in a sensible order
            var orderedKeys = ["status", "guid", "banks", "folder_path", "full_path",
                              "loop_type", "space", "max_voices", "parameters", "last_synced"];
            var usedKeys = {};

            for (var i = 0; i < orderedKeys.length; i++) {
                var k = orderedKeys[i];
                if (mergedProps[k] !== undefined && mergedProps[k] !== "") {
                    yaml += formatYamlProperty(k, mergedProps[k]);
                    usedKeys[k] = true;
                }
            }

            // Output remaining user properties
            var remainingKeys = Object.keys(mergedProps).filter(function(k) { return !usedKeys[k]; }).sort();
            if (remainingKeys.length > 0) {
                yaml += "# User-added properties preserved:\n";
                for (var j = 0; j < remainingKeys.length; j++) {
                    yaml += formatYamlProperty(remainingKeys[j], mergedProps[remainingKeys[j]]);
                }
            }

            yaml += "---\n\n";

            // Build markdown body
            var md = "# " + eventData.name + "\n\n";

            // Parameters section
            if (eventData.parameters.length > 0) {
                md += "## Parameters\n";
                md += "| Name | Type | Min | Max | Initial |\n";
                md += "|------|------|-----|-----|--------|\n";
                for (var p = 0; p < eventData.parameters.length; p++) {
                    var param = eventData.parameters[p];
                    md += "| " + param.name + " | " + param.type + " | " + param.min + " | " + param.max + " | " + param.initial + " |\n";
                }
                md += "\n";
            }

            // Notes section
            md += "## Notes\n";
            md += (eventData.notes || "") + "\n\n";

            // User Properties section
            if (eventData.userProperties.length > 0) {
                md += "## User Properties\n";
                for (var u = 0; u < eventData.userProperties.length; u++) {
                    var up = eventData.userProperties[u];
                    md += "- " + up.name;
                    if (up.type) md += " (" + up.type + ")";
                    if (up.value !== "" && up.value !== undefined) md += " = " + up.value;
                    md += "\n";
                }
                md += "\n";
            }

            // Preserved user sections
            var userSectionNames = Object.keys(userSections);
            for (var s = 0; s < userSectionNames.length; s++) {
                var sectionName = userSectionNames[s];
                md += "## " + sectionName + "\n";
                md += userSections[sectionName] + "\n\n";
            }

            return yaml + md;
        }

        function formatYamlProperty(key, value) {
            if (Array.isArray(value)) {
                var out = key + ":\n";
                for (var i = 0; i < value.length; i++) {
                    out += "  - " + yamlEscape(value[i]) + "\n";
                }
                return out;
            }
            return key + ": " + yamlEscape(value) + "\n";
        }

        function generatePlannedMarkdown(existingContent) {
            // For files that exist but have no matching FMOD event - preserve as-is
            return existingContent;
        }

        // -------------------------
        // Config handling
        // -------------------------
        function getConfigPath() {
            var projectPath = studio.project.filePath || "";
            var projectDirectory = projectPath.substring(0, projectPath.lastIndexOf("/"));
            return projectDirectory + "/.fmod-obsidian/config.json";
        }

        function loadConfig() {
            try {
                var configFile = studio.system.getFile(getConfigPath());
                if (configFile.exists()) {
                    if (configFile.open(studio.system.openMode.ReadOnly)) {
                        var content = configFile.readText();
                        configFile.close();
                        return JSON.parse(content);
                    }
                }
            } catch (e) { }
            return null;
        }

        function saveConfig(config) {
            try {
                var configPath = getConfigPath();
                var configFile = studio.system.getFile(configPath);
                if (configFile.open(studio.system.openMode.WriteOnly)) {
                    configFile.writeText(JSON.stringify(config, null, 2));
                    configFile.close();
                    return true;
                }
            } catch (e) { }
            return false;
        }

        function showConfigDialog(existingConfig) {
            var config = existingConfig || { vault_path: "", subfolder: "FMOD Events", mirror_folders: true };
            if (config.mirror_folders === undefined) config.mirror_folders = true;
            var cancelled = false;

            studio.ui.showModalDialog({
                windowTitle: "Obsidian Sync Configuration",
                widgetType: studio.ui.widgetType.Layout,
                layout: studio.ui.layoutType.VBoxLayout,
                spacing: 10,
                contentsMargins: { left: 12, top: 12, right: 12, bottom: 12 },
                minimumSize: { width: 640, height: 0 },
                items: [
                    {
                        widgetType: studio.ui.widgetType.Label,
                        text: "Obsidian Vault Path"
                    },
                    {
                        widgetType: studio.ui.widgetType.PathLineEdit,
                        widgetId: "vaultPath",
                        windowTitle: "Select Obsidian vault folder",
                        text: config.vault_path,
                        minimumWidth: 560,
                        sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding },
                        pathType: studio.ui.pathType.Directory,
                        onEditingFinished: function () { config.vault_path = this.text(); }
                    },
                    {
                        widgetType: studio.ui.widgetType.Label,
                        text: "Subfolder within vault (for FMOD events)"
                    },
                    {
                        widgetType: studio.ui.widgetType.LineEdit,
                        widgetId: "subfolder",
                        text: config.subfolder,
                        minimumWidth: 560,
                        sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding },
                        onEditingFinished: function () { config.subfolder = this.text(); }
                    },
                    {
                        widgetType: studio.ui.widgetType.CheckBox,
                        widgetId: "mirrorFolders",
                        text: "Mirror FMOD folder structure in Obsidian",
                        isChecked: config.mirror_folders,
                        onToggled: function () { config.mirror_folders = this.isChecked(); }
                    },
                    {
                        widgetType: studio.ui.widgetType.Layout,
                        layout: studio.ui.layoutType.HBoxLayout,
                        spacing: 10,
                        items: [
                            {
                                widgetType: studio.ui.widgetType.Spacer,
                                sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding }
                            },
                            {
                                widgetType: studio.ui.widgetType.PushButton,
                                text: "Cancel",
                                onClicked: function () {
                                    cancelled = true;
                                    this.closeDialog();
                                }
                            },
                            {
                                widgetType: studio.ui.widgetType.PushButton,
                                text: "Save & Sync",
                                onClicked: function () {
                                    try { config.vault_path = this.findWidget("vaultPath").text(); } catch (e) { }
                                    try { config.subfolder = this.findWidget("subfolder").text(); } catch (e) { }
                                    try { config.mirror_folders = this.findWidget("mirrorFolders").isChecked(); } catch (e) { }
                                    this.closeDialog();
                                }
                            }
                        ]
                    }
                ]
            });

            if (cancelled) return null;
            return config;
        }

        // -------------------------
        // File scanning
        // -------------------------
        function scanExistingNotes(targetDir, baseDir, notes) {
            if (!notes) notes = {};
            if (!baseDir) baseDir = targetDir;

            try {
                var dir = studio.system.getFile(targetDir);
                if (!dir.exists()) return notes;

                var entries = dir.entryList();
                for (var i = 0; i < entries.length; i++) {
                    var entry = entries[i];
                    if (entry === "." || entry === "..") continue;

                    var filePath = targetDir + "/" + entry;
                    var file = studio.system.getFile(filePath);

                    // Check if it's a directory (try to get entryList)
                    var isDir = false;
                    try {
                        var testEntries = file.entryList();
                        if (testEntries) isDir = true;
                    } catch (e) { }

                    if (isDir) {
                        // Recursively scan subdirectory
                        scanExistingNotes(filePath, baseDir, notes);
                    } else if (entry.slice(-3) === ".md") {
                        if (file.open(studio.system.openMode.ReadOnly)) {
                            var content = file.readText();
                            file.close();

                            var parsed = parseYamlFrontmatter(content);
                            var baseName = entry.substring(0, entry.length - 3); // Remove .md

                            // Calculate relative path from base
                            var relativePath = filePath.substring(baseDir.length + 1);
                            var relativeDir = "";
                            if (relativePath.indexOf("/") >= 0) {
                                relativeDir = relativePath.substring(0, relativePath.lastIndexOf("/"));
                            }

                            // Use full relative path as key for uniqueness
                            var noteKey = relativePath.substring(0, relativePath.length - 3); // Remove .md

                            notes[noteKey] = {
                                filename: entry,
                                baseName: baseName,
                                filePath: filePath,
                                relativeDir: relativeDir,
                                content: content,
                                guid: parsed.properties.guid || null,
                                status: parsed.properties.status || "planned",
                                properties: parsed.properties
                            };
                        }
                    }
                }
            } catch (e) {
                console.log("Error scanning notes: " + e);
            }

            return notes;
        }

        // -------------------------
        // Confirmation dialog for name matches
        // -------------------------
        function showMatchConfirmDialog(noteName, eventName) {
            var result = "skip";

            studio.ui.showModalDialog({
                windowTitle: "Link Note to FMOD Event?",
                widgetType: studio.ui.widgetType.Layout,
                layout: studio.ui.layoutType.VBoxLayout,
                spacing: 10,
                contentsMargins: { left: 12, top: 12, right: 12, bottom: 12 },
                minimumSize: { width: 500, height: 0 },
                items: [
                    {
                        widgetType: studio.ui.widgetType.Label,
                        text: "Found existing note '" + noteName + ".md'\nmatching FMOD event '" + eventName + "'"
                    },
                    {
                        widgetType: studio.ui.widgetType.Label,
                        text: " "
                    },
                    {
                        widgetType: studio.ui.widgetType.Layout,
                        layout: studio.ui.layoutType.VBoxLayout,
                        spacing: 6,
                        items: [
                            {
                                widgetType: studio.ui.widgetType.PushButton,
                                text: "Link - Merge FMOD data, preserve user content",
                                onClicked: function () {
                                    result = "link";
                                    this.closeDialog();
                                }
                            },
                            {
                                widgetType: studio.ui.widgetType.PushButton,
                                text: "Replace - Overwrite note with FMOD data",
                                onClicked: function () {
                                    result = "replace";
                                    this.closeDialog();
                                }
                            },
                            {
                                widgetType: studio.ui.widgetType.PushButton,
                                text: "Skip - Don't update this note",
                                onClicked: function () {
                                    result = "skip";
                                    this.closeDialog();
                                }
                            }
                        ]
                    }
                ]
            });

            return result;
        }

        // -------------------------
        // Main sync logic
        // -------------------------

        // Load or create config
        var config = loadConfig();
        if (!config || !config.vault_path) {
            config = showConfigDialog(config);
            if (!config) {
                alert("Cancelled.");
                return;
            }
            saveConfig(config);
        }

        if (config.mirror_folders === undefined) config.mirror_folders = true;

        if (!config.vault_path || config.vault_path.trim() === "") {
            alert("No vault path configured. Please run again and select a vault.");
            return;
        }

        // Determine target directory
        var targetDir = config.vault_path;
        if (config.subfolder && config.subfolder.trim() !== "") {
            targetDir = config.vault_path + "/" + config.subfolder;
        }

        // Check target directory exists
        var targetDirFile = studio.system.getFile(targetDir);
        if (!targetDirFile.exists()) {
            alert("Target folder does not exist. Please create it first:\n\n" + targetDir);
            return;
        }

        // Scan existing notes (recursively)
        var existingNotes = scanExistingNotes(targetDir);

        // Build GUID lookup from existing notes
        var notesByGuid = {};
        for (var noteKey in existingNotes) {
            var note = existingNotes[noteKey];
            if (note.guid) {
                notesByGuid[note.guid] = noteKey;
            }
        }

        // Also build a lookup by base name (for name matching)
        var notesByBaseName = {};
        for (var noteKey in existingNotes) {
            var note = existingNotes[noteKey];
            if (!notesByBaseName[note.baseName]) {
                notesByBaseName[note.baseName] = noteKey;
            }
        }

        // Collect all FMOD events
        var allEvents = studio.project.model.Event.findInstances();
        allEvents.sort(function (a, b) {
            var pa = tryCall(function () { return a.getPath(); }, "");
            var pb = tryCall(function () { return b.getPath(); }, "");
            if (pa < pb) return -1;
            if (pa > pb) return 1;
            return 0;
        });

        // Track directories created during sync
        var createdDirs = {};

        // Process events
        var stats = {
            updated: 0,
            created: 0,
            linked: 0,
            moved: 0,
            skipped: 0,
            errors: 0
        };

        var processedNotes = {}; // Track which notes we've handled

        for (var i = 0; i < allEvents.length; i++) {
            var e = allEvents[i];

            try {
                var eventPath = e.getPath();
                var eventName = e.name;
                var eventGuid = e.id;
                var sanitizedName = sanitizeFilename(eventName);
                var folderPath = getFolderPathFromEventPath(eventPath);

                // Calculate target file path
                var relativeFilePath;
                if (config.mirror_folders && folderPath) {
                    var segments = folderPath.split("/");
                    var sanitizedSegments = [];
                    for (var s = 0; s < segments.length; s++) {
                        sanitizedSegments.push(sanitizeFilename(segments[s]));
                    }
                    relativeFilePath = sanitizedSegments.join("/") + "/" + sanitizedName;
                } else {
                    relativeFilePath = sanitizedName;
                }

                var filePath = targetDir + "/" + relativeFilePath + ".md";

                // Collect event data
                var eventData = {
                    name: eventName,
                    guid: eventGuid,
                    fullPath: eventPath,
                    folderPath: folderPath,
                    banks: getBankNames(e),
                    loopType: detectLoopType(e),
                    space: detectSpace(e),
                    maxVoices: "",
                    notes: getNotesString(e),
                    parameters: collectEventParameters(e),
                    userProperties: getUserProperties(e)
                };

                try {
                    if (e.automatableProperties && e.automatableProperties.maxVoices !== undefined) {
                        eventData.maxVoices = e.automatableProperties.maxVoices;
                    }
                } catch (err) { }

                // Check for match
                var matchType = null;
                var matchedNote = null;
                var matchedNoteKey = null;

                // Priority 1: GUID match
                if (notesByGuid[eventGuid]) {
                    matchType = "guid";
                    matchedNoteKey = notesByGuid[eventGuid];
                    matchedNote = existingNotes[matchedNoteKey];
                }
                // Priority 2: Name match (check in expected location first, then anywhere)
                else if (existingNotes[relativeFilePath]) {
                    matchType = "name";
                    matchedNoteKey = relativeFilePath;
                    matchedNote = existingNotes[matchedNoteKey];
                }
                else if (notesByBaseName[sanitizedName]) {
                    matchType = "name";
                    matchedNoteKey = notesByBaseName[sanitizedName];
                    matchedNote = existingNotes[matchedNoteKey];
                }

                var existingContent = matchedNote ? matchedNote.content : null;

                if (matchType === "guid") {
                    // Check if file needs to be moved
                    var needsMove = matchedNote.filePath !== filePath;

                    if (needsMove) {
                        // Delete old file
                        var oldFile = studio.system.getFile(matchedNote.filePath);
                        if (oldFile.exists()) {
                            oldFile.remove();
                        }
                    }

                    // Write to new location
                    var markdown = generateMarkdown(eventData, existingContent);
                    if (writeMarkdownFile(filePath, markdown, createdDirs)) {
                        if (needsMove) {
                            stats.moved++;
                        } else {
                            stats.updated++;
                        }
                    } else {
                        stats.errors++;
                    }
                    processedNotes[relativeFilePath] = true;
                    if (matchedNoteKey !== relativeFilePath) {
                        processedNotes[matchedNoteKey] = true;
                    }
                }
                else if (matchType === "name") {
                    // Name match - ask user
                    var action = showMatchConfirmDialog(matchedNote.baseName, eventName);

                    if (action === "link") {
                        // Check if file needs to be moved
                        var needsMove = matchedNote.filePath !== filePath;

                        if (needsMove) {
                            var oldFile = studio.system.getFile(matchedNote.filePath);
                            if (oldFile.exists()) {
                                oldFile.remove();
                            }
                        }

                        // Merge FMOD data with existing content
                        var markdown = generateMarkdown(eventData, existingContent);
                        if (writeMarkdownFile(filePath, markdown, createdDirs)) {
                            stats.linked++;
                        } else {
                            stats.errors++;
                        }
                        processedNotes[relativeFilePath] = true;
                        if (matchedNoteKey !== relativeFilePath) {
                            processedNotes[matchedNoteKey] = true;
                        }
                    }
                    else if (action === "replace") {
                        var needsMove = matchedNote.filePath !== filePath;

                        if (needsMove) {
                            var oldFile = studio.system.getFile(matchedNote.filePath);
                            if (oldFile.exists()) {
                                oldFile.remove();
                            }
                        }

                        // Overwrite completely
                        var markdown = generateMarkdown(eventData, null);
                        if (writeMarkdownFile(filePath, markdown, createdDirs)) {
                            stats.updated++;
                        } else {
                            stats.errors++;
                        }
                        processedNotes[relativeFilePath] = true;
                        if (matchedNoteKey !== relativeFilePath) {
                            processedNotes[matchedNoteKey] = true;
                        }
                    }
                    else {
                        // Skip
                        stats.skipped++;
                        processedNotes[relativeFilePath] = true;
                        if (matchedNoteKey) {
                            processedNotes[matchedNoteKey] = true;
                        }
                    }
                }
                else {
                    // No match - create new
                    var markdown = generateMarkdown(eventData, null);
                    if (writeMarkdownFile(filePath, markdown, createdDirs)) {
                        stats.created++;
                    } else {
                        stats.errors++;
                    }
                    processedNotes[relativeFilePath] = true;
                }

            } catch (err) {
                console.log("Error processing event: " + (e.name || "") + " :: " + err);
                stats.errors++;
            }
        }

        // Count orphaned notes (planned events not in FMOD)
        var orphanCount = 0;
        for (var noteKey in existingNotes) {
            if (!processedNotes[noteKey]) {
                orphanCount++;
            }
        }

        var elapsedSec = Math.round((Date.now() - t0) / 1000);

        var msg =
            "Sync Complete!\n" +
            "----------------------------------\n" +
            "Folders created: " + Object.keys(createdDirs).length + "\n" +
            "Notes created: " + stats.created + "\n" +
            "Notes updated: " + stats.updated + "\n" +
            "Notes moved: " + stats.moved + "\n" +
            "Notes linked: " + stats.linked + "\n" +
            "Skipped: " + stats.skipped + "\n" +
            "Errors: " + stats.errors + "\n" +
            "Orphaned notes: " + orphanCount + "\n" +
            "----------------------------------\n" +
            "Time: " + elapsedSec + "s\n" +
            "Output: " + targetDir;

        alert(msg);
    }
});
