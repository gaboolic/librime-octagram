#ifndef RIME_KENLM_MODEL_H_
#define RIME_KENLM_MODEL_H_

#include <rime/common.h>

namespace rime {

class KenlmModel {
 public:
  struct Config {
    bool use_bos_eos = true;
    bool override_oov_penalty = false;
    double oov_penalty = -20.0;
  };

  explicit KenlmModel(string model_path);
  ~KenlmModel();

  bool Load();
  bool loaded() const { return loaded_; }
  const string& model_path() const { return model_path_; }
  size_t order() const { return order_; }

  double Query(const vector<string>& history_tokens,
               const string& word,
               bool is_rear,
               const Config& config) const;

 private:
  string model_path_;
  bool loaded_ = false;
  size_t order_ = 0;

#ifdef RIME_OCTAGRAM_ENABLE_KENLM
  class Impl;
  unique_ptr<Impl> impl_;
#endif
};

}  // namespace rime

#endif  // RIME_KENLM_MODEL_H_
