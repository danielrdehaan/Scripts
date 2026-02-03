// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetDynamicProperties command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetDynamicPropertiesResponse> CppPTSLClient::GetDynamicProperties(
        const GetDynamicPropertiesRequest& request)
    {
        struct GetDynamicPropertiesHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetDynamicProperties);

            std::string GetRequestName() const override
            {
                return "AsyncGetDynamicProperties";
            }

            void OnHasBody() override
            {
                std::vector<GetDynamicPropertiesGroup> tempGroupList;
                for (const auto& item : mGrpcResponseBody.group_list())
                {
                    GetDynamicPropertiesGroup group;

                    std::vector<PropertyContainer> tempKeyList;
                    for (const auto& key : item.key_list())
                    {
                        PropertyContainer container;
                        container.name = key.container_name();
                        container.type = static_cast<DP_ValueTypes>(key.type());
                        container.value = key.value();
                    }
                    group.keyList = { tempKeyList.begin(), tempKeyList.end() };

                    std::vector<PropertyDescriptor> tempPropertyList;
                    for (const auto& property : item.property_list())
                    {
                        PropertyDescriptor descriptor;
                        descriptor.name = property.name();
                        descriptor.valueType = static_cast<DP_ValueTypes>(property.value_type());
                        descriptor.objectType = property.object_type();
                        descriptor.required = property.required();
                        descriptor.description = property.description();
                        descriptor.units = property.units();
                        descriptor.acceptedValues = { property.accepted_values().begin(),
                            property.accepted_values().end() };
                        descriptor.maxValue = property.max_value();
                        descriptor.minValue = property.min_value();
                    }
                    group.propertyList = { tempPropertyList.begin(), tempPropertyList.end() };

                    tempGroupList.push_back(group);
                }

                std::dynamic_pointer_cast<GetDynamicPropertiesResponse>(mResponse)->groupList = { tempGroupList.begin(),
                    tempGroupList.end() };
                std::dynamic_pointer_cast<GetDynamicPropertiesResponse>(mResponse)->propertyType =
                    static_cast<DynamicPropertyType>(mGrpcResponseBody.property_type());
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetDynamicPropertiesResponse>(mResponse)->groupList.resize(0);
            }

            MAKE_RESP_OVRD(GetDynamicProperties);

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
            void FillGrpcRequest(const GetDynamicPropertiesRequest& request)
            {
                mGrpcRequestBody.set_property_type(static_cast<ptsl::DynamicPropertyType>(request.propertyType));
            }

            ptsl::GetDynamicPropertiesRequestBody mGrpcRequestBody;
            ptsl::GetDynamicPropertiesResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetDynamicProperties, true, true);

        return std::dynamic_pointer_cast<GetDynamicPropertiesResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
