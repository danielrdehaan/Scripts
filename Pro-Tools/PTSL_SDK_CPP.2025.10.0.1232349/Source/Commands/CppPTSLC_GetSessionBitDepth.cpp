// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionBitDepth command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionBitDepthResponse> CppPTSLClient::GetSessionBitDepth(const CommandRequest& request)
    {
        struct GetSessionBitDepthHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionBitDepth);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionBitDepth";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionBitDepthResponse>(mResponse)->currentSetting =
                    static_cast<BitDepth>(mGrpcResponseBody.current_setting());

                std::vector<BitDepth> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<BitDepth>(item));
                }

                std::dynamic_pointer_cast<GetSessionBitDepthResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetSessionBitDepthResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetSessionBitDepth);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionBitDepthResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionBitDepth, false, true);

        return std::dynamic_pointer_cast<GetSessionBitDepthResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
