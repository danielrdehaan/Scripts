"""
PTSL Client - gRPC wrapper for Pro Tools Scripting Library.

Provides a high-level interface for communicating with Pro Tools via PTSL.
"""

import json
import grpc
from typing import Optional, Any

# These will be generated from PTSL.proto
from proto import PTSL_pb2
from proto import PTSL_pb2_grpc


# Command IDs from PTSL.proto
class CommandId:
    RegisterConnection = 70
    GetSessionName = 42
    GetSessionPath = 43
    GetSessionSampleRate = 35
    GetTrackList = 3
    GetFileLocation = 16
    GetClipList = 125
    GetTrackPlaylists = 154
    GetPlaylistElements = 158
    GetTrackControlBreakpoints = 149


# Track types we care about
class TrackType:
    Audio = 2
    Instrument = 11


# Track section and control types for automation
class TrackSectionId:
    MainOut = 1


class TrackControlType:
    Volume = 1
    Pan = 2


# Timeline location types
class TimelineLocationType:
    Samples = 1
    Seconds = 8


class PTSLClient:
    """Client for communicating with Pro Tools via PTSL gRPC API."""

    PTSL_PORT = 31416
    API_VERSION = 2025
    API_VERSION_MINOR = 10

    def __init__(self, host: str = "localhost"):
        """Initialize the PTSL client.

        Args:
            host: Hostname where Pro Tools is running (default: localhost)
        """
        self.host = host
        self.channel: Optional[grpc.Channel] = None
        self.stub: Optional[PTSL_pb2_grpc.PTSLStub] = None
        self.session_id: Optional[str] = None

    def connect(self) -> None:
        """Establish gRPC connection to Pro Tools."""
        target = f"{self.host}:{self.PTSL_PORT}"
        self.channel = grpc.insecure_channel(target)
        self.stub = PTSL_pb2_grpc.PTSLStub(self.channel)

    def close(self) -> None:
        """Close the gRPC connection."""
        if self.channel:
            self.channel.close()
            self.channel = None
            self.stub = None

    def _make_request(self, command_id: int, body: dict = None) -> dict:
        """Send a request to Pro Tools and return the response body.

        Args:
            command_id: The PTSL command ID
            body: Optional request body as a dictionary

        Returns:
            Response body as a dictionary

        Raises:
            RuntimeError: If the request fails
        """
        if not self.stub:
            raise RuntimeError("Not connected. Call connect() first.")

        # Build request header
        header = PTSL_pb2.RequestHeader(
            command=command_id,
            version=self.API_VERSION,
            version_minor=self.API_VERSION_MINOR,
        )

        # Add session_id for all commands except RegisterConnection
        if self.session_id and command_id != CommandId.RegisterConnection:
            header.session_id = self.session_id

        # Build request
        request = PTSL_pb2.Request(
            header=header,
            request_body_json=json.dumps(body) if body else "{}"
        )

        # Send request
        response = self.stub.SendGrpcRequest(request)

        # Check for errors
        if response.response_error_json:
            error = json.loads(response.response_error_json)
            raise RuntimeError(f"PTSL error: {error}")

        # Parse and return response body
        if response.response_body_json:
            return json.loads(response.response_body_json)
        return {}

    def _paginated_request(self, command_id: int, body: dict = None,
                           list_key: str = None, page_size: int = 100) -> list:
        """Send a paginated request and collect all results.

        Args:
            command_id: The PTSL command ID
            body: Optional request body as a dictionary
            list_key: Key in response containing the list of items
            page_size: Number of items per page

        Returns:
            Complete list of all items across all pages
        """
        if body is None:
            body = {}

        all_items = []
        offset = 0

        while True:
            # Add pagination to request
            paginated_body = body.copy()
            paginated_body["pagination_request"] = {
                "limit": page_size,
                "offset": offset
            }

            response = self._make_request(command_id, paginated_body)

            # Get items from response
            items = response.get(list_key, [])
            all_items.extend(items)

            # Check pagination response
            pagination = response.get("pagination_response", {})
            total = pagination.get("total", len(items))

            offset += len(items)

            # Stop if we have all items or no items returned
            if offset >= total or len(items) == 0:
                break

        return all_items

    def register_connection(self, company_name: str = "PTSLPython",
                           application_name: str = "pt_to_reaper") -> str:
        """Register connection with Pro Tools.

        Args:
            company_name: Company name for registration
            application_name: Application name for registration

        Returns:
            Session ID for subsequent requests
        """
        body = {
            "company_name": company_name,
            "application_name": application_name
        }
        response = self._make_request(CommandId.RegisterConnection, body)
        self.session_id = response.get("session_id")
        return self.session_id

    def get_session_name(self) -> str:
        """Get the name of the current Pro Tools session."""
        response = self._make_request(CommandId.GetSessionName)
        return response.get("session_name", "")

    def get_session_path(self) -> str:
        """Get the path of the current Pro Tools session."""
        response = self._make_request(CommandId.GetSessionPath)
        session_path = response.get("session_path", {})
        return session_path.get("path", "")

    def get_session_sample_rate(self) -> int:
        """Get the sample rate of the current session.

        Returns:
            Sample rate as integer (e.g., 48000)
        """
        response = self._make_request(CommandId.GetSessionSampleRate)
        sample_rate = response.get("sample_rate", "SR_48000")

        # Parse sample rate from enum string (e.g., "SR_48000" -> 48000)
        if isinstance(sample_rate, str):
            # Handle formats: SR_48000, SRate_48000, or just 48000
            rate_str = sample_rate.replace("SR_", "").replace("SRate_", "")
            try:
                return int(rate_str)
            except ValueError:
                return 48000  # Default fallback
        return int(sample_rate)

    def get_track_list(self, track_types: list = None) -> list:
        """Get list of tracks in the session.

        Args:
            track_types: Optional list of track type values to filter by.
                        Can be integers or strings like "TT_Audio", "TType_Audio"

        Returns:
            List of track dictionaries
        """
        all_tracks = self._paginated_request(
            CommandId.GetTrackList,
            {},
            list_key="track_list"
        )

        # Filter client-side by track type if specified
        if track_types:
            # Build set of acceptable type values (both string and int forms)
            type_set = set()
            for t in track_types:
                if isinstance(t, int):
                    type_set.add(t)
                    # Add string equivalents
                    type_map = {
                        2: ["TT_Audio", "AudioTrack", "TType_Audio"],
                        11: ["TT_Instrument", "Instrument", "TType_Instrument"],
                    }
                    type_set.update(type_map.get(t, []))
                else:
                    type_set.add(t)

            return [
                track for track in all_tracks
                if track.get("type") in type_set
            ]

        return all_tracks

    def get_file_locations(self) -> list:
        """Get all file locations in the session.

        Returns:
            List of file location dictionaries with path and file_id
        """
        return self._paginated_request(
            CommandId.GetFileLocation,
            {},
            list_key="file_locations"
        )

    def get_clip_list(self) -> list:
        """Get all clips in the session.

        Returns:
            List of clip dictionaries
        """
        return self._paginated_request(
            CommandId.GetClipList,
            {},
            list_key="clips"
        )

    def get_track_playlists(self, track_id: str = None, track_name: str = None) -> list:
        """Get playlists for a track.

        Args:
            track_id: Track ID (hex string)
            track_name: Track name (alternative to track_id)

        Returns:
            List of playlist dictionaries
        """
        body = {}
        if track_id:
            body["track_id"] = track_id
        elif track_name:
            body["track_name"] = track_name
        else:
            raise ValueError("Either track_id or track_name must be provided")

        return self._paginated_request(
            CommandId.GetTrackPlaylists,
            body,
            list_key="playlists"
        )

    def get_playlist_elements(self, playlist_id: str = None,
                              playlist_name: str = None,
                              time_format: int = TimelineLocationType.Samples) -> list:
        """Get elements (clips) on a playlist.

        Args:
            playlist_id: Playlist ID
            playlist_name: Playlist name (alternative to playlist_id)
            time_format: Time format for returned positions

        Returns:
            List of playlist element dictionaries
        """
        body = {"time_format": time_format}
        if playlist_id:
            body["playlist_id"] = playlist_id
        elif playlist_name:
            body["playlist_name"] = playlist_name
        else:
            raise ValueError("Either playlist_id or playlist_name must be provided")

        return self._paginated_request(
            CommandId.GetPlaylistElements,
            body,
            list_key="elements_list"
        )

    def get_track_automation(self, track_id: str = None, track_name: str = None,
                             control_type: int = TrackControlType.Volume) -> list:
        """Get automation breakpoints for a track control.

        Args:
            track_id: Track ID (hex string)
            track_name: Track name (alternative to track_id)
            control_type: Type of control (Volume, Pan, etc.)

        Returns:
            List of breakpoint dictionaries with time and value
        """
        body = {
            "control_id": {
                "section": TrackSectionId.MainOut,
                "control_type": control_type
            }
        }
        if track_id:
            body["track_id"] = track_id
        elif track_name:
            body["track_name"] = track_name
        else:
            raise ValueError("Either track_id or track_name must be provided")

        response = self._make_request(CommandId.GetTrackControlBreakpoints, body)
        return response.get("breakpoints", [])

    def get_volume_automation(self, track_id: str = None,
                              track_name: str = None) -> list:
        """Get volume automation breakpoints for a track.

        Args:
            track_id: Track ID (hex string)
            track_name: Track name (alternative to track_id)

        Returns:
            List of breakpoint dictionaries
        """
        return self.get_track_automation(
            track_id=track_id,
            track_name=track_name,
            control_type=TrackControlType.Volume
        )

    def get_pan_automation(self, track_id: str = None,
                           track_name: str = None) -> list:
        """Get pan automation breakpoints for a track.

        Args:
            track_id: Track ID (hex string)
            track_name: Track name (alternative to track_id)

        Returns:
            List of breakpoint dictionaries
        """
        return self.get_track_automation(
            track_id=track_id,
            track_name=track_name,
            control_type=TrackControlType.Pan
        )

    def __enter__(self):
        """Context manager entry."""
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit."""
        self.close()
        return False
