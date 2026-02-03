// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information
// of Avid. Do not disclose to any third party. Use of the
// information contained in this document is subject to an Avid
// SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RenameTargetTrack command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RenameTargetTrack(const RenameTargetTrackRequest& request)
    {
        struct RenameTargetTrackHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RenameTargetTrack);

            std::string GetRequestName() const override
            {
                return "AsyncRenameTargetTrack";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const RenameTargetTrackRequest& request)
            {
                mGrpcRequestBody.set_track_id(request.trackId); // deprecated - replaced by current_name
                mGrpcRequestBody.set_current_name(request.currentTrackName);
                mGrpcRequestBody.set_new_name(request.newTrackName);
            }

            ptsl::RenameTargetTrackRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(RenameTargetTrack, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
