// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetPlaybackMode command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<SetPlaybackModeResponse> CppPTSLClient::SetPlaybackMode(const SetPlaybackModeRequest& request)
    {
        struct SetPlaybackModeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetPlaybackMode);

            std::string GetRequestName() const override
            {
                return "AsyncSetPlaybackMode";
            }

            void OnHasBody() override
            {
                for (const auto& item : mGrpcResponseBody.playback_mode_list())
                {
                    std::dynamic_pointer_cast<SetPlaybackModeResponse>(mResponse)->playbackModeList.push_back(
                        static_cast<PlaybackMode>(item));
                }

                std::dynamic_pointer_cast<SetPlaybackModeResponse>(mResponse)->currentPlaybackMode =
                    static_cast<PlaybackMode>(mGrpcResponseBody.current_playback_mode());
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<SetPlaybackModeResponse>(mResponse)->playbackModeList.resize(0);
            }

            MAKE_RESP_OVRD(SetPlaybackMode);

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
            void FillGrpcRequest(const SetPlaybackModeRequest& request)
            {
                mGrpcRequestBody.set_playback_mode(static_cast<ptsl::PlaybackMode>(request.playbackMode));
            }

            ptsl::SetPlaybackModeRequestBody mGrpcRequestBody;
            ptsl::SetPlaybackModeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(SetPlaybackMode, true, true);

        return std::dynamic_pointer_cast<SetPlaybackModeResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
