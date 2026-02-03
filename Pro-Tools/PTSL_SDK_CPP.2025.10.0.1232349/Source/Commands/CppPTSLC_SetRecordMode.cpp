// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetRecordMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<SetRecordModeResponse> CppPTSLClient::SetRecordMode(const SetRecordModeRequest& request)
    {
        struct SetRecordModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetRecordMode);

            std::string GetRequestName() const override
            {
                return "AsyncSetRecordMode";
            }

            void OnHasBody() override
            {
                for (const auto& item : mGrpcResponseBody.record_mode_list())
                {
                    std::dynamic_pointer_cast<SetRecordModeResponse>(mResponse)->recordModeList.push_back(
                        static_cast<RecordMode>(item));
                }

                std::dynamic_pointer_cast<SetRecordModeResponse>(mResponse)->currentRecordMode =
                    static_cast<RecordMode>(mGrpcResponseBody.current_record_mode());
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<SetRecordModeResponse>(mResponse)->recordModeList.resize(0);
            }

            MAKE_RESP_OVRD(SetRecordMode);

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            void FillGrpcRequest(const SetRecordModeRequest& request)
            {
                mGrpcRequestBody.set_record_mode(static_cast<ptsl::RecordMode>(request.recordMode));
                mGrpcRequestBody.set_record_arm_transport(request.recordArmTransport);
            }

            ptsl::SetRecordModeRequestBody mGrpcRequestBody;
            ptsl::SetRecordModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(SetRecordMode, true, true);

        return std::dynamic_pointer_cast<SetRecordModeResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
