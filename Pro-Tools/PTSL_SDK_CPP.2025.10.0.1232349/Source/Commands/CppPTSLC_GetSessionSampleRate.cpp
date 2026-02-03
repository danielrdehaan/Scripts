// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionSampleRate command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionSampleRateResponse> CppPTSLClient::GetSessionSampleRate(const CommandRequest& request)
    {
        struct GetSessionSampleRateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionSampleRate);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionSampleRate";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionSampleRateResponse>(mResponse)->sampleRate =
                    static_cast<SampleRate>(mGrpcResponseBody.sample_rate());
            }

            MAKE_RESP_OVRD(GetSessionSampleRate);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionSampleRateResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionSampleRate, false, true);

        return std::dynamic_pointer_cast<GetSessionSampleRateResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
