// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetEditModeOptions command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetEditModeOptions(const SetEditModeOptionsRequest& request)
    {
        struct SetEditModeOptionsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetEditModeOptions);

            std::string GetRequestName() const override
            {
                return "AsyncSetEditModeOptions";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetEditModeOptionsRequest& request)
            {
                mGrpcRequestBody.mutable_edit_mode_options()->set_tab_to_transients(
                    request.editModeOptions.tabToTransients);
                mGrpcRequestBody.mutable_edit_mode_options()->set_link_timeline_and_edit_selection(
                    request.editModeOptions.linkTimelineAndEditSelection);
                mGrpcRequestBody.mutable_edit_mode_options()->set_link_track_and_edit_selection(
                    request.editModeOptions.linkTrackAndEditSelection);
                mGrpcRequestBody.mutable_edit_mode_options()->set_insertion_follows_playback(
                    request.editModeOptions.insertionFollowsPlayback);
                mGrpcRequestBody.mutable_edit_mode_options()->set_automation_follows_edit(
                    request.editModeOptions.automationFollowsEdit);
                mGrpcRequestBody.mutable_edit_mode_options()->set_markers_follow_edit(
                    request.editModeOptions.markersFollowEdit);
                mGrpcRequestBody.mutable_edit_mode_options()->set_mirrored_midi_editing(
                    request.editModeOptions.mirroredMidiEditing);
                mGrpcRequestBody.mutable_edit_mode_options()->set_layered_editing(
                    request.editModeOptions.layeredEditing);
            }

            ptsl::SetEditModeOptionsRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetEditModeOptions, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
