// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ClearSpecial command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ClearSpecial(const ClearSpecialRequest& request)
    {
        struct ClearSpecialHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ClearSpecial);

            std::string GetRequestName() const override
            {
                return "AsyncClearSpecial";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const ClearSpecialRequest& request)
            {
                mGrpcRequestBody.set_automation_data_option(
                    static_cast<ptsl::AutomationDataOptions>(request.automationDataOption));
            }

            ptsl::ClearSpecialRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(ClearSpecial, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
