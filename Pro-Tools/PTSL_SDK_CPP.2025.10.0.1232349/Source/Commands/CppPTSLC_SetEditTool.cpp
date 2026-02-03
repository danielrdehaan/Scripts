// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetEditTool command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetEditTool(const SetEditToolRequest& request)
    {
        struct SetEditToolHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetEditTool);

            std::string GetRequestName() const override
            {
                return "AsyncSetEditTool";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetEditToolRequest& request)
            {
                mGrpcRequestBody.set_edit_tool(static_cast<ptsl::EditTool>(request.editTool));
            }

            ptsl::SetEditToolRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetEditTool, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
