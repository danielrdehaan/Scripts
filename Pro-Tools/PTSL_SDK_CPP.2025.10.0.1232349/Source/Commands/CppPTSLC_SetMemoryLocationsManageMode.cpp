// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetMemoryLocationsManageMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetMemoryLocationsManageMode(
        const SetMemoryLocationsManageModeRequest& request)
    {
        struct SetMemoryLocationsManageModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetMemoryLocationsManageMode);

            std::string GetRequestName() const override
            {
                return "AsyncSetMemoryLocationsManageMode";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetMemoryLocationsManageModeRequest& request)
            {
                mGrpcRequestBody.set_enabled(request.enabled);
            }

            ptsl::SetMemoryLocationsManageModeRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetMemoryLocationsManageMode, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
