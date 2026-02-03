// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetEditTool command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetEditToolResponse> CppPTSLClient::GetEditTool(const CommandRequest& request)
    {
        struct GetEditToolHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetEditTool);

            std::string GetRequestName() const override
            {
                return "AsyncGetEditTool";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetEditToolResponse>(mResponse)->currentSetting =
                    static_cast<EditTool>(mGrpcResponseBody.current_setting());
                std::vector<EditTool> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<EditTool>(item));
                }

                std::dynamic_pointer_cast<GetEditToolResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            MAKE_RESP_OVRD(GetEditTool);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetEditToolResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetEditTool, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetEditToolResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
