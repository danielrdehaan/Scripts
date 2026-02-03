// Copyright 2022-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RenameTargetClip command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RenameTargetClip(const RenameTargetClipRequest& request)
    {
        struct RenameTargetClipHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RenameTargetClip);

            std::string GetRequestName() const override
            {
                return "AsyncRenameTargetClip";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const RenameTargetClipRequest& request)
            {
                mGrpcRequestBody.set_clip_name(request.clipName);
                mGrpcRequestBody.set_new_name(request.newName);
                if (request.renameFileExplicitly)
                {
                    mGrpcRequestBody.set_rename_file(request.renameFile);
                }
            }

        private:
            ptsl::RenameTargetClipRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(RenameTargetClip, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
