# Pro Tools to Reaper Export

Export a Pro Tools session to a Reaper .rpp project file via the PTSL (Pro Tools Scripting Library) gRPC API.

## Features

- Track structure export (audio and instrument tracks)
- Audio clip placement with correct timeline positions
- Source audio file references
- Volume automation
- Pan automation

## Current Status

| Feature | Status | Notes |
|---------|--------|-------|
| Session info | Working | Name, path, sample rate |
| Track list | Working | Audio and instrument tracks exported |
| File locations | Working | Maps file IDs to paths |
| Clip positions | **Requires Private API** | Contact Avid for access |
| Volume automation | **Not implemented in PT** | GetTrackControlBreakpoints returns error |
| Pan automation | **Not implemented in PT** | GetTrackControlBreakpoints returns error |

The script exports track structure successfully. Clip positions and automation require additional API access.

## Requirements

- **Pro Tools 2025.10+** (required for `GetTrackControlBreakpoints` automation API)
- **Python 3.8+**
- Pro Tools must be running with a session open

## Setup

### 1. Install Python dependencies

```bash
cd scripts/pt_to_reaper
pip install -r requirements.txt
```

### 2. Compile the protobuf stubs

```bash
python -m grpc_tools.protoc \
  -I../../PTSL_SDK_CPP.2025.10.0.1232349/Source \
  --python_out=proto \
  --grpc_python_out=proto \
  ../../PTSL_SDK_CPP.2025.10.0.1232349/Source/PTSL.proto
```

This generates `proto/PTSL_pb2.py` and `proto/PTSL_pb2_grpc.py`.

## Usage

With Pro Tools running and a session open:

```bash
python pt_to_reaper.py --output ~/Desktop/exported.rpp
```

### Options

| Option | Description |
|--------|-------------|
| `--output`, `-o` | Required. Output path for the .rpp file |
| `--host` | Host where Pro Tools is running (default: localhost) |
| `--verbose`, `-v` | Enable verbose output |

### Examples

```bash
# Basic export
python pt_to_reaper.py -o ~/Desktop/my_session.rpp

# Verbose mode to see progress
python pt_to_reaper.py -o ~/Desktop/my_session.rpp -v
```

## What Gets Exported

### Tracks
- Audio tracks
- Instrument tracks
- Track names preserved

### Audio Clips
- Timeline position
- Clip length
- Source offset (in-point)
- Audio file path

### Automation
- Volume automation breakpoints
- Pan automation breakpoints

## Limitations

### PTSL Private API Requirement

Some PTSL features require "Private API" access which must be obtained from Avid:

- **GetPlaylistElements** - Required for clip timeline positions
- **GetClipList** - Returns empty without Private API

Contact audiosdk@avid.com to request access for your application.

### Other Limitations

- MIDI data is not exported (only audio references)
- Plugin data is not exported
- Routing/sends are not exported
- Clip gain automation is not exported (only track volume)
- Fades are not converted (would require additional mapping)
- GetTrackControlBreakpoints may not be available in all Pro Tools versions

## Troubleshooting

### "Could not connect to Pro Tools"

- Ensure Pro Tools is running
- Ensure a session is open
- The PTSL server runs on port 31416 (localhost only)

### Proto compilation errors

Ensure you have the correct path to `PTSL.proto`:
```bash
ls ../../PTSL_SDK_CPP.2025.10.0.1232349/Source/PTSL.proto
```

### Import errors

After compiling protos, you may need to fix the import in `PTSL_pb2_grpc.py`:
```python
# Change:
import PTSL_pb2 as PTSL__pb2
# To:
from . import PTSL_pb2 as PTSL__pb2
```

## File Structure

```
pt_to_reaper/
├── pt_to_reaper.py     # Main CLI script
├── ptsl_client.py      # gRPC client wrapper
├── rpp_writer.py       # RPP file generator
├── requirements.txt    # Python dependencies
├── README.md          # This file
└── proto/             # Compiled protobuf stubs
    ├── __init__.py
    ├── PTSL_pb2.py        # (generated)
    └── PTSL_pb2_grpc.py   # (generated)
```

## API Reference

### PTSL Commands Used

| Command | Purpose |
|---------|---------|
| `RegisterConnection` | Establish session with Pro Tools |
| `GetSessionName` | Get session name |
| `GetSessionPath` | Get session file path |
| `GetSessionSampleRate` | Get sample rate |
| `GetTrackList` | List all tracks |
| `GetFileLocation` | Map file IDs to paths |
| `GetClipList` | List all clips in session |
| `GetTrackPlaylists` | Get playlists for a track |
| `GetPlaylistElements` | Get clips on timeline |
| `GetTrackControlBreakpoints` | Get automation data |

### RPP Format

The script generates minimal Reaper project files with:
- Project header with sample rate
- Track blocks with names
- Item blocks with audio sources
- Volume envelope (VOLENV2)
- Pan envelope (PANENV2)
