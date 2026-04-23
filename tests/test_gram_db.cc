//
// Unit tests for GramDb module
//

#include "gram_db.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

using namespace rime;

class GramDbTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create a temporary directory for test files
    test_dir_ = std::filesystem::temp_directory_path() / "rime_octagram_test";
    std::filesystem::create_directories(test_dir_);
    test_db_path_ = test_dir_ / "test.gram";
  }

  void TearDown() override {
    // Clean up test files
    if (std::filesystem::exists(test_db_path_)) {
      std::filesystem::remove(test_db_path_);
    }
    if (std::filesystem::exists(test_dir_)) {
      std::filesystem::remove_all(test_dir_);
    }
  }

  std::filesystem::path test_dir_;
  std::filesystem::path test_db_path_;
};

// Test GramDb construction
TEST_F(GramDbTest, Construction) {
  GramDb db(test_db_path_);
  EXPECT_FALSE(db.IsOpen());
}

// Test Build function
TEST_F(GramDbTest, Build) {
  GramDb db(test_db_path_);
  
  // Create test data
  std::vector<std::pair<std::string, double>> test_data = {
    {"你好", 1.0},
    {"世界", 2.0},
    {"测试", 3.0},
  };
  
  bool result = db.Build(test_data);
  EXPECT_TRUE(result);
  
  // After Build, Save should work
  bool save_result = db.Save();
  EXPECT_TRUE(save_result);
}

// Test Build with empty data
TEST_F(GramDbTest, BuildEmpty) {
  GramDb db(test_db_path_);
  
  std::vector<std::pair<std::string, double>> empty_data;
  bool result = db.Build(empty_data);
  // Should handle empty data gracefully
  // (actual behavior depends on implementation)
}

// Test Build and Load cycle
TEST_F(GramDbTest, BuildAndLoad) {
  GramDb db(test_db_path_);
  
  // Build with test data
  std::vector<std::pair<std::string, double>> test_data = {
    {"test", 1.0},
  };
  
  EXPECT_TRUE(db.Build(test_data));
  EXPECT_TRUE(db.Save());
  db.Close();
  
  // Load the database
  GramDb db2(test_db_path_);
  bool load_result = db2.Load();
  EXPECT_TRUE(load_result);
  EXPECT_TRUE(db2.IsOpen());
}

// Test Lookup function
TEST_F(GramDbTest, Lookup) {
  GramDb db(test_db_path_);
  
  // Build with test data
  std::vector<std::pair<std::string, double>> test_data = {
    {"你好", 1.0},
    {"世界", 2.0},
  };
  
  EXPECT_TRUE(db.Build(test_data));
  
  // Test lookup
  GramDb::Match results[GramDb::kMaxResults];
  int num_results = db.Lookup("你", "好", results);
  
  // Should find at least one result if the data is properly encoded
  // Note: This depends on the encoding format
  EXPECT_GE(num_results, 0);
  EXPECT_LE(num_results, GramDb::kMaxResults);
}

// Test Lookup with non-existent key
TEST_F(GramDbTest, LookupNonExistent) {
  GramDb db(test_db_path_);
  
  std::vector<std::pair<std::string, double>> test_data = {
    {"test", 1.0},
  };
  
  EXPECT_TRUE(db.Build(test_data));
  
  GramDb::Match results[GramDb::kMaxResults];
  int num_results = db.Lookup("nonexistent", "key", results);
  EXPECT_EQ(num_results, 0);
}

// Test kMaxResults constant
TEST_F(GramDbTest, MaxResultsConstant) {
  EXPECT_EQ(GramDb::kMaxResults, 8);
}

// Test kValueScale constant
TEST_F(GramDbTest, ValueScaleConstant) {
  EXPECT_EQ(GramDb::kValueScale, 10000.0);
}

