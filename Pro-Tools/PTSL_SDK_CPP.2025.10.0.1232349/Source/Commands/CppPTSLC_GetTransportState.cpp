// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetTransportState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetTransportStateResponse> CppPTSLClient::GetTransportState(const CommandRequest& request)
    {
        struct GetTransportStateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetTransportState);

            std::string GetRequestName() const override
            {
                return "AsyncGetTransportState";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetTransportStateResponse>(mResponse)->currentSetting =
                    static_cast<TransportState>(mGrpcResponseBody.current_setting());

                std::vector<TransportState> settingsTempList;
                for (const auto& item : mGrpcResponseBody.possible_settings())
                {
                    settingsTempList.push_back(static_cast<TransportState>(item));
                }

                std::dynamic_pointer_cast<GetTransportStateResponse>(mResponse)->possibleSettings = {
                    settingsTempList.begin(), settingsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetTransportStateResponse>(mResponse)->possibleSettings.resize(0);
            }

            MAKE_RESP_OVRD(GetTransportState);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetTransportStateResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetTransportState, false, true);

        return std::dynamic_pointer_cast<GetTransportStateResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
