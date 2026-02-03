// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SelectAllClipsOnTrack command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SelectAllClipsOnTrack(const SelectAllClipsOnTrackRequest& request)
    {
        struct SelectAllClipsOnTrackHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SelectAllClipsOnTrack);

            std::string GetRequestName() const override
            {
                return "AsyncSelectAllClipsOnTrack";
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<SelectAllClipsOnTrackError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->trackName = mGrpcResponseBody.track_name();
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
            void FillGrpcRequest(const SelectAllClipsOnTrackRequest& request)
            {
                mGrpcRequestBody.set_track_name(request.trackName);
            }

            ptsl::SelectAllClipsOnTrackRequestBody mGrpcRequestBody;
            ptsl::SelectAllClipsOnTrackResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(SelectAllClipsOnTrack, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
