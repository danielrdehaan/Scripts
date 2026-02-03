// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ExtendSelectionToTargetTracks command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ExtendSelectionToTargetTracks(
        const ExtendSelectionToTargetTracksRequest& request)
    {
        struct ExtendSelectionToTargetTracksHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ExtendSelectionToTargetTracks);

            std::string GetRequestName() const override
            {
                return "AsyncExtendSelectionToTargetTracks";
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<ExtendSelectionToTargetTracksError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->tracksToExtendTo = { mGrpcResponseBody.tracks_to_extend_to().begin(),
                        mGrpcResponseBody.tracks_to_extend_to().end() };
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
            void FillGrpcRequest(const ExtendSelectionToTargetTracksRequest& request)
            {
                *mGrpcRequestBody.mutable_tracks_to_extend_to() = { request.tracksToExtendTo.begin(),
                    request.tracksToExtendTo.end() };
            }

            ptsl::ExtendSelectionToTargetTracksRequestBody mGrpcRequestBody;
            ptsl::ExtendSelectionToTargetTracksResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(ExtendSelectionToTargetTracks, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
