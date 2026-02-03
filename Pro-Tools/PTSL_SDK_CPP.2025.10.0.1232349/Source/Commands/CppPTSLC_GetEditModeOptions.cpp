// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetEditModeOptions command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetEditModeOptionsResponse> CppPTSLClient::GetEditModeOptions(const CommandRequest& request)
    {
        struct GetEditModeOptionsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetEditModeOptions);

            std::string GetRequestName() const override
            {
                return "AsyncGetEditModeOptions";
            }

            void OnHasBody() override
            {
                auto response = std::dynamic_pointer_cast<GetEditModeOptionsResponse>(mResponse);
                EditModeOptions& editModeOptions = response->editModeOptions;
                auto grpcEditModeOptions = mGrpcResponseBody.mutable_edit_mode_options();

                editModeOptions.tabToTransients = grpcEditModeOptions->tab_to_transients();
                editModeOptions.linkTimelineAndEditSelection = grpcEditModeOptions->link_timeline_and_edit_selection();
                editModeOptions.linkTrackAndEditSelection = grpcEditModeOptions->link_track_and_edit_selection();
                editModeOptions.insertionFollowsPlayback = grpcEditModeOptions->insertion_follows_playback();
                editModeOptions.automationFollowsEdit = grpcEditModeOptions->automation_follows_edit();
                editModeOptions.markersFollowEdit = grpcEditModeOptions->markers_follow_edit();
                editModeOptions.mirroredMidiEditing = grpcEditModeOptions->mirrored_midi_editing();
                editModeOptions.layeredEditing = grpcEditModeOptions->layered_editing();
            }

            MAKE_RESP_OVRD(GetEditModeOptions);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetEditModeOptionsResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetEditModeOptions, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetEditModeOptionsResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
