// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CopySpecial command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CopySpecial(const CopySpecialRequest& request)
    {
        struct CopySpecialHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CopySpecial);

            std::string GetRequestName() const override
            {
                return "AsyncCopySpecial";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const CopySpecialRequest& request)
            {
                mGrpcRequestBody.set_automation_data_option(
                    static_cast<ptsl::AutomationDataOptions>(request.automationDataOption));
            }

            ptsl::CopySpecialRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(CopySpecial, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
