// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SelectMemoryLocation command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SelectMemoryLocation(const SelectMemoryLocationRequest& request)
    {
        struct SelectMemoryLocationHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SelectMemoryLocation);

            std::string GetRequestName() const override
            {
                return "AsyncSelectMemoryLocation";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SelectMemoryLocationRequest& request)
            {
                mGrpcRequestBody.set_number(static_cast<int32_t>(request.number));
            }

            ptsl::SelectMemoryLocationRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SelectMemoryLocation, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
