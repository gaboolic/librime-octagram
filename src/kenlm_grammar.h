#ifndef RIME_KENLM_GRAMMAR_H_
#define RIME_KENLM_GRAMMAR_H_

#include "kenlm_model.h"
#include <rime/gear/grammar.h>

namespace rime {

class Config;
class OctagramComponent;

class KenlmGrammar : public Grammar {
 public:
  KenlmGrammar(Config* config, OctagramComponent* component);
  ~KenlmGrammar() override;

  double Query(const GrammarContext& context,
               const string& word,
               bool is_rear) override;
  double Query(const string& context,
               const string& word,
               bool is_rear) override;

 private:
  KenlmModel* model_ = nullptr;
  KenlmModel::Config config_;
};

}  // namespace rime

#endif  // RIME_KENLM_GRAMMAR_H_
