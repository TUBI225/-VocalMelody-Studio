#include <vocalmelody/common/JsonWriter.h>

#include "TestContext.h"

#include <string>

namespace {
using vocalmelody::testing::TestContext;

void testSimpleObject(TestContext& context) {
    vocalmelody::common::JsonWriter writer;
    writer.beginObject();
    writer.key("a");
    writer.value(1);
    writer.key("b");
    writer.value(true);
    writer.key("c");
    writer.valueNull();
    writer.key("d");
    writer.value("x");
    writer.endObject();

    const std::string actual = writer.toString();
    const std::string expected = R"({"a":1,"b":true,"c":null,"d":"x"})";
    if (actual != expected) {
        std::cerr << "JSON actual:   " << actual << '\n';
        std::cerr << "JSON expected: " << expected << '\n';
    }
    context.expect(actual == expected, "simple object serializes correctly");
}

void testNested(TestContext& context) {
    vocalmelody::common::JsonWriter writer;
    writer.beginObject();
    writer.key("a");
    writer.beginObject();
    writer.key("b");
    writer.beginArray();
    writer.value(1);
    writer.value(2);
    writer.value(3);
    writer.endArray();
    writer.endObject();
    writer.endObject();
    context.expect(writer.toString() == R"({"a":{"b":[1,2,3]}})",
                   "nested object and array serialize correctly");
}

void testStringEscaping(TestContext& context) {
    vocalmelody::common::JsonWriter writer;
    writer.beginObject();
    writer.key("text");
    writer.value(std::string("quote \" and slash \\ and newline \n"));
    writer.endObject();
    context.expect(writer.toString() == "{\"text\":\"quote \\\" and slash \\\\ and newline \\n\"}",
                   "string escaping is applied");
}

void testDouble(TestContext& context) {
    vocalmelody::common::JsonWriter writer;
    writer.beginObject();
    writer.key("value");
    writer.value(1.5);
    writer.endObject();
    context.expect(writer.toString() == R"({"value":1.500000})",
                   "double values serialize with default formatting");
}

} // namespace

int main() {
    TestContext context;
    testSimpleObject(context);
    testNested(context);
    testStringEscaping(context);
    testDouble(context);
    return context.result();
}
