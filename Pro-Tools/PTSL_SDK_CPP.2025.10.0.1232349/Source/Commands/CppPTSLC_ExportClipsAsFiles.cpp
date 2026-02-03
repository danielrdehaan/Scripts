// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ExportClipsAsFiles command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ExportClipsAsFiles(const ExportClipsAsFilesRequest& request)
    {
        struct ExportClipsAsFilesHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ExportClipsAsFiles);

            std::string GetRequestName() const override
            {
                return "AsyncExportClipsAsFiles";
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
            void FillGrpcRequest(const ExportClipsAsFilesRequest& request)
            {
                mGrpcRequestBody.set_file_path(request.filePath);
                mGrpcRequestBody.set_format(static_cast<ptsl::ExportFormat>(request.format));
                mGrpcRequestBody.set_file_type(static_cast<ptsl::ExportFileType>(request.fileType));
                mGrpcRequestBody.set_bit_depth(static_cast<ptsl::BitDepth>(request.bitDepth));
                mGrpcRequestBody.set_duplicate_names(
                    static_cast<ptsl::ResolveDuplicateNamesBy>(request.duplicateNames));
                mGrpcRequestBody.set_enforce_avid_compatibility(request.enforceAvidCompatibility);
                mGrpcRequestBody.set_sample_rate_custom(request.sampleRateCustom);
            }

            ptsl::ExportClipsAsFilesRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(ExportClipsAsFiles, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
