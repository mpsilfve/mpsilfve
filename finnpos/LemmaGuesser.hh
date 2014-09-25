#ifndef HEADER_LemmaGuesser_hh
#define HEADER_LemmaGuesser_hh

#include "LabelExtractor.hh"
#include "LemmaExtractor.hh"
#include "Data.hh"

typedef std::vector<unsigned int> LemmaVector;

class LemmaGuesser : public LemmaExtractor
{
 public:
  LemmaGuesser(cosnt Data &data);

  ~LemmaGuesser(void);
  void train(const Data &train_data, const Data &dev_data);
  virtual std::string get_lemma_candidate(const std::string &word_form, unsigned int label) const = 0;
  virtual bool is_known_wf(const std::string &word_form) const = 0;
};

#endif // HEADER_LemmaGuesser_hh
