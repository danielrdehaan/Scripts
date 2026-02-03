// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetFileLocation command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetFileLocationResponse> CppPTSLClient::GetFileLocation(const GetFileLocationRequest& request)
    {
        struct GetFileLocationHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetFileLocation);

            std::string GetRequestName() const override
            {
                return "AsyncGetFileLocation";
            }

            void OnHasBody() override
            {
                for (const auto& item : mGrpcResponseBody.file_locations())
                {
                    std::dynamic_pointer_cast<GetFileLocationResponse>(mResponse)->fileLocations.push_back(
                        FileLocation { item.path(), FileLocationInfo { item.info().is_online() } });
                }
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetFileLocationResponse>(mResponse)->fileLocations.resize(0);
            }

            MAKE_RESP_OVRD(GetFileLocation);

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
            void FillGrpcRequest(const GetFileLocationRequest& request)
            {
                mGrpcRequestBody.mutable_pagination_request()->set_limit(request.paginationRequest.limit);
                mGrpcRequestBody.mutable_pagination_request()->set_offset(request.paginationRequest.offset);

                std::vector<ptsl::FileLocationTypeFilter> temporaryList;
                for (const auto& item : request.fileFilters)
                {
                    temporaryList.push_back(static_cast<ptsl::FileLocationTypeFilter>(item));
                }

                *mGrpcRequestBody.mutable_file_filters() = { temporaryList.begin(), temporaryList.end() };
            }

            ptsl::GetFileLocationRequestBody mGrpcRequestBody;
            ptsl::GetFileLocationResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetFileLocation, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);

        return std::dynamic_pointer_cast<GetFileLocationResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
