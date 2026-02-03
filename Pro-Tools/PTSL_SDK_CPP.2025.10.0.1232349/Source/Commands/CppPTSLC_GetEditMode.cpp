// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetEditMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetEditModeResponse> CppPTSLClient::GetEditMode(const CommandRequest& request)
    {
        struct GetEditModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetEditMode);

            std::string GetRequestName() const override
            {
                return "AsyncGetEditMode";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetEditModeResponse>(mResponse)->currentSetting =
                    static_cast<EditMode>(mGrpcResponseBody.current_setting());
                std::vector<EditMode> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<EditMode>(item));
                }

                std::dynamic_pointer_cast<GetEditModeResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            MAKE_RESP_OVRD(GetEditMode);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetEditModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetEditMode, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetEditModeResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
