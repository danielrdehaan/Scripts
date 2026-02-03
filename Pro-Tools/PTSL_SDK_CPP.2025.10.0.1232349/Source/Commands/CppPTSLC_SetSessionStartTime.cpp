// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionStartTime command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionStartTime(const SetSessionStartTimeRequest& request)
    {
        struct SetSessionStartTimeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionStartTime);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionStartTime";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionStartTimeRequest& request)
            {
                mGrpcRequestBody.set_session_start_time(request.sessionStartTime);
                mGrpcRequestBody.set_track_offset_opts(static_cast<ptsl::TrackOffsetOptions>(request.trackOffsetOpts));
                mGrpcRequestBody.set_maintain_relative_position(static_cast<bool>(request.maintainRelativePosition));
            }

            ptsl::SetSessionStartTimeRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionStartTime, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
