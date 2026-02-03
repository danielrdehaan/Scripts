#!/usr/bin/env python3
"""
Pro Tools to Reaper Export Script

Queries a running Pro Tools session via PTSL and generates a Reaper .rpp project file
containing track structure, audio clip placements, and volume/pan automation.

Requirements:
- Pro Tools 2025.10+ running with a session open
- Python 3.8+
- grpcio, protobuf

Usage:
    python pt_to_reaper.py --output ~/Desktop/exported.rpp
"""

import argparse
import os
import sys
from pathlib import Path

from ptsl_client import PTSLClient, TrackType
from rpp_writer import RPPWriter


def parse_timeline_location(location: dict, sample_rate: int = 48000) -> int:
    """Parse a TimelineLocation to sample count.

    Args:
        location: TimelineLocation dict with 'location' and 'time_type'
        sample_rate: Session sample rate for time conversions

    Returns:
        Sample position as integer
    """
    loc_str = location.get("location", "0")
    time_type = location.get("time_type", 1)

    try:
        if time_type == 1:  # Samples
            return int(loc_str)
        elif time_type == 8:  # Seconds
            return int(float(loc_str) * sample_rate)
        else:
            # Try to parse as samples by default
            return int(loc_str)
    except (ValueError, TypeError):
        return 0


def parse_media_position(position: dict) -> int:
    """Parse a MediaTimePosition to sample count.

    Args:
        position: MediaTimePosition dict with 'position' and 'time_type'

    Returns:
        Sample position as integer
    """
    pos = position.get("position", 0)
    # MediaTimePosition uses BasicTimeType (1=Samples, 2=Ticks, 3=Frames)
    # We expect samples for audio media
    try:
        return int(pos)
    except (ValueError, TypeError):
        return 0


def main():
    parser = argparse.ArgumentParser(
        description="Export Pro Tools session to Reaper project file"
    )
    parser.add_argument(
        "--output", "-o",
        required=True,
        help="Output path for the .rpp file"
    )
    parser.add_argument(
        "--host",
        default="localhost",
        help="Host where Pro Tools is running (default: localhost)"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )

    args = parser.parse_args()

    output_path = Path(args.output).expanduser().resolve()

    # Ensure output has .rpp extension
    if output_path.suffix.lower() != ".rpp":
        output_path = output_path.with_suffix(".rpp")

    # Ensure output directory exists
    output_path.parent.mkdir(parents=True, exist_ok=True)

    def log(msg: str):
        if args.verbose:
            print(msg)

    print(f"Connecting to Pro Tools at {args.host}...")

    try:
        with PTSLClient(host=args.host) as client:
            # Register connection
            log("Registering connection...")
            session_id = client.register_connection()
            log(f"  Session ID: {session_id}")

            # Get session info
            log("Getting session info...")
            session_name = client.get_session_name()
            session_path = client.get_session_path()
            sample_rate = client.get_session_sample_rate()

            print(f"Session: {session_name}")
            print(f"Path: {session_path}")
            print(f"Sample Rate: {sample_rate}")

            # Get tracks (audio and instrument only)
            log("Getting track list...")
            tracks = client.get_track_list(
                track_types=[TrackType.Audio, TrackType.Instrument]
            )
            print(f"Found {len(tracks)} audio/instrument tracks")

            if not tracks:
                print("No audio tracks found in session.")
                return 1

            # Get file locations
            log("Getting file locations...")
            file_locations = client.get_file_locations()
            file_map = {
                fl.get("file_id"): fl.get("path")
                for fl in file_locations
                if fl.get("file_id") and fl.get("path")
            }
            log(f"  Found {len(file_map)} files")

            # Get all clips
            log("Getting clip list...")
            try:
                clips = client.get_clip_list()
                clip_map = {
                    clip.get("clip_id"): clip
                    for clip in clips
                    if clip.get("clip_id")
                }
                log(f"  Found {len(clip_map)} clips")
            except RuntimeError as e:
                if "Private API" in str(e):
                    print("\nNote: GetClipList requires Private API access.")
                    print("      Contact audiosdk@avid.com for access.")
                    clips = []
                    clip_map = {}
                else:
                    raise

            if not clip_map:
                print("\nWarning: No clip data available.")
                print("         This may require Private API access from Avid.")

            # For each track, get playlist elements and automation
            clips_by_track = {}
            volume_automation = {}
            pan_automation = {}

            # Track warnings to avoid repeating them
            private_api_warned = False
            automation_warned = False

            for i, track in enumerate(tracks):
                track_id = track.get("id")
                track_name = track.get("name", "Untitled")
                log(f"Processing track {i+1}/{len(tracks)}: {track_name}")

                # Get playlists for this track
                try:
                    playlists = client.get_track_playlists(track_id=track_id)
                except Exception as e:
                    log(f"  Warning: Could not get playlists: {e}")
                    playlists = []

                # Find the main/target playlist
                main_playlist = None
                for pl in playlists:
                    if pl.get("is_target", False):
                        main_playlist = pl
                        break
                if not main_playlist and playlists:
                    main_playlist = playlists[0]

                # Get playlist elements (requires Private API)
                track_clips = []
                if main_playlist:
                    playlist_id = main_playlist.get("playlist_id")
                    try:
                        elements = client.get_playlist_elements(
                            playlist_id=playlist_id
                        )
                        log(f"  Found {len(elements)} elements")

                        for elem in elements:
                            # Get timing from PlaylistElement
                            start_time = parse_timeline_location(
                                elem.get("start_time", {}), sample_rate
                            )
                            end_time = parse_timeline_location(
                                elem.get("end_time", {}), sample_rate
                            )

                            # Get clip info from channel_clips
                            channel_clips = elem.get("channel_clips", [])
                            if not channel_clips:
                                continue

                            # Use first non-null channel clip
                            for ch_clip in channel_clips:
                                if ch_clip.get("is_null", False):
                                    continue
                                clip_id = ch_clip.get("clip_id")
                                if not clip_id:
                                    continue

                                # Look up full clip info
                                clip_info = clip_map.get(clip_id, {})
                                file_id = clip_info.get("file_id")

                                if not file_id:
                                    continue

                                # Get source offset
                                src_start = parse_media_position(
                                    clip_info.get("src_start_point", {})
                                )

                                track_clips.append({
                                    "clip_id": clip_id,
                                    "clip_name": clip_info.get("clip_full_name", ""),
                                    "file_id": file_id,
                                    "start_time_samples": start_time,
                                    "end_time_samples": end_time,
                                    "src_start_samples": src_start,
                                })
                                break  # Only use first valid channel

                    except RuntimeError as e:
                        if "Private API" in str(e):
                            if not private_api_warned:
                                print("\nNote: GetPlaylistElements requires Private API access.")
                                print("      Clip positions will not be exported.")
                                print("      Contact audiosdk@avid.com for API access.")
                                private_api_warned = True
                            # Don't log for every track
                        elif args.verbose:
                            log(f"  Warning: {e}")

                clips_by_track[track_id] = track_clips

                # Get volume automation (may not be implemented in all PT versions)
                try:
                    vol_bp = client.get_volume_automation(track_id=track_id)
                    if vol_bp:
                        volume_automation[track_id] = vol_bp
                        log(f"  Volume automation: {len(vol_bp)} points")
                except RuntimeError as e:
                    if ("Not yet implemented" in str(e) or "UnsupportedCommand" in str(e)) and not automation_warned:
                        print("\nNote: GetTrackControlBreakpoints not available in this Pro Tools version.")
                        print("      Automation will not be exported.")
                        automation_warned = True
                    elif "InvalidParameter" not in str(e) and args.verbose:
                        log(f"  Warning: {e}")

                # Get pan automation
                if not automation_warned:
                    try:
                        pan_bp = client.get_pan_automation(track_id=track_id)
                        if pan_bp:
                            pan_automation[track_id] = pan_bp
                            log(f"  Pan automation: {len(pan_bp)} points")
                    except RuntimeError:
                        pass  # Already warned about automation

            # Write RPP file
            print(f"Writing {output_path}...")
            writer = RPPWriter(sample_rate=sample_rate)

            with open(output_path, "w", encoding="utf-8") as f:
                writer.write_project(
                    f,
                    session_name,
                    tracks,
                    clips_by_track,
                    file_map,
                    volume_automation,
                    pan_automation
                )

            print(f"Export complete: {output_path}")
            return 0

    except ConnectionRefusedError:
        print("Error: Could not connect to Pro Tools.")
        print("Make sure Pro Tools is running with a session open.")
        return 1
    except Exception as e:
        print(f"Error: {e}")
        if args.verbose:
            import traceback
            traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
