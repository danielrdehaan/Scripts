// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CreateSession command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CreateSession(const CreateSessionRequest& request)
    {
        struct CreateSessionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CreateSession);

            std::string GetRequestName() const override
            {
                return "AsyncCreateSession";
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
            void FillGrpcRequest(const CreateSessionRequest& request)
            {
                mGrpcRequestBody.set_session_name(request.sessionName);
                mGrpcRequestBody.set_create_from_template(request.createFromTemplate);

                if (request.createFromTemplate)
                {
                    mGrpcRequestBody.set_template_group(request.templateGroup);
                    mGrpcRequestBody.set_template_name(request.templateName);
                }

                mGrpcRequestBody.set_file_type(static_cast<ptsl::FileType>(request.fileType));
                mGrpcRequestBody.set_sample_rate(static_cast<ptsl::SampleRate>(request.sampleRate));
                mGrpcRequestBody.set_input_output_settings(static_cast<ptsl::IOSettings>(request.inputOutputSettings));
                mGrpcRequestBody.set_is_interleaved(request.isInterleaved);
                mGrpcRequestBody.set_session_location(request.sessionLocation);
                mGrpcRequestBody.set_is_cloud_project(request.isCloudProject);
                mGrpcRequestBody.set_bit_depth(static_cast<ptsl::BitDepth>(request.bitDepth));
                mGrpcRequestBody.set_create_from_aaf(request.createFromAAF);

                if (request.createFromAAF)
                {
                    mGrpcRequestBody.set_path_to_aaf(request.pathToAAF);
                }
            }

            ptsl::CreateSessionRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(CreateSession, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
