// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RenameSelectedClip command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RenameSelectedClip(const RenameSelectedClipRequest& request)
    {
        struct RenameSelectedClipHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RenameSelectedClip);

            std::string GetRequestName() const override
            {
                return "AsyncRenameSelectedClip";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const RenameSelectedClipRequest& request)
            {
                mGrpcRequestBody.set_clip_location(static_cast<ptsl::ClipLocation>(request.clipLocation));
                mGrpcRequestBody.set_new_name(request.newName);
                if (request.renameFileExplicitly)
                {
                    mGrpcRequestBody.set_rename_file(request.renameFile);
                }
            }

        private:
            ptsl::RenameSelectedClipRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(RenameSelectedClip, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
