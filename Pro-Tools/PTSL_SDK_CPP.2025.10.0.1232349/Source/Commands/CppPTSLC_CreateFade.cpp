// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information
// of Avid. Do not disclose to any third party. Use of the
// information contained in this document is subject to an Avid
// SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CreateFade command.
 *
 * @deprecated CreateFade is deprecated starting in Pro Tools 2023.09.
 * Replaced with @ref PTSLC_CPP::CppPTSLClient::CreateFadesBasedOnPreset "CreateFadesBasedOnPreset"
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CreateFade(const CreateFadesBasedOnPresetRequest& request)
    {
        struct CreateFadesBasedOnPresetHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CreateFadesBasedOnPreset);

            std::string GetRequestName() const override
            {
                return "AsyncCreateFadesBasedOnPreset";
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<CreateFadesBasedOnPresetError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->fadePresetName = mGrpcResponseBody.fade_preset_name();
                    mResponse->errors.push_back(commandError);
                }
            }

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
            void FillGrpcRequest(const CreateFadesBasedOnPresetRequest& request)
            {
                mGrpcRequestBody.set_fade_preset_name(request.fadePresetName);
                mGrpcRequestBody.set_auto_adjust_bounds(request.autoAdjustBounds);
            }

            ptsl::CreateFadesBasedOnPresetRequestBody mGrpcRequestBody;
            ptsl::CreateFadesBasedOnPresetResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(CreateFadesBasedOnPreset, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
