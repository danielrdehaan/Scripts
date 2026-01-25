# Scripts Repository

A collection of scripts for audio workflow enhancement across Unity (FMOD integration), FMOD Studio, Obsidian, and Reaper.

## Contents

- [FMOD to Obsidian Sync](#fmod-to-obsidian-sync) - Two-stage system for syncing FMOD events to Obsidian notes
- [Unity FMOD Scripts](#unity-fmod-scripts) - UI controls for FMOD audio in Unity
- [FMOD Studio Scripts](#fmod-studio-scripts) - CSV export and Obsidian sync for FMOD Studio
- [Reaper Scripts](#reaper-scripts) - BWF metadata utilities

---

## FMOD to Obsidian Sync

A two-stage system that exports FMOD Studio events to Obsidian as markdown notes with full metadata, folder structure mirroring, and bi-directional workflow support.

### Architecture

```
FMOD Studio                         Obsidian
┌─────────────────────┐            ┌─────────────────────────────┐
│ FMOD_Obsidian_Sync  │            │ fmod-sync plugin            │
│                     │            │                             │
│ • Reads project     │   JSON     │ • Reads JSON                │
│ • Collects events   │ ────────►  │ • Creates folders           │
│ • Exports JSON      │            │ • Writes markdown files     │
│                     │            │ • Preserves user content    │
└─────────────────────┘            └─────────────────────────────┘
```

### Why Two Stages?

FMOD Studio's JavaScript API cannot create directories, making it impossible to mirror folder structures directly. The two-stage approach solves this:

1. **FMOD Script** exports event data to a simple JSON file (no folder creation needed)
2. **Obsidian Plugin** reads the JSON and creates markdown files with folders using Obsidian's full filesystem API

### Installation

#### FMOD Studio Script

1. Copy `FMOD/FMOD_Obsidian_Sync.js` to your FMOD Studio Scripts folder:
   - **macOS**: `~/Library/Application Support/FMOD Studio/Scripts/`
   - **Windows**: `%APPDATA%\FMOD Studio\Scripts\`
2. Restart FMOD Studio
3. Access via menu: **Scripts > DRD > Export for Obsidian...**

#### Obsidian Plugin

1. Build the plugin:
   ```bash
   cd obsidian-fmod-sync
   npm install
   npm run build
   ```

2. Copy to your vault:
   ```bash
   mkdir -p "/path/to/vault/.obsidian/plugins/fmod-sync"
   cp main.js manifest.json styles.css "/path/to/vault/.obsidian/plugins/fmod-sync/"
   ```

3. Enable in Obsidian:
   - Settings → Community plugins → Enable "FMOD Sync"
   - Configure the JSON file path in plugin settings

### Usage

1. **In FMOD Studio**: Run **Scripts > DRD > Export for Obsidian...**
   - Choose output location (default: `obsidian-sync.json` in project folder)
   - Click Export

2. **In Obsidian**: Run command **FMOD Sync: Import from JSON**
   - Or click the audio file icon in the ribbon
   - Notes are created/updated in configured output folder

### Features

- **Folder Mirroring**: Creates subfolders matching FMOD event hierarchy
- **GUID Tracking**: Events matched by GUID for reliable updates even after renames
- **Planned Events**: Create notes in Obsidian first, they'll be linked when the event is created in FMOD
- **Content Preservation**: User-added frontmatter properties and markdown sections are preserved on sync
- **File Moves**: When events move in FMOD, notes are moved to match

### JSON Structure

```json
{
  "exported_at": "2024-01-24T10:30:00",
  "project_name": "MyGame",
  "event_count": 150,
  "events": [
    {
      "name": "Footstep_Grass",
      "guid": "{abc123-def456}",
      "full_path": "event:/Player/Footsteps/Footstep_Grass",
      "folder_path": "Player/Footsteps",
      "banks": ["SFX", "Gameplay"],
      "loop_type": "One-shot",
      "space": "3D",
      "max_voices": 4,
      "notes": "Player footstep on grass surface",
      "parameters": [
        { "name": "Surface", "type": "Labeled", "min": 0, "max": 3, "initial": 0 }
      ],
      "user_properties": [
        { "name": "Category", "type": "String", "value": "Foley" }
      ]
    }
  ]
}
```

### Generated Markdown

```markdown
---
status: exists
guid: "{abc123-def456}"
banks:
  - SFX
  - Gameplay
folder_path: Player/Footsteps
full_path: "event:/Player/Footsteps/Footstep_Grass"
loop_type: One-shot
space: 3D
max_voices: 4
parameters:
  - Surface
last_synced: 2024-01-24T10:30:00
---

# Footstep_Grass

## Parameters
| Name | Type | Min | Max | Initial |
|------|------|-----|-----|---------|
| Surface | Labeled | 0 | 3 | 0 |

## Notes
Player footstep on grass surface

## User Properties
- Category (String) = Foley
```

### Plugin Settings

| Setting | Description | Default |
|---------|-------------|---------|
| JSON file path | Absolute path to `obsidian-sync.json` | (none) |
| Output folder | Folder in vault for event notes | `FMOD Events` |
| Mirror folder structure | Create subfolders matching FMOD hierarchy | `true` |

---

## Unity FMOD Scripts

C# scripts for controlling FMOD audio through Unity UI elements.

**Requirements**: Unity 2022.3.10f1+, FMOD 2.02.17+

### Scripts

| Script | Purpose |
|--------|---------|
| `FMODButtonEventControl.cs` | Trigger FMOD events on UI button hover/click |
| `FMODSliderParameterControl.cs` | Control FMOD parameters via UI sliders |
| `FMODToggleParameterControl.cs` | Control binary FMOD parameters via toggles |
| `FMODAnimationEventTriggers.cs` | Trigger FMOD events from animation timeline |

### Editor Tools

| Script | Purpose |
|--------|---------|
| `FMODSceneManager.cs` | Dockable window showing all FMOD references across loaded scenes |
| `FMODSceneScanner.cs` | Reflection-based scanner for detecting FMOD fields |
| `FMODFieldInfo.cs` | Data model for scan results |

See [Unity/FMOD/README.md](./Unity/FMOD/README.md) for detailed usage.

---

## FMOD Studio Scripts

JavaScript scripts that run inside FMOD Studio.

**Installation**: Copy `.js` files to FMOD Studio's Scripts folder and restart.

### Scripts

| Script | Menu Location | Purpose |
|--------|---------------|---------|
| `FMOD_CSV_Exporter.js` | DRD > Export to CSV... | Export all events to CSV with full metadata |
| `FMOD_Obsidian_Sync.js` | DRD > Export for Obsidian... | Export events to JSON for Obsidian plugin |

### CSV Exporter Columns

Banks, Folder Path, Event Name, Full Path, GUID, Loop Type, Space (2D/3D), Max Voices, Is Default, Notes, User Properties, Parameters, Parameter Details

---

## Reaper Scripts

Lua scripts for Reaper DAW.

### Scripts

| Script | Purpose |
|--------|---------|
| `DRD_Open-Original-Reaper-Project-From-Path-In-BWF-Metadata.lua` | Opens source project from BWF metadata |

The BWF metadata script expects `BWF:Description` to start with `RPP:` followed by a project path.

---

## Contributing

Contributions welcome! Submit issues or pull requests.

## Contact

[Daniel R. Dehaan](http://www.danielrdehaan.com/)
