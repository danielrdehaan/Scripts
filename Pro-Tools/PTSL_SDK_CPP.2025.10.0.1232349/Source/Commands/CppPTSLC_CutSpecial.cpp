// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CutSpecial command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CutSpecial(const CutSpecialRequest& request)
    {
        struct CutSpecialHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CutSpecial);

            std::string GetRequestName() const override
            {
                return "AsyncCutSpecial";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const CutSpecialRequest& request)
            {
                mGrpcRequestBody.set_automation_data_option(
                    static_cast<ptsl::AutomationDataOptions>(request.automationDataOption));
            }

            ptsl::CutSpecialRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(CutSpecial, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
