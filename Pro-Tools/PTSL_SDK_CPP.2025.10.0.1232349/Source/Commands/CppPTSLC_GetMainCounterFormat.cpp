// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetMainCounterFormat command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetMainCounterFormatResponse> CppPTSLClient::GetMainCounterFormat(const CommandRequest& request)
    {
        struct GetMainCounterFormatHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetMainCounterFormat);

            std::string GetRequestName() const override
            {
                return "AsyncGetMainCounterFormat";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetMainCounterFormatResponse>(mResponse)->currentSetting =
                    static_cast<TrackOffsetOptions>(mGrpcResponseBody.current_setting());
                std::vector<TrackOffsetOptions> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<TrackOffsetOptions>(item));
                }

                std::dynamic_pointer_cast<GetMainCounterFormatResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            MAKE_RESP_OVRD(GetMainCounterFormat);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetMainCounterFormatResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetMainCounterFormat, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetMainCounterFormatResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
