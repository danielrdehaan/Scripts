// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetTrackInactiveState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetTrackInactiveState(const SetTrackInactiveStateRequest& request)
    {
        struct SetTrackInactiveStateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetTrackInactiveState);

            std::string GetRequestName() const override
            {
                return "AsyncSetTrackInactiveState";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetTrackInactiveStateRequest& request)
            {
                *mGrpcRequestBody.mutable_track_names() = { request.trackNames.begin(), request.trackNames.end() };
                mGrpcRequestBody.set_enabled(request.enabled);
            }

            ptsl::SetTrackInactiveStateRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetTrackInactiveState, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
