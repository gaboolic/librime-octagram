//
// Unit tests for Octagram module
//

#include "octagram.h"
#include "gram_db.h"
#include "kenlm_grammar.h"
#include <gtest/gtest.h>
#include <rime/config.h>
#include <filesystem>
#include <memory>

using namespace rime;

// Note: We'll test OctagramComponent through its public interface
// For more advanced testing, you may need to add a test-friendly method
// or use friend classes

class OctagramTest : public ::testing::Test {
 protected:
  void SetUp() override {
    component_ = std::make_unique<OctagramComponent>();
    
    // Create a temporary test database
    test_dir_ = std::filesystem::temp_directory_path() / "rime_octagram_test";
    std::filesystem::create_directories(test_dir_);
    test_db_path_ = test_dir_ / "test.gram";
    
    // Build a test database
    GramDb db(test_db_path_);
    std::vector<std::pair<std::string, double>> test_data = {
      {"你好", 1.0},
      {"世界", 2.0},
    };
    db.Build(test_data);
    db.Save();
    db.Close();
  }

  void TearDown() override {
    if (std::filesystem::exists(test_db_path_)) {
      std::filesystem::remove(test_db_path_);
    }
    if (std::filesystem::exists(test_dir_)) {
      std::filesystem::remove_all(test_dir_);
    }
  }

  std::unique_ptr<OctagramComponent> component_;
  std::filesystem::path test_dir_;
  std::filesystem::path test_db_path_;
};

// Test Octagram construction with null config
TEST_F(OctagramTest, ConstructionNullConfig) {
  Octagram octagram(nullptr, component_.get());
  // Should construct without crashing
}

// Test Octagram construction with config
TEST_F(OctagramTest, ConstructionWithConfig) {
  // Create a minimal config
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Octagram octagram(&config, component_.get());
  // Should construct successfully
}

// Test Query with empty context
TEST_F(OctagramTest, QueryEmptyContext) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Octagram octagram(&config, component_.get());
  double result = octagram.Query("", "test", false);
  
  // Should return non_collocation_penalty (default -12)
  EXPECT_LE(result, 0);
}

// Test Query with valid context and word
TEST_F(OctagramTest, QueryValidInput) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Octagram octagram(&config, component_.get());
  double result = octagram.Query("你", "好", false);
  
  // Should return a valid score
  EXPECT_TRUE(std::isfinite(result));
}

// Test Query with is_rear flag
TEST_F(OctagramTest, QueryWithRear) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Octagram octagram(&config, component_.get());
  double result = octagram.Query("你", "好", true);
  
  // Should return a valid score (may be different from non-rear)
  EXPECT_TRUE(std::isfinite(result));
}

// Test Query with different context lengths
TEST_F(OctagramTest, QueryDifferentContextLengths) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Octagram octagram(&config, component_.get());
  
  double result1 = octagram.Query("你", "好", false);
  double result2 = octagram.Query("你好", "世界", false);
  
  // Both should return valid scores
  EXPECT_TRUE(std::isfinite(result1));
  EXPECT_TRUE(std::isfinite(result2));
}

// Test OctagramComponent Create method
TEST_F(OctagramTest, ComponentCreate) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  
  Grammar* grammar = component_->Create(&config);
  EXPECT_NE(grammar, nullptr);
  EXPECT_NE(dynamic_cast<Octagram*>(grammar), nullptr);
  
  if (grammar) {
    delete grammar;
  }
}

TEST_F(OctagramTest, ComponentCreateKenlmBackend) {
  Config config;
  config.SetString("grammar/type", "kenlm");
  config.SetString("grammar/model_path", "missing.binary");

  Grammar* grammar = component_->Create(&config);
  EXPECT_NE(grammar, nullptr);
  EXPECT_NE(dynamic_cast<KenlmGrammar*>(grammar), nullptr);

  if (grammar) {
    delete grammar;
  }
}

// Test OctagramComponent GetDb method
// Note: This test requires proper resource resolver setup
// which may not be available in unit test environment
// TEST_F(OctagramTest, ComponentGetDb) {
//   GramDb* db = component_->GetDb("zh-hant");
//   // This requires Service::instance() and ResourceResolver to be set up
// }

// Test with custom configuration values
TEST_F(OctagramTest, QueryWithCustomConfig) {
  Config config;
  config.SetString("grammar/language", "zh-hant");
  config.SetInt("grammar/collocation_max_length", 5);
  config.SetInt("grammar/collocation_min_length", 2);
  config.SetDouble("grammar/collocation_penalty", -10.0);
  config.SetDouble("grammar/non_collocation_penalty", -15.0);
  
  Octagram octagram(&config, component_.get());
  double result = octagram.Query("你", "好", false);
  
  EXPECT_TRUE(std::isfinite(result));
}

