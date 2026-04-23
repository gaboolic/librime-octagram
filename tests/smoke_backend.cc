#include "kenlm_grammar.h"
#include "octagram.h"

#include <cmath>
#include <rime/config.h>
#include <iostream>
#include <memory>

using namespace rime;

int main(int argc, char** argv) {
  OctagramComponent component;

  Config octagram_config;
  octagram_config.SetString("grammar/language", "zh-hant");
  std::unique_ptr<Grammar> octagram(component.Create(&octagram_config));
  if (!octagram || !dynamic_cast<Octagram*>(octagram.get())) {
    std::cerr << "failed to create octagram backend\n";
    return 1;
  }

  Config kenlm_config;
  kenlm_config.SetString("grammar/type", "kenlm");
  kenlm_config.SetString("grammar/model_path", argc > 1 ? argv[1] : "missing.binary");
  std::unique_ptr<Grammar> kenlm(component.Create(&kenlm_config));
  if (!kenlm || !dynamic_cast<KenlmGrammar*>(kenlm.get())) {
    std::cerr << "failed to create kenlm backend\n";
    return 1;
  }

  if (argc > 1) {
    GrammarContext context{"", {"moqi", "input"}};
    const double score = kenlm->Query(context, "method", true);
    if (!std::isfinite(score)) {
      std::cerr << "kenlm query returned non-finite score\n";
      return 1;
    }
  }

  return 0;
}
