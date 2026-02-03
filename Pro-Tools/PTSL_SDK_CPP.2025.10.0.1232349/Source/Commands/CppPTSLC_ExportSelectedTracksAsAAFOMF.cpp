// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ExportSelectedTracksAsAAFOMF command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ExportSelectedTracksAsAAFOMF(
        const ExportSelectedTracksAsAAFOMFRequest& request)
    {
        struct ExportSelectedTracksAsAAFOMFHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ExportSelectedTracksAsAAFOMF);

            std::string GetRequestName() const override
            {
                return "AsyncExportSelectedTracksAsAAFOMF";
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
            void FillGrpcRequest(const ExportSelectedTracksAsAAFOMFRequest& request)
            {
                mGrpcRequestBody.set_file_type(static_cast<ptsl::ExportAsAAFFileType>(request.fileType));
                mGrpcRequestBody.set_bit_depth(static_cast<ptsl::AAFFileBitDepth>(request.bitDepth));
                mGrpcRequestBody.set_copy_option(static_cast<ptsl::CopyOption>(request.copyOption));
                mGrpcRequestBody.set_enforce_media_composer_compatibility(request.enforceMediaComposerCompatibility);
                mGrpcRequestBody.set_quantize_edits_to_frame_boundaries(request.quantizeEditsToFrameBoundaries);
                mGrpcRequestBody.set_export_stereo_as_multichannel(request.exportStereoAsMultichannel);
                mGrpcRequestBody.set_container_file_name(request.containerFileName);
                mGrpcRequestBody.set_container_file_location(request.containerFileLocation);
                mGrpcRequestBody.set_asset_file_location(request.assetFileLocation);
                mGrpcRequestBody.set_comments(request.comments);
                mGrpcRequestBody.set_sequence_name(request.sequenceName);
            }

            ptsl::ExportSelectedTracksAsAAFOMFRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(ExportSelectedTracksAsAAFOMF, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
