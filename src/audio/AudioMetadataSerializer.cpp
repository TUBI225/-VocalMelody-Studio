#include <vocalmelody/audio/AudioMetadataSerializer.h>

#include <vocalmelody/common/JsonWriter.h>

#include <filesystem>
#include <fstream>

namespace vocalmelody::audio {

std::string audioMetadataToJson(const AudioImportResult& result) {
    common::JsonWriter writer;
    writer.beginObject();
    writer.key("source");
    writer.beginObject();
    writer.key("id");
    writer.value(result.source.id());
    writer.key("originalPath");
    writer.value(result.source.originalPath());
    writer.key("importedAt");
    writer.value(result.source.importedAt());
    writer.key("originalFormat");
    writer.value(common::audioFormatToString(result.source.originalFormat()));
    writer.key("sampleRate");
    writer.value(result.source.sampleRate());
    writer.key("channelCount");
    writer.value(result.source.channelCount());
    writer.key("bitDepth");
    writer.value(result.source.bitDepth());
    writer.key("durationSeconds");
    writer.value(result.source.durationSeconds().value());
    writer.key("fileHash");
    writer.value(result.source.fileHash());
    writer.endObject();
    writer.key("analysis");
    writer.beginObject();
    writer.key("analysisVersion");
    writer.value(result.analysis.analysisVersion());
    writer.key("durationSeconds");
    writer.value(result.analysis.durationSeconds().value());
    writer.key("analysisSampleRate");
    writer.value(result.analysis.analysisSampleRate());
    writer.key("monoAnalysisPath");
    writer.value(result.analysis.monoAnalysisPath());
    writer.key("clippingScore");
    writer.value(result.analysis.clippingScore().value());
    writer.key("noiseScore");
    writer.value(result.analysis.noiseScore().value());
    writer.key("voicePresenceScore");
    writer.value(result.analysis.voicePresenceScore().value());
    writer.key("qualityScore");
    writer.value(result.analysis.qualityScore().value());
    writer.key("silenceSegments");
    writer.beginArray();
    for (const auto& segment : result.analysis.silenceMap()) {
        writer.beginObject();
        writer.key("startSeconds");
        writer.value(segment.start().value());
        writer.key("endSeconds");
        writer.value(segment.end().value());
        writer.endObject();
    }
    writer.endArray();
    writer.key("warnings");
    writer.beginArray();
    for (const auto& warning : result.analysis.warnings()) {
        writer.value(warning);
    }
    writer.endArray();
    writer.endObject();
    writer.endObject();
    return writer.toString();
}

bool saveAudioMetadata(const AudioImportResult& result, const std::string& path) {
    // Ouverture par std::filesystem::path : les chemins non-ASCII (UTF-8 fournis
    // par juce::File) échoueraient avec un ofstream sur chaîne étroite.
    const auto utf8Path = std::filesystem::path(std::u8string(path.begin(), path.end()));
    std::ofstream file(utf8Path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file << audioMetadataToJson(result);
    file << '\n';
    return file.good();
}

} // namespace vocalmelody::audio
