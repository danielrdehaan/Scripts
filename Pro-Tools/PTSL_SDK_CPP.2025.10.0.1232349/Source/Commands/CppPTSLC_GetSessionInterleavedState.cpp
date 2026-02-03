// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionInterleavedState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionInterleavedStateResponse> CppPTSLClient::GetSessionInterleavedState(
        const CommandRequest& request)
    {
        struct GetSessionInterleavedStateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionInterleavedState);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionInterleavedState";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionInterleavedStateResponse>(mResponse)->currentSetting =
                    static_cast<bool>(mGrpcResponseBody.current_setting());

                std::vector<bool> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<bool>(item));
                }

                std::dynamic_pointer_cast<GetSessionInterleavedStateResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetSessionInterleavedStateResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetSessionInterleavedState);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionInterleavedStateResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionInterleavedState, false, true);

        return std::dynamic_pointer_cast<GetSessionInterleavedStateResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
