// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetMemoryLocationsManageMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetMemoryLocationsManageModeResponse> CppPTSLClient::GetMemoryLocationsManageMode(
        const CommandRequest& request)
    {
        struct GetMemoryLocationsManageModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetMemoryLocationsManageMode);

            std::string GetRequestName() const override
            {
                return "AsyncGetMemoryLocationsManageMode";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetMemoryLocationsManageModeResponse>(mResponse)->enabled =
                    mGrpcResponseBody.enabled();
            }

            MAKE_RESP_OVRD(GetMemoryLocationsManageMode);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetMemoryLocationsManageModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetMemoryLocationsManageMode, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetMemoryLocationsManageModeResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
