// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::HostReadyCheck command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    /**
    * Checks to see if the Pro Tools application is fully loaded and ready to execute PTSL commands.
    * Called by internal client wrapper utility function @ref PTSLC_CPP::CppPTSLClient::HostReadyCheck.
    */
    std::shared_ptr<HostReadyCheckResponse> CppPTSLClient::HostReadyCheck(const CommandRequest& request)
    {
        struct HostReadyCheckHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(HostReadyCheck);

            std::string GetRequestName() const override
            {
                return "AsyncHostReadyCheck";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<HostReadyCheckResponse>(mResponse)->isHostReady =
                    mGrpcResponseBody.is_host_ready();
            }

            void OnNoBody() override
            {
                // overrided OnNoBody to handle backward compatibility, so if the old Pro Tools version
                // returns a default command response without HostReadyCheckResponseBody, we set m_isHostReady
                // based on the command status as it was implemented in the old version of C++ Client
                std::dynamic_pointer_cast<HostReadyCheckResponse>(mResponse)->isHostReady =
                    mResponse->status.type == TaskStatus::TStatus_Completed;
            }

            MAKE_RESP_OVRD(HostReadyCheck);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::HostReadyCheckResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(HostReadyCheck, /*HAS_REQ*/ false, /*IS_STREAMING*/ false);

        return std::dynamic_pointer_cast<HostReadyCheckResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
