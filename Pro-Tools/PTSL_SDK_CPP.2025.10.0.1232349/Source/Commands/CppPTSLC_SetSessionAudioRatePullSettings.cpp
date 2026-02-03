// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionAudioRatePullSettings command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionAudioRatePullSettings(
        const SetSessionAudioRatePullSettingsRequest& request)
    {
        struct SetSessionAudioRatePullSettingsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionAudioRatePullSettings);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionAudioRatePullSettings";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionAudioRatePullSettingsRequest& request)
            {
                mGrpcRequestBody.set_audio_rate_pull(static_cast<ptsl::SessionRatePull>(request.audioRatePull));
            }

            ptsl::SetSessionAudioRatePullSettingsRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionAudioRatePullSettings, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
