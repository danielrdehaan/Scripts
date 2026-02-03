// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetTrackOnlineState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetTrackOnlineState(const SetTrackOnlineStateRequest& request)
    {
        struct SetTrackOnlineStateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetTrackOnlineState);

            std::string GetRequestName() const override
            {
                return "AsyncSetTrackOnlineState";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetTrackOnlineStateRequest& request)
            {
                mGrpcRequestBody.set_track_name(request.trackName);
                mGrpcRequestBody.set_enabled(request.enabled);
            }

            ptsl::SetTrackOnlineStateRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetTrackOnlineState, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
