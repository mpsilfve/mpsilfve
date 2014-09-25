#ifndef HEADER_LabelGuesser_hh
#define HEADER_LabelGuesser_hh

#include <string>
#include <vector>
#include <unordered_map>
#include <exception>

#include "io.hh"
#include "LabelExtractor.hh"

typedef std::vector<unsigned int> LabelVector;

struct IllegalLabel : public std::exception
{};

class LabelGuesser : public LabelExtractor
{
 public:
  LabelGuesser(unsigned int suffix_length);
  ~LabelGuesser(void);
  
  void set_label_candidates(std::string &word_form, int count, LabelVector &target) const = 0;

private:
  unsigned int suffix_length;
};

#endif // HEADER_LabelGuesser_hh
