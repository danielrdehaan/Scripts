// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::PasteSpecial command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::PasteSpecial(const PasteSpecialRequest& request)
    {
        struct PasteSpecialHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(PasteSpecial);

            std::string GetRequestName() const override
            {
                return "AsyncPasteSpecial";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const PasteSpecialRequest& request)
            {
                mGrpcRequestBody.set_paste_special_option(
                    static_cast<ptsl::PasteSpecialOptions>(request.pasteSpecialOption));
            }

            ptsl::PasteSpecialRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(PasteSpecial, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
