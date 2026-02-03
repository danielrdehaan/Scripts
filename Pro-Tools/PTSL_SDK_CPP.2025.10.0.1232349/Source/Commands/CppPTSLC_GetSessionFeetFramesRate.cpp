// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionFeetFramesRate command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionFeetFramesRateResponse> CppPTSLClient::GetSessionFeetFramesRate(
        const CommandRequest& request)
    {
        struct GetSessionFeetFramesRateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionFeetFramesRate);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionFeetFramesRate";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionFeetFramesRateResponse>(mResponse)->currentSetting =
                    static_cast<SessionFeetFramesRate>(mGrpcResponseBody.current_setting());

                std::vector<SessionFeetFramesRate> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<SessionFeetFramesRate>(item));
                }

                std::dynamic_pointer_cast<GetSessionFeetFramesRateResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetSessionFeetFramesRateResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetSessionFeetFramesRate);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionFeetFramesRateResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionFeetFramesRate, false, true);

        return std::dynamic_pointer_cast<GetSessionFeetFramesRateResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
