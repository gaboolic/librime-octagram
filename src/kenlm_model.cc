#include "kenlm_model.h"

#include <algorithm>
#include <memory>

#ifdef RIME_OCTAGRAM_ENABLE_KENLM
#include <lm/model.hh>
#include <lm/virtual_interface.hh>
#include <util/mmap.hh>
#endif

namespace rime {

#ifdef RIME_OCTAGRAM_ENABLE_KENLM

class KenlmModel::Impl {
 public:
  bool Load(const string& model_path, size_t* order_out) {
    lm::ngram::Config config;
    config.load_method = util::POPULATE_OR_READ;
    model_.reset(lm::ngram::LoadVirtual(model_path.c_str(), config));
    if (!model_) {
      return false;
    }
    vocab_ = &model_->BaseVocabulary();
    *order_out = model_->Order();
    return true;
  }

  double Query(const vector<string>& history_tokens,
               const string& word,
               bool is_rear,
               const KenlmModel::Config& config,
               size_t order) const {
    if (!model_ || !vocab_ || word.empty()) {
      return config.override_oov_penalty ? config.oov_penalty : 0.0;
    }

    lm::ngram::State state;
    lm::ngram::State out_state;
    if (config.use_bos_eos) {
      model_->BeginSentenceWrite(&state);
    } else {
      model_->NullContextWrite(&state);
    }

    const size_t history_limit = order > 0 ? order - 1 : 0;
    const size_t start =
        history_tokens.size() > history_limit ? history_tokens.size() - history_limit : 0;
    for (size_t i = start; i < history_tokens.size(); ++i) {
      const auto token = vocab_->Index(history_tokens[i]);
      model_->BaseScore(&state, token, &out_state);
      state = out_state;
    }

    const auto word_index = vocab_->Index(word);
    double score = model_->BaseScore(&state, word_index, &out_state);
    state = out_state;
    if (word_index == vocab_->NotFound() && config.override_oov_penalty) {
      score = config.oov_penalty;
    }

    if (is_rear && config.use_bos_eos) {
      score += model_->BaseScore(&state, vocab_->EndSentence(), &out_state);
    }
    return score;
  }

 private:
  std::unique_ptr<lm::base::Model> model_;
  const lm::base::Vocabulary* vocab_ = nullptr;
};

#endif  // RIME_OCTAGRAM_ENABLE_KENLM

KenlmModel::KenlmModel(string model_path)
    : model_path_(std::move(model_path)) {}

KenlmModel::~KenlmModel() = default;

bool KenlmModel::Load() {
#ifdef RIME_OCTAGRAM_ENABLE_KENLM
  if (loaded_) {
    return true;
  }
  impl_ = std::make_unique<Impl>();
  loaded_ = impl_->Load(model_path_, &order_);
  if (!loaded_) {
    impl_.reset();
    LOG(ERROR) << "failed to load kenlm model: " << model_path_;
  }
  return loaded_;
#else
  LOG(ERROR) << "kenlm backend requested but librime-octagram was built without KenLM support.";
  return false;
#endif
}

double KenlmModel::Query(const vector<string>& history_tokens,
                         const string& word,
                         bool is_rear,
                         const Config& config) const {
#ifdef RIME_OCTAGRAM_ENABLE_KENLM
  if (!impl_ || !loaded_) {
    return config.override_oov_penalty ? config.oov_penalty : 0.0;
  }
  return impl_->Query(history_tokens, word, is_rear, config, order_);
#else
  return config.override_oov_penalty ? config.oov_penalty : 0.0;
#endif
}

}  // namespace rime
