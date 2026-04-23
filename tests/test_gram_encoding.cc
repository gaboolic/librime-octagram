//
// Unit tests for gram_encoding module
//

#include "gram_encoding.h"
#include <gtest/gtest.h>
#include <string>

using namespace rime;
using namespace rime::grammar;

class GramEncodingTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

// Test basic encoding of ASCII characters
TEST_F(GramEncodingTest, EncodeASCII) {
  std::string input = "hello";
  std::string encoded = encode(input);
  // ASCII characters should be encoded as-is (except null)
  EXPECT_EQ(encoded.length(), input.length());
  EXPECT_EQ(encoded, "hello");
}

// Test encoding of null character
TEST_F(GramEncodingTest, EncodeNull) {
  std::string input = "\0test";
  std::string encoded = encode(input.c_str(), input.c_str() + input.length());
  // Null should be encoded as 0xE0
  EXPECT_NE(encoded[0], '\0');
}

// Test encoding of Chinese characters (CJK)
TEST_F(GramEncodingTest, EncodeCJK) {
  // Test with Chinese characters (UTF-8)
  std::string input = "你好";
  std::string encoded = encode(input);
  // Should encode successfully
  EXPECT_FALSE(encoded.empty());
  EXPECT_NE(encoded, input);  // Should be different from input
}

// Test encoding of empty string
TEST_F(GramEncodingTest, EncodeEmpty) {
  std::string input = "";
  std::string encoded = encode(input);
  EXPECT_TRUE(encoded.empty());
}

// Test next_unicode function
TEST_F(GramEncodingTest, NextUnicode) {
  std::string input = "hello";
  const char* p = input.c_str();
  const char* next = next_unicode(p);
  EXPECT_EQ(next - p, 1);  // ASCII character is 1 byte
}

// Test next_unicode with multi-byte characters
TEST_F(GramEncodingTest, NextUnicodeMultiByte) {
  std::string input = "你好";
  const char* p = input.c_str();
  const char* next = next_unicode(p);
  // Chinese character in UTF-8 is 3 bytes
  EXPECT_GE(next - p, 1);
}

// Test unicode_length function
TEST_F(GramEncodingTest, UnicodeLength) {
  std::string encoded = encode("hello");
  size_t length = unicode_length(encoded, encoded.length());
  EXPECT_EQ(length, 5);  // "hello" has 5 characters
}

// Test unicode_length with Chinese characters
TEST_F(GramEncodingTest, UnicodeLengthCJK) {
  std::string input = "你好";
  std::string encoded = encode(input);
  size_t length = unicode_length(encoded, encoded.length());
  EXPECT_EQ(length, 2);  // "你好" has 2 characters
}

// Test encoding with various Unicode ranges
TEST_F(GramEncodingTest, EncodeVariousUnicode) {
  // Test with different Unicode ranges
  std::string input1 = "a";      // ASCII
  std::string input2 = "中";     // CJK
  std::string input3 = "€";      // Extended
  
  std::string encoded1 = encode(input1);
  std::string encoded2 = encode(input2);
  std::string encoded3 = encode(input3);
  
  EXPECT_FALSE(encoded1.empty());
  EXPECT_FALSE(encoded2.empty());
  EXPECT_FALSE(encoded3.empty());
}

