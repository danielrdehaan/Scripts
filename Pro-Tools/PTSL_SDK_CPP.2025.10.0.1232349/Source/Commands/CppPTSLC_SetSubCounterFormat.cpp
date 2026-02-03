// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSubCounterFormat command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSubCounterFormat(const SetSubCounterFormatRequest& request)
    {
        struct SetSubCounterFormatHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSubCounterFormat);

            std::string GetRequestName() const override
            {
                return "AsyncSetSubCounterFormat";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSubCounterFormatRequest& request)
            {
                mGrpcRequestBody.set_time_scale(static_cast<ptsl::TrackOffsetOptions>(request.timeScale));
            }

            ptsl::SetSubCounterFormatRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSubCounterFormat, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
