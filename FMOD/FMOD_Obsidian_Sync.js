studio.menu.addMenuItem({
    name: "DRD\\Export for Obsidian...",
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
        // Timestamp helper
        // -------------------------
        function pad2(n) { return (n < 10 ? "0" : "") + n; }

        function getISOTimestamp() {
            var now = new Date();
            return now.getFullYear() + "-" + pad2(now.getMonth() + 1) + "-" + pad2(now.getDate()) +
                   "T" + pad2(now.getHours()) + ":" + pad2(now.getMinutes()) + ":" + pad2(now.getSeconds());
        }

        // -------------------------
        // Main export logic
        // -------------------------

        // Get project info
        var projectPath = studio.project.filePath || "";
        var projectDirectory = projectPath.substring(0, projectPath.lastIndexOf("/"));
        var projectName = projectPath.substring(projectPath.lastIndexOf("/") + 1).replace(/\.fspro$/i, "");

        // Default output path
        var outputPath = projectDirectory + "/obsidian-sync.json";

        // Ask user for output location
        var cancelled = false;
        studio.ui.showModalDialog({
            windowTitle: "Export for Obsidian",
            widgetType: studio.ui.widgetType.Layout,
            layout: studio.ui.layoutType.VBoxLayout,
            spacing: 10,
            contentsMargins: { left: 12, top: 12, right: 12, bottom: 12 },
            minimumSize: { width: 640, height: 0 },
            items: [
                {
                    widgetType: studio.ui.widgetType.Label,
                    text: "Export FMOD events to JSON for Obsidian sync.\n\nThe Obsidian plugin will read this file and create markdown notes."
                },
                {
                    widgetType: studio.ui.widgetType.Label,
                    text: " "
                },
                {
                    widgetType: studio.ui.widgetType.Label,
                    text: "Output JSON file:"
                },
                {
                    widgetType: studio.ui.widgetType.PathLineEdit,
                    widgetId: "outputPath",
                    windowTitle: "Save JSON file",
                    text: outputPath,
                    minimumWidth: 560,
                    sizePolicy: { horizontalPolicy: studio.ui.sizePolicy.MinimumExpanding },
                    pathType: studio.ui.pathType.SaveFileName,
                    nameFilter: "JSON files (*.json)",
                    onEditingFinished: function () { outputPath = this.text(); }
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
                                try { outputPath = this.findWidget("outputPath").text(); } catch (e) { }
                                this.closeDialog();
                            }
                        }
                    ]
                }
            ]
        });

        if (cancelled) {
            return;
        }

        if (!outputPath || outputPath.trim() === "") {
            alert("No output path specified.");
            return;
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

        // Build events array for JSON
        var eventsData = [];

        for (var i = 0; i < allEvents.length; i++) {
            var e = allEvents[i];

            try {
                var eventPath = e.getPath();
                var eventName = e.name;
                var eventGuid = e.id;
                var folderPath = getFolderPathFromEventPath(eventPath);

                var maxVoices = "";
                try {
                    if (e.automatableProperties && e.automatableProperties.maxVoices !== undefined) {
                        maxVoices = e.automatableProperties.maxVoices;
                    }
                } catch (err) { }

                var eventData = {
                    name: eventName,
                    guid: eventGuid,
                    full_path: eventPath,
                    folder_path: folderPath,
                    banks: getBankNames(e),
                    loop_type: detectLoopType(e),
                    space: detectSpace(e),
                    max_voices: maxVoices,
                    notes: getNotesString(e),
                    parameters: collectEventParameters(e),
                    user_properties: getUserProperties(e)
                };

                eventsData.push(eventData);

            } catch (err) {
                console.log("Error processing event: " + (e.name || "") + " :: " + err);
            }
        }

        // Build final JSON structure
        var exportData = {
            exported_at: getISOTimestamp(),
            project_name: projectName,
            project_path: projectPath,
            event_count: eventsData.length,
            events: eventsData
        };

        // Write JSON file
        var file = studio.system.getFile(outputPath);
        if (file.open(studio.system.openMode.WriteOnly)) {
            file.writeText(JSON.stringify(exportData, null, 2));
            file.close();

            var elapsedMs = Date.now() - t0;

            alert(
                "Export Complete!\n" +
                "----------------------------------\n" +
                "Events exported: " + eventsData.length + "\n" +
                "Time: " + elapsedMs + "ms\n" +
                "----------------------------------\n" +
                "Output: " + outputPath + "\n\n" +
                "Next step: Open Obsidian and run\n" +
                "'FMOD Sync: Import from JSON'"
            );
        } else {
            alert("Error: Could not write to file:\n" + outputPath);
        }
    }
});
