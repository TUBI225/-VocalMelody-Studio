#pragma once

#include <vocalmelody/audio/AudioFileImporter.h>

#include <string>

namespace vocalmelody::audio {

[[nodiscard]] std::string audioMetadataToJson(const AudioImportResult& result);

bool saveAudioMetadata(const AudioImportResult& result, const std::string& path);

} // namespace vocalmelody::audio
