"""
RPP Writer - Generates Reaper project files from Pro Tools session data.

Creates minimal .rpp files with tracks, items, audio sources, and automation.
"""

import math
from typing import TextIO


def pt_volume_to_reaper(pt_value: float) -> float:
    """Convert Pro Tools volume value to Reaper volume.

    Pro Tools: -1.0 to 1.0 (represents -inf to +12dB approximately)
    Reaper: 0.0 to ~4.0 (linear scale where 1.0 = 0dB)

    The PT range maps roughly as:
    - PT -1.0 = -inf dB (Reaper 0.0)
    - PT 0.0 = 0 dB (Reaper 1.0)
    - PT 1.0 = +12 dB (Reaper ~4.0)
    """
    if pt_value <= -1.0:
        return 0.0

    # Map PT 0 to Reaper 1.0, PT 1.0 to ~4.0 (12dB)
    # Using dB = pt_value * 12, then convert to linear
    db = pt_value * 12.0
    return 10.0 ** (db / 20.0)


def pt_pan_to_reaper(pt_value: float) -> float:
    """Convert Pro Tools pan value to Reaper pan.

    Both use -1.0 to 1.0 range, so direct mapping.
    """
    return max(-1.0, min(1.0, pt_value))


def samples_to_seconds(samples: int, sample_rate: int) -> float:
    """Convert sample count to seconds."""
    if sample_rate <= 0:
        return 0.0
    return samples / sample_rate


class RPPWriter:
    """Writes Reaper project files (.rpp)."""

    def __init__(self, sample_rate: int = 48000):
        """Initialize the RPP writer.

        Args:
            sample_rate: Session sample rate
        """
        self.sample_rate = sample_rate
        self._indent_level = 0

    def _write_line(self, f: TextIO, line: str) -> None:
        """Write a line with proper indentation."""
        indent = "  " * self._indent_level
        f.write(f"{indent}{line}\n")

    def _begin_block(self, f: TextIO, block_name: str, *args) -> None:
        """Begin an RPP block with optional arguments."""
        args_str = " ".join(str(a) for a in args) if args else ""
        if args_str:
            self._write_line(f, f"<{block_name} {args_str}")
        else:
            self._write_line(f, f"<{block_name}")
        self._indent_level += 1

    def _end_block(self, f: TextIO) -> None:
        """End an RPP block."""
        self._indent_level -= 1
        self._write_line(f, ">")

    def write_project(self, f: TextIO, session_name: str, tracks: list,
                      clips_by_track: dict, file_map: dict,
                      volume_automation: dict, pan_automation: dict) -> None:
        """Write a complete RPP project file.

        Args:
            f: File handle to write to
            session_name: Name of the session
            tracks: List of track dictionaries from PTSL
            clips_by_track: Dict mapping track_id to list of clip placements
            file_map: Dict mapping file_id to file path
            volume_automation: Dict mapping track_id to volume breakpoints
            pan_automation: Dict mapping track_id to pan breakpoints
        """
        self._begin_block(f, "REAPER_PROJECT", "0.1", '"7.0"')

        # Project settings
        self._write_line(f, f"SAMPLERATE {self.sample_rate} 0 0")
        self._write_line(f, f"TEMPO 120 4 4")

        # Write each track
        for track in tracks:
            self._write_track(
                f,
                track,
                clips_by_track.get(track.get("id"), []),
                file_map,
                volume_automation.get(track.get("id"), []),
                pan_automation.get(track.get("id"), [])
            )

        self._end_block(f)

    def _write_track(self, f: TextIO, track: dict, clips: list,
                     file_map: dict, volume_points: list,
                     pan_points: list) -> None:
        """Write a single track with items and automation."""
        self._begin_block(f, "TRACK")

        track_name = track.get("name", "Untitled")
        self._write_line(f, f'NAME "{track_name}"')
        self._write_line(f, "VOLPAN 1.0 0.0 -1.0 -1.0 1.0")
        self._write_line(f, "MUTESOLO 0 0 0")
        self._write_line(f, "IPHASE 0")
        self._write_line(f, "ISBUS 0 0")
        self._write_line(f, "BUSCOMP 0 0 0 0 0")
        self._write_line(f, "SHOWINMIX 1 0.6667 0.5 1 0.5 0 0 0")
        self._write_line(f, "FREEMODE 0")
        self._write_line(f, "REC 0 0 1 0 0 0 0 0")
        self._write_line(f, "VU 2")
        self._write_line(f, "TRACKHEIGHT 0 0 0 0 0 0 0")
        self._write_line(f, "INQ 0 0 0 0.5 100 0 0 100")
        self._write_line(f, "NCHAN 2")
        self._write_line(f, "FX 1")
        self._write_line(f, "TRACKID {" + track.get("id", "00000000") + "}")
        self._write_line(f, "PERF 0")
        self._write_line(f, "MIDIOUT -1")
        self._write_line(f, "MAINSEND 1 0")

        # Write items (clips)
        for clip_data in clips:
            self._write_item(f, clip_data, file_map)

        # Write volume automation if present
        if volume_points and len(volume_points) > 1:
            self._write_volume_envelope(f, volume_points)

        # Write pan automation if present
        if pan_points and len(pan_points) > 1:
            self._write_pan_envelope(f, pan_points)

        self._end_block(f)

    def _write_item(self, f: TextIO, clip_data: dict, file_map: dict) -> None:
        """Write a single media item."""
        # Get timing info
        start_time = clip_data.get("start_time_samples", 0)
        end_time = clip_data.get("end_time_samples", 0)
        src_start = clip_data.get("src_start_samples", 0)

        position_sec = samples_to_seconds(start_time, self.sample_rate)
        length_sec = samples_to_seconds(end_time - start_time, self.sample_rate)
        soffs_sec = samples_to_seconds(src_start, self.sample_rate)

        # Skip invalid items
        if length_sec <= 0:
            return

        # Get file path
        file_id = clip_data.get("file_id", "")
        file_path = file_map.get(file_id, "")

        if not file_path:
            return

        self._begin_block(f, "ITEM")
        self._write_line(f, f"POSITION {position_sec:.6f}")
        self._write_line(f, "SNAPOFFS 0")
        self._write_line(f, f"LENGTH {length_sec:.6f}")
        self._write_line(f, "LOOP 1")
        self._write_line(f, "ALLTAKES 0")
        self._write_line(f, "FADEIN 1 0 0 1 0 0 0")
        self._write_line(f, "FADEOUT 1 0 0 1 0 0 0")
        self._write_line(f, "MUTE 0 0")
        self._write_line(f, "SEL 0")
        self._write_line(f, "IGUID {" + clip_data.get("clip_id", "00000000") + "}")
        self._write_line(f, "IID 1")

        clip_name = clip_data.get("clip_name", "")
        if clip_name:
            self._write_line(f, f'NAME "{clip_name}"')

        self._write_line(f, "VOLPAN 1 0 1 -1")
        self._write_line(f, f"SOFFS {soffs_sec:.6f}")
        self._write_line(f, "PLAYRATE 1 1 0 -1 0 0.0025")
        self._write_line(f, "CHANMODE 0")
        self._write_line(f, "GUID {" + clip_data.get("clip_id", "00000001") + "}")

        # Write source
        self._begin_block(f, "SOURCE", "WAVE")
        self._write_line(f, f'FILE "{file_path}"')
        self._end_block(f)

        self._end_block(f)

    def _write_volume_envelope(self, f: TextIO, breakpoints: list) -> None:
        """Write volume automation envelope."""
        self._begin_block(f, "VOLENV2")
        self._write_line(f, "ACT 1 -1")
        self._write_line(f, "VIS 1 1 1")
        self._write_line(f, "LANEHEIGHT 0 0")
        self._write_line(f, "ARM 0")
        self._write_line(f, "DEFSHAPE 0 -1 -1")

        for bp in breakpoints:
            time_info = bp.get("time", {})
            time_str = time_info.get("location", "0")
            time_type = time_info.get("time_type", 1)  # 1 = Samples

            # Parse time value
            try:
                if time_type == 1:  # Samples
                    time_samples = int(time_str)
                    time_sec = samples_to_seconds(time_samples, self.sample_rate)
                elif time_type == 8:  # Seconds
                    time_sec = float(time_str)
                else:
                    time_sec = float(time_str) if time_str else 0.0
            except (ValueError, TypeError):
                time_sec = 0.0

            pt_value = bp.get("value", 0.0)
            reaper_value = pt_volume_to_reaper(pt_value)

            self._write_line(f, f"PT {time_sec:.6f} {reaper_value:.6f} 0")

        self._end_block(f)

    def _write_pan_envelope(self, f: TextIO, breakpoints: list) -> None:
        """Write pan automation envelope."""
        self._begin_block(f, "PANENV2")
        self._write_line(f, "ACT 1 -1")
        self._write_line(f, "VIS 1 1 1")
        self._write_line(f, "LANEHEIGHT 0 0")
        self._write_line(f, "ARM 0")
        self._write_line(f, "DEFSHAPE 0 -1 -1")

        for bp in breakpoints:
            time_info = bp.get("time", {})
            time_str = time_info.get("location", "0")
            time_type = time_info.get("time_type", 1)  # 1 = Samples

            # Parse time value
            try:
                if time_type == 1:  # Samples
                    time_samples = int(time_str)
                    time_sec = samples_to_seconds(time_samples, self.sample_rate)
                elif time_type == 8:  # Seconds
                    time_sec = float(time_str)
                else:
                    time_sec = float(time_str) if time_str else 0.0
            except (ValueError, TypeError):
                time_sec = 0.0

            pt_value = bp.get("value", 0.0)
            reaper_value = pt_pan_to_reaper(pt_value)

            self._write_line(f, f"PT {time_sec:.6f} {reaper_value:.6f} 0")

        self._end_block(f)
