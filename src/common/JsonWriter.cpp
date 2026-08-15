#include <vocalmelody/common/JsonWriter.h>

#include <cmath>
#include <cstdio>

namespace vocalmelody::common {

JsonWriter::JsonWriter() = default;

void JsonWriter::beginObject() {
    writeSeparator();
    out_ += '{';
    itemCounts_.push_back(-1);
}

void JsonWriter::endObject() {
    itemCounts_.pop_back();
    out_ += '}';
}

void JsonWriter::beginArray() {
    writeSeparator();
    out_ += '[';
    itemCounts_.push_back(-1);
}

void JsonWriter::endArray() {
    itemCounts_.pop_back();
    out_ += ']';
}

void JsonWriter::key(const std::string_view name) {
    writeSeparator();
    writeString(name);
    out_ += ':';
}

void JsonWriter::value(const std::string_view value) {
    writeSeparator();
    writeString(value);
}

void JsonWriter::value(const char* text) { value(std::string_view(text)); }

void JsonWriter::value(const int value) {
    writeSeparator();
    out_ += std::to_string(value);
}

void JsonWriter::value(const double value) {
    if (!std::isfinite(value)) {
        valueNull();
        return;
    }
    writeSeparator();
    out_ += std::to_string(value);
}

void JsonWriter::value(const bool value) {
    writeSeparator();
    out_ += value ? "true" : "false";
}

void JsonWriter::valueNull() {
    writeSeparator();
    out_ += "null";
}

std::string JsonWriter::toString() const { return out_; }

void JsonWriter::writeSeparator() {
    if (itemCounts_.empty()) {
        return;
    }
    if (!out_.empty() && out_.back() == ':') {
        return;
    }
    if (itemCounts_.back() >= 0) {
        out_ += ',';
    }
    itemCounts_.back() += 1;
}

void JsonWriter::writeString(const std::string_view text) {
    out_ += '"';
    for (const char c : text) {
        switch (c) {
        case '"':
            out_ += "\\\"";
            break;
        case '\\':
            out_ += "\\\\";
            break;
        case '\n':
            out_ += "\\n";
            break;
        case '\r':
            out_ += "\\r";
            break;
        case '\t':
            out_ += "\\t";
            break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                char buffer[7]{};
                std::snprintf(buffer, sizeof(buffer), "\\u%04x",
                              static_cast<unsigned int>(static_cast<unsigned char>(c)));
                out_ += buffer;
            } else {
                out_ += c;
            }
            break;
        }
    }
    out_ += '"';
}

} // namespace vocalmelody::common
