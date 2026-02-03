// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionTimeCodeRate command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionTimeCodeRateResponse> CppPTSLClient::GetSessionTimeCodeRate(const CommandRequest& request)
    {
        struct GetSessionTimeCodeRateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionTimeCodeRate);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionTimeCodeRate";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionTimeCodeRateResponse>(mResponse)->currentSetting =
                    static_cast<SessionTimeCodeRate>(mGrpcResponseBody.current_setting());

                std::vector<SessionTimeCodeRate> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<SessionTimeCodeRate>(item));
                }

                std::dynamic_pointer_cast<GetSessionTimeCodeRateResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetSessionTimeCodeRateResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetSessionTimeCodeRate);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionTimeCodeRateResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionTimeCodeRate, false, true);

        return std::dynamic_pointer_cast<GetSessionTimeCodeRateResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
