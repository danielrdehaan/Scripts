// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionIDs command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionIDsResponse> CppPTSLClient::GetSessionIDs(const CommandRequest& request)
    {
        struct GetSessionIDsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionIDs);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionIDs";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionIDsResponse>(mResponse)->originId = mGrpcResponseBody.origin_id();
                std::dynamic_pointer_cast<GetSessionIDsResponse>(mResponse)->instanceId =
                    mGrpcResponseBody.instance_id();
                std::dynamic_pointer_cast<GetSessionIDsResponse>(mResponse)->parentId = mGrpcResponseBody.parent_id();
            }

            MAKE_RESP_OVRD(GetSessionIDs);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionIDsResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionIDs, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetSessionIDsResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
