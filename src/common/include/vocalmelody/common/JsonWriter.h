#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace vocalmelody::common {

class JsonWriter final {
  public:
    JsonWriter();

    void beginObject();
    void endObject();
    void beginArray();
    void endArray();
    void key(std::string_view name);
    void value(std::string_view value);
    void value(const char* value);
    void value(int value);
    void value(double value);
    void value(bool value);
    void valueNull();

    [[nodiscard]] std::string toString() const;

  private:
    void writeSeparator();
    void writeString(std::string_view text);

    std::string out_;
    std::vector<int> itemCounts_;
};

} // namespace vocalmelody::common
