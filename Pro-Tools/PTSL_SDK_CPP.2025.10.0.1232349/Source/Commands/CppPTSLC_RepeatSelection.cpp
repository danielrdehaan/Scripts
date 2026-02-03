// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RepeatSelection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RepeatSelection(const RepeatSelectionRequest& request)
    {
        struct RepeatSelectionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RepeatSelection);

            std::string GetRequestName() const override
            {
                return "AsyncRepeatSelection";
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
            void FillGrpcRequest(const RepeatSelectionRequest& request)
            {
                mGrpcRequestBody.set_num_repeats(request.numRepeats);
            }

            ptsl::RepeatSelectionRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(RepeatSelection, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
