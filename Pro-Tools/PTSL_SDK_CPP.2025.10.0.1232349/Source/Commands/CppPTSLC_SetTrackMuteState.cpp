// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetTrackMuteState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetTrackMuteState(const SetTrackMuteStateRequest& request)
    {
        struct SetTrackMuteStateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetTrackMuteState);

            std::string GetRequestName() const override
            {
                return "AsyncSetTrackMuteState";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetTrackMuteStateRequest& request)
            {
                *mGrpcRequestBody.mutable_track_names() = { request.trackNames.begin(), request.trackNames.end() };
                mGrpcRequestBody.set_enabled(request.enabled);
            }

            ptsl::SetTrackMuteStateRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetTrackMuteState, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
