// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetTrackList command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetTrackListResponse> CppPTSLClient::GetTrackList(const GetTrackListRequest& request)
    {
        struct GetTrackListHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetTrackList);

            std::string GetRequestName() const override
            {
                return "AsyncGetTrackList";
            }

            void OnHasBody() override
            {
                auto FillTrack = [](const ptsl::Track& iSrcTrack, Track& oDstTrack)
                {
                    TrackAttributes attributes;
                    attributes.isInactive =
                        static_cast<TrackAttributeState>(iSrcTrack.track_attributes().is_inactive());
                    attributes.isHidden = static_cast<TrackAttributeState>(iSrcTrack.track_attributes().is_hidden());
                    attributes.isSelected =
                        static_cast<TrackAttributeState>(iSrcTrack.track_attributes().is_selected());
                    attributes.containsClips = iSrcTrack.track_attributes().contains_clips();
                    attributes.containsAutomation = iSrcTrack.track_attributes().contains_automation();
                    attributes.isSoloed = iSrcTrack.track_attributes().is_soloed();
                    attributes.isRecordEnabled = iSrcTrack.track_attributes().is_record_enabled();
                    attributes.isInputMonitoringOn =
                        static_cast<TrackAttributeState>(iSrcTrack.track_attributes().is_input_monitoring_on());
                    attributes.isSmartDspOn = iSrcTrack.track_attributes().is_smart_dsp_on();
                    attributes.isLocked = iSrcTrack.track_attributes().is_locked();
                    attributes.isMuted = iSrcTrack.track_attributes().is_muted();
                    attributes.isFrozen = iSrcTrack.track_attributes().is_frozen();
                    attributes.isOpen = iSrcTrack.track_attributes().is_open();
                    attributes.isOnline = iSrcTrack.track_attributes().is_online();
                    attributes.isRecordEnabledSafe = iSrcTrack.track_attributes().is_record_enabled_safe();
                    attributes.isSmartDspOnSafe = iSrcTrack.track_attributes().is_smart_dsp_on_safe();
                    attributes.isSoloedSafe = iSrcTrack.track_attributes().is_soloed_safe();
                    oDstTrack.name = iSrcTrack.name();
                    oDstTrack.type = static_cast<TrackType>(iSrcTrack.type());
                    oDstTrack.id = iSrcTrack.id();
                    oDstTrack.index = iSrcTrack.index();
                    oDstTrack.color = iSrcTrack.color();
                    oDstTrack.idCompressed = iSrcTrack.id_compressed();
                    oDstTrack.format = static_cast<TrackFormat>(iSrcTrack.format());
                    oDstTrack.timebase = static_cast<TrackTimebase>(iSrcTrack.timebase());
                    oDstTrack.trackAttributes = attributes;
                    oDstTrack.parentFolderName = iSrcTrack.parent_folder_name();
                    oDstTrack.parentFolderId = iSrcTrack.parent_folder_id();
                };

                for (const auto& item : mGrpcResponseBody.track_list())
                {
                    Track track;
                    FillTrack(item, track);

                    std::dynamic_pointer_cast<GetTrackListResponse>(mResponse)->trackList.push_back(track);
                }
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetTrackListResponse>(mResponse)->trackList.resize(0);
            }

            MAKE_RESP_OVRD(GetTrackList);

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
            void FillGrpcRequest(const GetTrackListRequest& request)
            {
                mGrpcRequestBody.mutable_pagination_request()->set_limit(request.paginationRequest.limit);
                mGrpcRequestBody.mutable_pagination_request()->set_offset(request.paginationRequest.offset);

                mGrpcRequestBody.set_is_filter_list_additive(request.isFilterListAdditive);

                auto mutable_track_filters = mGrpcRequestBody.mutable_track_filter_list();

                std::vector<ptsl::TrackListInvertibleFilter> tempTrackInvFilters;
                for (const auto& item : request.trackFilters)
                {
                    ptsl::TrackListInvertibleFilter invFilter;
                    invFilter.set_is_inverted(item.isInverted);
                    invFilter.set_filter(static_cast<ptsl::TrackListFilter>(item.filter));
                    tempTrackInvFilters.push_back(invFilter);
                }

                *mGrpcRequestBody.mutable_track_filter_list() = { tempTrackInvFilters.begin(),
                    tempTrackInvFilters.end() };
            }

            ptsl::GetTrackListRequestBody mGrpcRequestBody;
            ptsl::GetTrackListResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetTrackList, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);

        return std::dynamic_pointer_cast<GetTrackListResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
