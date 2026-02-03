// Copyright 2022-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetTransportArmed command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetTransportArmedResponse> CppPTSLClient::GetTransportArmed(const CommandRequest& request)
    {
        struct GetTransportArmedHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetTransportArmed);

            std::string GetRequestName() const override
            {
                return "AsyncGetTransportArmed";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetTransportArmedResponse>(mResponse)->isTransportArmed =
                    mGrpcResponseBody.is_transport_armed();
            }

            MAKE_RESP_OVRD(GetTransportArmed);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetTransportArmedResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetTransportArmed, false, true);

        return std::dynamic_pointer_cast<GetTransportArmedResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
