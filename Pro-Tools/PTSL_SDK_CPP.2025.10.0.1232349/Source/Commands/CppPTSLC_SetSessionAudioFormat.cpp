// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionAudioFormat command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionAudioFormat(const SetSessionAudioFormatRequest& request)
    {
        struct SetSessionAudioFormatHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionAudioFormat);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionAudioFormat";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionAudioFormatRequest& request)
            {
                mGrpcRequestBody.set_audio_format(static_cast<ptsl::SessionAudioFormat>(request.audioFormat));
            }

            ptsl::SetSessionAudioFormatRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionAudioFormat, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
