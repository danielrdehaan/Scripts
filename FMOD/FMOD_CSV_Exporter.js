studio.menu.addMenuItem({
    name: "DRD\\Export project data to csv...",
    execute: function () {
        var t0 = Date.now();

        // -------------------------
        // Helpers
        // -------------------------
        function csvEscape(v) {
            if (v === null || v === undefined) return "";
            var s = "" + v;
            // Normalize newlines to \n to keep CSV consistent
            s = s.replace(/\r\n/g, "\n").replace(/\r/g, "\n");
            // Escape double quotes per RFC4180
            s = s.replace(/"/g, '""');
            return s;
        }

        function csvRow(arr) {
            // Always quote everything; safest for spreadsheets
            return '"' + arr.map(csvEscape).join('","') + '"\n';
        }

        function safeGet(obj, keyPath) {
            // keyPath: "a.b.c"
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
            // Robust-ish: catch both spellings and plugin spatializers.
            // Known built-ins include SpatialiserEffect and ObjectSpatialiserEffect.
            var t = "";
            try { t = effectObj && effectObj.type ? ("" + effectObj.type) : ""; } catch (e) { }
            if (!t) {
                try { t = effectObj && effectObj.isOfExactType ? "" : ""; } catch (e) { }
            }

            // Prefer the scripting "isOfExactType" when available
            try {
                if (effectObj && effectObj.isOfExactType) {
                    if (effectObj.isOfExactType("SpatialiserEffect")) return true;
                    if (effectObj.isOfExactType("ObjectSpatialiserEffect")) return true;
                    // Some projects use third-party spatializers; if we can see their type name, detect by substring:
                }
            } catch (e) { }

            // Fallback substring detection
            var name = "";
            try { name = effectObj && effectObj.name ? ("" + effectObj.name) : ""; } catch (e) { }
            var typeName = "";
            try { typeName = effectObj && effectObj.type ? ("" + effectObj.type) : ""; } catch (e) { }

            var blob = (name + " " + typeName).toLowerCase();
            return (blob.indexOf("spatial") >= 0); // catches spatialiser/spatializer/object spatializer/etc.
        }

        function detectSpace(e) {
            // FMOD considers an event 3D if it has a spatializer on the master track.
            // We'll try to detect this via effect chains; if we can't reliably tell, return "Unknown".
            try {
                // Master track effect chain
                if (e.masterTrack && e.masterTrack.mixerGroup && e.masterTrack.mixerGroup.relationships.effectChain) {
                    var chain = e.masterTrack.mixerGroup.relationships.effectChain.destinations[0];
                    if (chain && chain.relationships.effects) {
                        var effects = chain.relationships.effects.destinations;
                        for (var i = 0; i < effects.length; i++) {
                            if (isLikelySpatialiserEffect(effects[i])) return "3D";
                        }
                    }
                }

                // Also check return tracks if accessible (some projects put spatialization there)
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
            // "Loop" if any LoopRegion marker exists
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
            // event:/A/B/C/Name -> A/B/C
            var cleanPath = (eventPath || "").replace("event:/", "");
            var parts = cleanPath.split("/");
            if (parts.length <= 1) return "";
            parts.pop();
            return parts.join("/");
        }

        function getBankString(e) {
            try {
                if (e.relationships.banks && e.relationships.banks.destinations && e.relationships.banks.destinations.length > 0) {
                    var names = e.relationships.banks.destinations.map(function (b) { return b.name; }).filter(Boolean);
                    names.sort();
                    return names.join(" | ");
                }
            } catch (err) { }
            return "Unassigned";
        }

        function getUserPropertiesString(e) {
            // Format: Key=Value | Key2=Value2 (include type when possible)
            try {
                if (!e.userProperties || e.userProperties.length === 0) return "";
                var out = [];
                for (var i = 0; i < e.userProperties.length; i++) {
                    var p = e.userProperties[i];

                    var key = tryCall(function () { return p.name; }, "");
                    var typ = tryCall(function () { return p.type; }, "");
                    var val = "";

                    // Try common value shapes
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

                    var label = key;
                    if (typ !== "" && typ !== undefined && typ !== null) label += " (" + typ + ")";
                    if (val !== "" && val !== undefined && val !== null) label += "=" + val;

                    out.push(label);
                }
                out.sort();
                return out.join(" | ");
            } catch (err) {
                return "";
            }
        }

        function getNotesString(e) {
            // Notes field name isn't consistent across versions; probe a few likely keys.
            var candidates = ["notes", "note", "comment", "comments", "description"];
            for (var i = 0; i < candidates.length; i++) {
                var v = safeGet(e, candidates[i]);
                if (v !== "" && v !== null && v !== undefined) return v;
            }
            return "";
        }

        function formatParamDetails(p) {
            // Try to extract useful info: type/min/max/initial/labels (if any)
            // Works across versions by probing multiple possible shapes.
            var name = tryCall(function () { return (p.presetOwner) ? p.presetOwner.name : p.name; }, "");
            if (!name) name = tryCall(function () { return p.name; }, "");

            var preset = tryCall(function () { return p.preset || (p.presetOwner ? p.presetOwner.preset : null) || p; }, p);

            var typ = tryCall(function () { return preset.type; }, "");
            var min = tryCall(function () { return preset.min; }, "");
            var max = tryCall(function () { return preset.max; }, "");
            var init = tryCall(function () { return preset.initialValue; }, "");
            var labels = tryCall(function () { return preset.enumerationLabels; }, "");

            // enumerationLabels can be null in some versions; tolerate it
            var labelStr = "";
            if (labels && labels.length && typeof labels.join === "function") {
                labelStr = labels.join(",");
            }

            var bits = [];
            if (typ !== "") bits.push("type=" + typ);
            if (min !== "") bits.push("min=" + min);
            if (max !== "") bits.push("max=" + max);
            if (init !== "") bits.push("init=" + init);
            if (labelStr !== "") bits.push("labels=" + labelStr);

            if (bits.length === 0) return name;
            return name + " (" + bits.join(" ") + ")";
        }

        function collectEventParameters(e) {
            // Return { names: [..], details: [..] }
            var byName = {}; // name -> details string
            var total = {};

            function addParam(p) {
                var name = tryCall(function () { return (p.presetOwner) ? p.presetOwner.name : p.name; }, "");
                if (!name || name === "undefined") return;
                byName[name] = formatParamDetails(p);
                total[name] = true;
            }

            try {
                if (typeof e.getParameterPresets === "function") {
                    e.getParameterPresets().forEach(addParam);
                }
            } catch (err) { }

            // Some versions expose parameters via relationship proxies
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
            var details = names.map(function (n) { return byName[n]; });

            return { names: names, details: details, total: total };
        }

        // -------------------------
        // Build export rows
        // -------------------------
        var allEvents = studio.project.model.Event.findInstances();

        // Deterministic ordering for clean diffs
        allEvents.sort(function (a, b) {
            var pa = tryCall(function () { return a.getPath(); }, "");
            var pb = tryCall(function () { return b.getPath(); }, "");
            if (pa < pb) return -1;
            if (pa > pb) return 1;
            return 0;
        });

        var errors = [];
        var totalParamsFound = {};
        var eventCount = 0;

        // New CSV header (no L1/L2/L3; include full folder path)
        var csv = "";
        csv += [
            "Banks",
            "Folder Path",
            "Event Name",
            "Full Path",
            "GUID",
            "Loop Type",
            "Space",
            "Max Voices",
            "Is Default",
            "Notes",
            "User Properties",
            "Parameters",
            "Parameter Details"
        ].join(",") + "\n";

        allEvents.forEach(function (e) {
            try {
                var eventPath = e.getPath();
                var name = e.name;
                var guid = e.id;

                var folderPath = getFolderPathFromEventPath(eventPath);
                var banks = getBankString(e);
                var loopType = detectLoopType(e);
                var space = detectSpace(e);

                var maxVoices = "";
                try {
                    if (e.automatableProperties && e.automatableProperties.maxVoices !== undefined) {
                        maxVoices = e.automatableProperties.maxVoices;
                    }
                } catch (err) { }

                var isDefault = "";
                try { if (e.isDefault !== undefined) isDefault = e.isDefault ? "true" : "false"; } catch (err) { }

                var notes = getNotesString(e);
                var userProps = getUserPropertiesString(e);

                var paramInfo = collectEventParameters(e);
                paramInfo.names.forEach(function (n) { totalParamsFound[n] = true; });

                var paramNamesStr = paramInfo.names.join(" | ");
                var paramDetailsStr = paramInfo.details.join(" | ");

                csv += csvRow([
                    banks,
                    folderPath,
                    name,
                    eventPath,
                    guid,
                    loopType,
                    space,
                    maxVoices,
                    isDefault,
                    notes,
                    userProps,
                    paramNamesStr,
                    paramDetailsStr
                ]);

                eventCount++;
            } catch (err) {
                var p = "";
                try { p = e.getPath(); } catch (e2) { }
                errors.push({ path: p, name: (e && e.name) ? e.name : "", error: "" + err });
                console.log("Error processing event: " + p + " :: " + err);
            }
        });

        // -------------------------
        // Export destination UI
        // (Directory picker + separate filename field)
        // -------------------------
        var projectPath = studio.project.filePath || "";
        var projectDirectory = projectPath.substring(0, projectPath.lastIndexOf("/"));
        var projectFile = projectPath.substring(projectPath.lastIndexOf("/") + 1);
        var projectName = projectFile.replace(/\.fspro$/i, "") || "FMOD_Project";

        function pad2(n) { return (n < 10 ? "0" : "") + n; }
        var now = new Date();
        var dateStamp = now.getFullYear() + "-" + pad2(now.getMonth() + 1) + "-" + pad2(now.getDate());

        var defaultDir = projectDirectory || "";
        var defaultFileName = projectName + "_EventExport_" + dateStamp + ".csv";

        var exportDir = defaultDir;
        var exportFileName = defaultFileName;
        var cancelled = false;

        studio.ui.showModalDialog({
            windowTitle: "Export CSV",
            widgetType: studio.ui.widgetType.Layout,
            layout: studio.ui.layoutType.VBoxLayout,
            spacing: 10,
            contentsMargins: { left: 12, top: 12, right: 12, bottom: 12 },
            minimumSize: { width: 640, height: 0 },
            items: [
                {
                    widgetType: studio.ui.widgetType.Label,
                    text: "Export folder"
                },
                {
                    widgetType: studio.ui.widgetType.PathLineEdit,
                    widgetId: "exportDir",
                    windowTitle: "Select export folder",
text: defaultDir,
                    minimumWidth: 560,
                    sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding },
                    pathType: studio.ui.pathType.Directory,
                    onEditingFinished: function () { exportDir = this.text(); }
                },
                {
                    widgetType: studio.ui.widgetType.Label,
                    text: "File name"
                },
                {
                    widgetType: studio.ui.widgetType.LineEdit,
                    widgetId: "exportFileName",
text: defaultFileName,
                    minimumWidth: 560,
                    sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding },
                    onEditingFinished: function () { exportFileName = this.text(); }
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
                            text: "Export",
                            onClicked: function () {
                                // Pull latest values (in case user didn't defocus fields)
                                try { exportDir = this.findWidget("exportDir").text(); } catch (e) { }
                                try { exportFileName = this.findWidget("exportFileName").text(); } catch (e) { }
                                this.closeDialog();
                            }
                        }
            ]
                }
            ]
        });

        if (cancelled) {
            alert("Cancelled.");
            return;
        }

        if (!exportDir || exportDir.trim() === "") {
            alert("Cancelled: no export folder selected.");
            return;
        }

        if (!exportFileName || exportFileName.trim() === "") {
            exportFileName = defaultFileName;
        }

        // Ensure filename ends with .csv
        if (!/\.csv$/i.test(exportFileName)) {
            exportFileName = exportFileName + ".csv";
        }

        // Join directory + filename robustly for / or \ paths
        var sep = "/";
        if (exportDir.indexOf("\\") >= 0 && exportDir.indexOf("/") < 0) sep = "\\";
        // Trim trailing separators
        exportDir = exportDir.replace(/[\/\\]+$/, "");
        var savePath = exportDir + sep + exportFileName;

        // Write main CSV

        var okMain = false;
        var file = studio.system.getFile(savePath);
        if (file.open(studio.system.openMode.WriteOnly)) {
            file.writeText(csv);
            file.close();
            okMain = true;
        }

        // Write errors CSV (if any) alongside the main export
        var errorsPath = savePath.replace(/\.csv$/i, "_errors.csv");
        var okErr = true;

        if (errors.length > 0) {
            var errCsv = "Full Path,Event Name,Error\n";
            errors.forEach(function (r) {
                errCsv += csvRow([r.path, r.name, r.error]);
            });

            var ef = studio.system.getFile(errorsPath);
            if (ef.open(studio.system.openMode.WriteOnly)) {
                ef.writeText(errCsv);
                ef.close();
            } else {
                okErr = false;
            }
        }

        if (!okMain) {
            alert("ERROR: Could not write the CSV.\nCheck folder permissions or try a different location.\n\nPath:\n" + savePath);
            return;
        }

        var totalUniqueParams = Object.keys(totalParamsFound).length;
        var elapsedSec = Math.round((Date.now() - t0) / 1000);

        var msg =
            "SUCCESS!\n" +
            "----------------------------------\n" +
            "Events exported: " + eventCount + "\n" +
            "Unique parameters: " + totalUniqueParams + "\n" +
            "Errors: " + errors.length + "\n" +
            "Time: " + elapsedSec + "s\n" +
            "CSV: " + savePath + "\n";

        if (errors.length > 0) {
            msg += "Errors CSV: " + errorsPath + (okErr ? "" : " (FAILED TO WRITE)") + "\n";
        }

        msg += "----------------------------------";

        alert(msg);
    }
});
