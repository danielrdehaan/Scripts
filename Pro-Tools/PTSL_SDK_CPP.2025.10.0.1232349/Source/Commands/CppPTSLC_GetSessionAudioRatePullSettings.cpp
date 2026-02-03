// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionAudioRatePullSettings command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionAudioRatePullSettingsResponse> CppPTSLClient::GetSessionAudioRatePullSettings(
        const CommandRequest& request)
    {
        struct GetSessionAudioRatePullSettingsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionAudioRatePullSettings);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionAudioRatePullSettings";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionAudioRatePullSettingsResponse>(mResponse)->currentSetting =
                    static_cast<SessionRatePull>(mGrpcResponseBody.current_setting());

                std::vector<SessionRatePull> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<SessionRatePull>(item));
                }

                std::dynamic_pointer_cast<GetSessionAudioRatePullSettingsResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetSessionAudioRatePullSettingsResponse>(mResponse)->possibleSettings.resize(
                    0);
            }

            MAKE_RESP_OVRD(GetSessionAudioRatePullSettings);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionAudioRatePullSettingsResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionAudioRatePullSettings, false, true);

        return std::dynamic_pointer_cast<GetSessionAudioRatePullSettingsResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
