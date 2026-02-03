// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetRecordMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetRecordModeResponse> CppPTSLClient::GetRecordMode(const CommandRequest& request)
    {
        struct GetRecordModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetRecordMode);

            std::string GetRequestName() const override
            {
                return "AsyncGetRecordMode";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetRecordModeResponse>(mResponse)->currentSetting =
                    static_cast<RecordMode>(mGrpcResponseBody.current_setting());

                std::vector<RecordMode> settingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    settingsTempList.push_back(static_cast<RecordMode>(item));
                }

                std::dynamic_pointer_cast<GetRecordModeResponse>(mResponse)->possibleSettings = {
                    settingsTempList.begin(), settingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetRecordModeResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetRecordMode);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetRecordModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetRecordMode, false, true);

        return std::dynamic_pointer_cast<GetRecordModeResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
