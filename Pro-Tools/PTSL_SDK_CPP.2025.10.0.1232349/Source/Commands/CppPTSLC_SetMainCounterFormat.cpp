// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetMainCounterFormat command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetMainCounterFormat(const SetMainCounterFormatRequest& request)
    {
        struct SetMainCounterFormatHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetMainCounterFormat);

            std::string GetRequestName() const override
            {
                return "AsyncSetMainCounterFormat";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetMainCounterFormatRequest& request)
            {
                mGrpcRequestBody.set_time_scale(static_cast<ptsl::TrackOffsetOptions>(request.timeScale));
            }

            ptsl::SetMainCounterFormatRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetMainCounterFormat, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
