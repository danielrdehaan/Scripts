# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

A collection of scripts for audio workflow enhancement across Unity (FMOD integration), FMOD Studio, and Reaper.

## Structure

- **Unity/FMOD/**: C# scripts for FMOD audio control in Unity
  - `FMODButtonEventControl.cs` - Trigger FMOD events on UI button hover/click
  - `FMODSliderParameterControl.cs` - Control FMOD parameters via UI sliders
  - `FMODToggleParameterControl.cs` - Control binary FMOD parameters via toggles
  - `FMODAnimationEventTriggers.cs` - Trigger FMOD events from animation timeline

- **Unity/FMOD/Editor/**: Editor-only tools (excluded from builds)
  - `FMODSceneManager.cs` - Dockable window showing all FMOD references across loaded scenes
  - `FMODSceneScanner.cs` - Reflection-based scanner for detecting FMOD fields
  - `FMODFieldInfo.cs` - Data model for scan results

- **FMOD/**: JavaScript scripts for FMOD Studio
  - `FMOD_CSV_Exporter.js` - Exports all project events to CSV with metadata (banks, parameters, 2D/3D, loop type, etc.)
  - `FMOD_Obsidian_Sync.js` - Exports event data to JSON for Obsidian plugin consumption

- **obsidian-fmod-sync/**: Obsidian plugin for importing FMOD event data
  - `main.ts` - Plugin implementation (TypeScript)
  - `manifest.json` - Plugin metadata
  - `package.json` - Dependencies
  - Build with: `npm install && npm run build`

- **Reaper/**: Lua scripts for Reaper DAW
  - `DRD_Open-Original-Reaper-Project-From-Path-In-BWF-Metadata.lua` - Opens source project from BWF metadata

## Unity/FMOD Scripts

**Requirements**: Unity 2022.3.10f1+, FMOD 2.02.17+

**Key dependencies**:
```csharp
using FMODUnity;  // RuntimeManager, EventReference
using UnityEngine.UI;  // Button, Slider, Toggle
using UnityEngine.EventSystems;  // EventTrigger
```

**Pattern**: All FMOD scripts use serializable structs with arrays for Inspector configuration, linking UI elements to FMOD events/parameters at runtime.

**Important**: FMOD parameter names must exactly match those defined in the FMOD project. Slider/toggle value ranges should match FMOD parameter ranges (no value scaling is implemented).

## FMOD Studio Scripts

**Installation**: Copy `.js` files to FMOD Studio's Scripts folder and restart FMOD Studio. Scripts appear in the Scripts menu.

**Pattern**: Scripts use the `studio.` API for project access, UI dialogs, and file operations. Menu items are registered via `studio.menu.addMenuItem()`.

**CSV Exporter columns**: Banks, Folder Path, Event Name, Full Path, GUID, Loop Type (One-shot/Loop), Space (2D/3D), Max Voices, Is Default, Notes, User Properties, Parameters, Parameter Details.

**Obsidian Sync (Two-Stage Architecture)**:
1. FMOD script exports events to `obsidian-sync.json` (menu: "DRD > Export for Obsidian...")
2. Obsidian plugin reads JSON and creates markdown files with folder structure

JSON structure:
```json
{
  "exported_at": "2024-01-24T10:30:00",
  "project_name": "MyGame",
  "events": [{ "name", "guid", "full_path", "folder_path", "banks", "loop_type", "space", "parameters", "user_properties", "notes" }]
}
```

## Obsidian FMOD Sync Plugin

**Installation**:
1. Build: `cd obsidian-fmod-sync && npm install && npm run build`
2. Copy `main.js`, `manifest.json`, `styles.css` to `.obsidian/plugins/fmod-sync/`
3. Enable in Obsidian Settings > Community plugins

**Settings**:
- JSON file path: Path to exported `obsidian-sync.json`
- Output folder: Where to create event notes (default: "FMOD Events")
- Mirror folder structure: Match FMOD folder hierarchy (default: true)

**Features**:
- GUID-based matching for established events, name-based matching for new links
- Preserves user-added frontmatter properties and markdown sections
- Automatically creates folders matching FMOD structure
- Command: "FMOD Sync: Import from JSON" (also accessible via ribbon icon)

## Reaper Scripts

**Pattern**: Scripts use `reaper.` API calls. The BWF metadata script expects `BWF:Description` to start with `RPP:` followed by a project path.
