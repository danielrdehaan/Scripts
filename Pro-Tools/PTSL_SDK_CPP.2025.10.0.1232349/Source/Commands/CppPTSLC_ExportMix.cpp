// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ExportMix command.
 */

#include "CppPTSLC_DefaultRequest.h"

using namespace google::protobuf::util;

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ExportMix(const ExportMixRequest& request)
    {
        struct ExportMixHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ExportMix);

            std::string GetRequestName() const override
            {
                return "AsyncExportMix";
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
            void FillGrpcRequest(const ExportMixRequest& request)
            {
                if (request.fileType != EM_FileType::EMFType_WAVADM)
                {
                    JsonStringToMessage("{files_list:" + request.filesList + "}",
                        &mGrpcRequestBody); // deprecated - replaced by mix_source_list
                    JsonStringToMessage("{mix_source_list:" + request.mixSourceList + "}", &mGrpcRequestBody);
                }

                mGrpcRequestBody.set_preset_path(request.presetPath);
                mGrpcRequestBody.set_file_name(request.fileName);
                mGrpcRequestBody.set_file_type(static_cast<ptsl::EM_FileType>(request.fileType));

                if (request.fileType != EM_FileType::EMFType_WAVADM)
                {
                    mGrpcRequestBody.mutable_audio_info()->set_compression_type(
                        static_cast<ptsl::CompressionType>(request.audioInfo.compressionType));

                    if (request.fileType != EM_FileType::EMFType_MXFOPAtom)
                    {
                        mGrpcRequestBody.mutable_audio_info()->set_export_format(
                            static_cast<ptsl::ExportFormat>(request.audioInfo.exportFormat));
                        mGrpcRequestBody.mutable_audio_info()->set_pad_to_frame_boundary(
                            static_cast<ptsl::TripleBool>(request.audioInfo.padToFrameBoundary));
                    }

                    if (request.fileType != EM_FileType::EMFType_MP3)
                    {
                        mGrpcRequestBody.mutable_audio_info()->set_bit_depth(
                            static_cast<ptsl::BitDepth>(request.audioInfo.bitDepth));
                    }

                    mGrpcRequestBody.mutable_audio_info()->set_sample_rate(
                        static_cast<ptsl::SampleRate>(request.audioInfo.sampleRate));
                    mGrpcRequestBody.mutable_audio_info()->set_sample_rate_custom(request.audioInfo.sampleRateCustom);
                    mGrpcRequestBody.mutable_audio_info()->set_delivery_format(
                        static_cast<ptsl::EM_DeliveryFormat>(request.audioInfo.deliveryFormat));
                }

                std::string property_list = "{property_list:";
                JsonStringToMessage(property_list + request.dolbyAtmosInfo.propertyList + "}",
                    mGrpcRequestBody.mutable_dolby_atmos_info());
                mGrpcRequestBody.mutable_dolby_atmos_info()->set_add_first_frame_of_action(
                    static_cast<ptsl::TripleBool>(request.dolbyAtmosInfo.firstFrameOfAction));
                mGrpcRequestBody.mutable_dolby_atmos_info()->set_timecode_value(request.dolbyAtmosInfo.timeCodeValue);
                mGrpcRequestBody.mutable_dolby_atmos_info()->set_frame_rate(request.dolbyAtmosInfo.frameRate);

                mGrpcRequestBody.mutable_video_info()->set_include_video(
                    static_cast<ptsl::TripleBool>(request.videoInfo.includeVideo));
                mGrpcRequestBody.mutable_video_info()->set_export_option(
                    static_cast<ptsl::EM_VideoExportOptions>(request.videoInfo.videoExportOptions));
                mGrpcRequestBody.mutable_video_info()->set_replace_timecode_track(
                    static_cast<ptsl::TripleBool>(request.videoInfo.replaceTimeCodeTrack));
                mGrpcRequestBody.mutable_video_info()->mutable_codec_info()->set_codec_name(
                    request.videoInfo.codecInfo.codecName);
                JsonStringToMessage(property_list + request.videoInfo.codecInfo.propertyList + "}",
                    mGrpcRequestBody.mutable_video_info()->mutable_codec_info());

                mGrpcRequestBody.mutable_location_info()->set_file_destination(
                    static_cast<ptsl::EM_FileDestination>(request.locationInfo.fileDestination));
                mGrpcRequestBody.mutable_location_info()->set_directory(request.locationInfo.directory);

                if (request.fileType != EM_FileType::EMFType_MP3)
                {
                    mGrpcRequestBody.mutable_location_info()->set_import_after_bounce(
                        static_cast<ptsl::TripleBool>(request.locationInfo.importAfterBounce));
                    mGrpcRequestBody.mutable_location_info()->mutable_import_options()->set_import_destination(
                        static_cast<ptsl::MediaDestination>(request.locationInfo.importOptions.importDestination));
                    mGrpcRequestBody.mutable_location_info()->mutable_import_options()->set_import_location(
                        static_cast<ptsl::MediaLocation>(request.locationInfo.importOptions.importLocation));
                    mGrpcRequestBody.mutable_location_info()->mutable_import_options()->set_gaps_between_clips(
                        request.locationInfo.importOptions.gapsBetweenClips);
                    mGrpcRequestBody.mutable_location_info()->mutable_import_options()->set_import_audio_from_file(
                        static_cast<ptsl::TripleBool>(request.locationInfo.importOptions.importAudioFromFile));
                    mGrpcRequestBody.mutable_location_info()
                        ->mutable_import_options()
                        ->set_remove_existing_video_tracks(static_cast<ptsl::TripleBool>(
                            request.locationInfo.importOptions.removeExistingVideoTracks));
                    mGrpcRequestBody.mutable_location_info()->mutable_import_options()->set_remove_existing_video_clips(
                        static_cast<ptsl::TripleBool>(request.locationInfo.importOptions.removeExistingVideoClips));
                    mGrpcRequestBody.mutable_location_info()
                        ->mutable_import_options()
                        ->set_clear_destination_video_track_playlist(static_cast<ptsl::TripleBool>(
                            request.locationInfo.importOptions.clearDestinationVideoTrackPlaylist));
                }

                mGrpcRequestBody.set_offline_bounce(static_cast<ptsl::TripleBool>(request.offlineBounce));
            }

            ptsl::ExportMixRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(ExportMix, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
