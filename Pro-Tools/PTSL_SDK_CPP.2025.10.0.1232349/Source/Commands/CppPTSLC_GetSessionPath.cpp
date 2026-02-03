// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionPath command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionPathResponse> CppPTSLClient::GetSessionPath(const CommandRequest& request)
    {
        struct GetSessionPathHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionPath);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionPath";
            }

            void OnHasBody() override
            {
                FileLocation fl;
                fl.info = { mGrpcResponseBody.session_path().info().is_online() };
                fl.path = mGrpcResponseBody.session_path().path();

                std::dynamic_pointer_cast<GetSessionPathResponse>(mResponse)->sessionPath = fl;
            }

            MAKE_RESP_OVRD(GetSessionPath);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionPathResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionPath, false, true);

        return std::dynamic_pointer_cast<GetSessionPathResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
