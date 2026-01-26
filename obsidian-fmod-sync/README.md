# FMOD Sync for Obsidian

An Obsidian plugin that imports FMOD Studio event data into your vault as markdown notes, enabling you to document, organize, and link your game audio alongside other project documentation.

## Features

- **Import FMOD Events**: Creates markdown notes for each FMOD event with metadata (banks, parameters, loop type, 2D/3D, etc.)
- **Multi-Project Support**: Manage multiple FMOD projects with independent output folders
- **GUID-Based Tracking**: Maintains links even when events are renamed or moved in FMOD
- **Folder Mirroring**: Recreates FMOD's folder structure in your vault
- **Newer Export Detection**: Automatically detects when a newer JSON export is available
- **FMOD Studio Integration**: Open FMOD projects directly from Obsidian with version-matched installations
- **Companion Script Installer**: Install the FMOD export script directly from the plugin settings

## Installation

### Plugin Installation

1. Build the plugin:
   ```bash
   cd obsidian-fmod-sync
   npm install
   npm run build
   ```

2. Copy the built files to your vault's plugins folder:
   ```bash
   cp main.js manifest.json styles.css <your-vault>/.obsidian/plugins/fmod-sync/
   ```

3. Enable the plugin in Obsidian: Settings > Community plugins > FMOD Sync

### FMOD Studio Setup

1. Open the plugin settings in Obsidian
2. Under "FMOD Studio Installations", click **Add Installation** and select your FMOD Studio application
3. If the export script isn't installed, click **Install Export Script** to add it automatically
4. Restart FMOD Studio to load the new script

## Usage

### Exporting from FMOD Studio

1. Open your FMOD project in FMOD Studio
2. Go to **Scripts > DRD > Export for Obsidian...**
3. Select an output directory (the plugin remembers your last export location)
4. Click **Export**

The script creates a timestamped JSON file (e.g., `ProjectName_2024-01-25_143052.json`).

### Importing into Obsidian

1. Open Obsidian and go to **Settings > FMOD Sync**
2. Click **Add Project** and configure:
   - **Vault folder**: Where event notes will be created
   - **JSON file path**: Path to the exported JSON file
3. Click the **Sync** button (↻) on the project card, or use:
   - The FMOD ribbon icon
   - Command palette: "FMOD Sync: Import from JSON"

### Multiple Projects

When you have multiple projects configured:
- The sync command shows a picker to choose which project(s) to sync
- Select "Sync All" to sync all projects at once
- Each project maintains its own output folder and settings

### Newer Export Detection

The plugin automatically checks for newer exports in the same directory:
- A badge appears on project cards when a newer export is detected
- The sync button highlights to indicate an update is available
- Clicking sync automatically uses the newer file

## Generated Note Format

Each event note includes:

**Frontmatter (YAML)**:
- `status`: "exists"
- `guid`: FMOD event GUID
- `project`: Project name
- `banks`: Assigned banks
- `folder_path`: FMOD folder location
- `full_path`: Complete event path
- `loop_type`: One-shot or Loop
- `space`: 2D or 3D
- `max_voices`: Voice limit
- `parameters`: Parameter names
- `last_synced`: Export timestamp

**Body**:
- Parameters table with type, range, and initial values
- Notes from FMOD
- User properties

Custom sections you add to notes are preserved during sync.

## Settings

### FMOD Studio Installations

Manage installed FMOD Studio versions:
- **Version badge**: Detected version number
- **Checkmark**: Export script is installed
- **Install Export Script**: Install the companion script for this version

### Projects

Configure FMOD projects to sync:
- **Vault folder**: Destination folder in your vault
- **JSON file path**: Path to the export JSON file
- **Version dropdown**: Select which FMOD installation to use for opening the project
- **Open FMOD**: Launch the project in FMOD Studio
- **Sync**: Import events from the JSON file

## Troubleshooting

### Export script not appearing in FMOD Studio
- Ensure you clicked "Install Export Script" in the plugin settings
- Restart FMOD Studio after installing the script
- Check that the script exists in the FMOD Scripts folder

### Version mismatch warning
- The warning icon indicates the project was created with a different FMOD version
- Use the version dropdown to select the correct installation
- The plugin will auto-select matching versions when available

### Events not updating
- Check that you're exporting to the same directory
- Verify the JSON file path in project settings
- Look for the "New export available" badge

## License

MIT
