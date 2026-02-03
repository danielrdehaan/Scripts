// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SaveSessionAs command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SaveSessionAs(const SaveSessionAsRequest& request)
    {
        struct SaveSessionAsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SaveSessionAs);

            std::string GetRequestName() const override
            {
                return "AsyncSaveSessionAs";
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SaveSessionAsRequest& request)
            {
                mGrpcRequestBody.set_session_name(request.sessionName);
                mGrpcRequestBody.set_session_location(request.sessionLocation);
            }

            ptsl::SaveSessionAsRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SaveSessionAs, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
