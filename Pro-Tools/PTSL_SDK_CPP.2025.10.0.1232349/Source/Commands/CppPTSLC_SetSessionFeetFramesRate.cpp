// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionFeetFramesRate command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionFeetFramesRate(
        const SetSessionFeetFramesRateRequest& request)
    {
        struct SetSessionFeetFramesRateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionFeetFramesRate);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionFeetFramesRate";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionFeetFramesRateRequest& request)
            {
                mGrpcRequestBody.set_feet_frames_rate(static_cast<ptsl::SessionFeetFramesRate>(request.feetFramesRate));
            }

            ptsl::SetSessionFeetFramesRateRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionFeetFramesRate, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
