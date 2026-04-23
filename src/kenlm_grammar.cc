#include "kenlm_grammar.h"

#include "octagram.h"
#include <rime/config.h>

namespace rime {

KenlmGrammar::KenlmGrammar(Config* config, OctagramComponent* component) {
  if (!config) {
    return;
  }
  string model_path;
  if (!config->GetString("grammar/model_path", &model_path) || model_path.empty()) {
    LOG(ERROR) << "kenlm grammar requires grammar/model_path.";
    return;
  }
  if (config->GetDouble("grammar/oov_penalty", &config_.oov_penalty)) {
    config_.override_oov_penalty = true;
  }
  config->GetBool("grammar/use_bos_eos", &config_.use_bos_eos);
  model_ = component->GetKenlmModel(model_path);
}

KenlmGrammar::~KenlmGrammar() = default;

double KenlmGrammar::Query(const GrammarContext& context,
                           const string& word,
                           bool is_rear) {
  if (!model_ || !model_->loaded()) {
    return config_.override_oov_penalty ? config_.oov_penalty : 0.0;
  }
  auto history_tokens = context.history_tokens;
  if (history_tokens.empty() && !context.raw_text.empty()) {
    history_tokens = GrammarContext::TokenizeRawText(context.raw_text);
  }
  return model_->Query(history_tokens, word, is_rear, config_);
}

double KenlmGrammar::Query(const string& context,
                           const string& word,
                           bool is_rear) {
  return Query(GrammarContext::FromRawText(context), word, is_rear);
}

}  // namespace rime
