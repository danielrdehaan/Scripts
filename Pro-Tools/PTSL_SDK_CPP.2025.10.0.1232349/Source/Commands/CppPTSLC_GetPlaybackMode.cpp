// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetPlaybackMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetPlaybackModeResponse> CppPTSLClient::GetPlaybackMode(const CommandRequest& request)
    {
        struct GetPlaybackModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetPlaybackMode);

            std::string GetRequestName() const override
            {
                return "AsyncGetPlaybackMode";
            }

            void OnHasBody() override
            {
                std::vector<PlaybackMode> settingsTempList;
                for (const auto& item : mGrpcResponseBody.current_settings())
                {
                    settingsTempList.push_back(static_cast<PlaybackMode>(item));
                }

                std::dynamic_pointer_cast<GetPlaybackModeResponse>(mResponse)->currentSettings = {
                    settingsTempList.begin(), settingsTempList.end()
                };

                settingsTempList.clear();
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    settingsTempList.push_back(static_cast<PlaybackMode>(item));
                }

                std::dynamic_pointer_cast<GetPlaybackModeResponse>(mResponse)->possibleSettings = {
                    settingsTempList.begin(), settingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetPlaybackModeResponse>(mResponse)->possibleSettings.resize(0);
                std::dynamic_pointer_cast<GetPlaybackModeResponse>(mResponse)->currentSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetPlaybackMode);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetPlaybackModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetPlaybackMode, false, true);

        return std::dynamic_pointer_cast<GetPlaybackModeResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
