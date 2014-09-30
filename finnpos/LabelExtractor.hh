#ifndef HEADER_LabelExtractor_hh
#define HEADER_LabelExtractor_hh

#include <string>
#include <vector>
#include <unordered_map>


#include "io.hh"
#include "exceptions.hh"

typedef std::vector<unsigned int> LabelVector;

class Data;

class LabelExtractor
{
 public:
  LabelExtractor(unsigned int max_suffix_len);
  virtual ~LabelExtractor(void);

  virtual void set_label_candidates(const std::string &word_form,
				    bool use_lexicon,
				    unsigned int count, 
				    LabelVector &target) const;
  void train(Data &data);
  unsigned int get_boundary_label(void) const;
  unsigned int get_label(const std::string &label_string);
  LabelVector get_labels(const StringVector &label_strings);
  const std::string &get_label_string(unsigned int label) const;
  unsigned int label_count(void) const;

private:
  typedef std::unordered_map<std::string, LabelVector> SubstringLabelMap;

  unsigned int max_suffix_len;
  std::unordered_map<std::string, unsigned int> label_map;
  StringVector string_map;
  SubstringLabelMap label_counts;
  SubstringLabelMap lexicon;
};

#endif // HEADER_LabelExtractor_hh
