// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RecallZoomPreset command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RecallZoomPreset(const RecallZoomPresetRequest& request)
    {
        struct RecallZoomPresetHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RecallZoomPreset);

            std::string GetRequestName() const override
            {
                return "AsyncRecallZoomPreset";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const RecallZoomPresetRequest& request)
            {
                mGrpcRequestBody.set_zoom_preset(request.zoomPreset);
            }

            ptsl::RecallZoomPresetRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(RecallZoomPreset, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
