// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetEditMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetEditMode(const SetEditModeRequest& request)
    {
        struct SetEditModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetEditMode);

            std::string GetRequestName() const override
            {
                return "AsyncSetEditMode";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetEditModeRequest& request)
            {
                mGrpcRequestBody.set_edit_mode(static_cast<ptsl::EditMode>(request.editMode));
            }

            ptsl::SetEditModeRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetEditMode, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
