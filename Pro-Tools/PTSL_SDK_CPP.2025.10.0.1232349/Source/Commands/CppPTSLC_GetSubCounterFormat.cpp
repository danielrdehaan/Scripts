// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSubCounterFormat command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSubCounterFormatResponse> CppPTSLClient::GetSubCounterFormat(const CommandRequest& request)
    {
        struct GetSubCounterFormatHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSubCounterFormat);

            std::string GetRequestName() const override
            {
                return "AsyncGetSubCounterFormat";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSubCounterFormatResponse>(mResponse)->currentSetting =
                    static_cast<TrackOffsetOptions>(mGrpcResponseBody.current_setting());
                std::vector<TrackOffsetOptions> possibleSettingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    possibleSettingsTempList.push_back(static_cast<TrackOffsetOptions>(item));
                }

                std::dynamic_pointer_cast<GetSubCounterFormatResponse>(mResponse)->possibleSettings = {
                    possibleSettingsTempList.begin(), possibleSettingsTempList.end()
                };
            }

            MAKE_RESP_OVRD(GetSubCounterFormat);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSubCounterFormatResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSubCounterFormat, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetSubCounterFormatResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
