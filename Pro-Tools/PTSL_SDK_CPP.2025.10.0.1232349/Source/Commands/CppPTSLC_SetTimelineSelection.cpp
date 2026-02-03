// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information
// of Avid. Do not disclose to any third party. Use of the
// information contained in this document is subject to an Avid
// SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetTimelineSelection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetTimelineSelection(const SetTimelineSelectionRequest& request)
    {
        struct SetTimelineSelectionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetTimelineSelection);

            std::string GetRequestName() const override
            {
                return "AsyncSetTimelineSelection";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetTimelineSelectionRequest& request)
            {
                mGrpcRequestBody.set_play_start_marker_time(request.playStartMarkerTime);
                mGrpcRequestBody.set_in_time(request.inTime);
                mGrpcRequestBody.set_out_time(request.outTime);
                mGrpcRequestBody.set_pre_roll_start_time(request.preRollStartTime);
                mGrpcRequestBody.set_post_roll_stop_time(request.postRollStopTime);
                mGrpcRequestBody.set_pre_roll_enabled(static_cast<ptsl::TripleBool>(request.preRollEnabled));
                mGrpcRequestBody.set_post_roll_enabled(static_cast<ptsl::TripleBool>(request.postRollEnabled));
                mGrpcRequestBody.set_update_video_to(static_cast<ptsl::TimelineUpdateVideo>(request.updateVideoTo));
                mGrpcRequestBody.set_propagate_to_satellites(
                    static_cast<ptsl::TripleBool>(request.propagateToSatellites));
            }

            ptsl::SetTimelineSelectionRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetTimelineSelection, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);

        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
