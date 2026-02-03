// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ExportSessionInfoAsText command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<ExportSessionInfoAsTextResponse> CppPTSLClient::ExportSessionInfoAsText(
        const ExportSessionInfoAsTextRequest& request)
    {
        struct ExportSessionInfoAsTextHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ExportSessionInfoAsText);

            std::string GetRequestName() const override
            {
                return "AsyncExportSessionInfoAsText";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<ExportSessionInfoAsTextResponse>(mResponse)->sessionInfo =
                    mGrpcResponseBody.session_info();
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }

            MAKE_RESP_OVRD(ExportSessionInfoAsText);

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
            void FillGrpcRequest(const ExportSessionInfoAsTextRequest& request)
            {
                mGrpcRequestBody.set_include_file_list(request.includeFileList);
                mGrpcRequestBody.set_include_clip_list(request.includeClipList);
                mGrpcRequestBody.set_include_markers(request.includeMarkers);
                mGrpcRequestBody.set_include_plugin_list(request.includePluginList);
                mGrpcRequestBody.set_include_track_edls(request.includeTrackEDLs);
                mGrpcRequestBody.set_track_offset_options(
                    static_cast<ptsl::TrackOffsetOptions>(request.trackOffsetOptions));
                mGrpcRequestBody.set_text_as_file_format(static_cast<ptsl::TextAsFileFormat>(request.textAsFileFormat));
                mGrpcRequestBody.set_output_type(static_cast<ptsl::ESI_OutputType>(request.outputType));
                mGrpcRequestBody.set_output_path(request.outputPath);

                if (request.includeTrackEDLs)
                {
                    mGrpcRequestBody.set_show_sub_frames(request.showSubFrames);
                    mGrpcRequestBody.set_include_user_timestamps(request.includeUserTimestamps);
                    mGrpcRequestBody.set_track_list_type(static_cast<ptsl::TrackListType>(request.trackListType));
                    mGrpcRequestBody.set_fade_handling_type(
                        static_cast<ptsl::FadeHandlingType>(request.fadeHandlingType));
                }
            }

            ptsl::ExportSessionInfoAsTextRequestBody mGrpcRequestBody;
            ptsl::ExportSessionInfoAsTextResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(ExportSessionInfoAsText, true, true);

        return std::dynamic_pointer_cast<ExportSessionInfoAsTextResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
