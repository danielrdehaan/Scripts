// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionBitDepth command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionBitDepth(const SetSessionBitDepthRequest& request)
    {
        struct SetSessionBitDepthHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionBitDepth);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionBitDepth";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionBitDepthRequest& request)
            {
                mGrpcRequestBody.set_bit_depth(static_cast<ptsl::BitDepth>(request.bitDepth));
            }

            ptsl::SetSessionBitDepthRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionBitDepth, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
