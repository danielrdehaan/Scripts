// Copyright 2023-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetTimelineSelection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetTimelineSelectionResponse> CppPTSLClient::GetTimelineSelection(
        const GetTimelineSelectionRequest& request)
    {
        struct GetTimelineSelectionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetTimelineSelection);

            std::string GetRequestName() const override
            {
                return "AsyncGetTimelineSelection";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->playStartMarkerTime =
                    mGrpcResponseBody.play_start_marker_time();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->inTime =
                    mGrpcResponseBody.in_time();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->outTime =
                    mGrpcResponseBody.out_time();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->preRollStartTime =
                    mGrpcResponseBody.pre_roll_start_time();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->postRollStopTime =
                    mGrpcResponseBody.post_roll_stop_time();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->preRollEnabled =
                    mGrpcResponseBody.pre_roll_enabled();
                std::dynamic_pointer_cast<GetTimelineSelectionResponse>(mResponse)->postRollEnabled =
                    mGrpcResponseBody.post_roll_enabled();
            }

            MAKE_RESP_OVRD(GetTimelineSelection);

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            void FillGrpcRequest(const GetTimelineSelectionRequest& request)
            {
                mGrpcRequestBody.set_time_scale(static_cast<ptsl::TrackOffsetOptions>(request.timeScale));
            }

            ptsl::GetTimelineSelectionRequestBody mGrpcRequestBody;
            ptsl::GetTimelineSelectionResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetTimelineSelection, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetTimelineSelectionResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
