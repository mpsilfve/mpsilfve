#ifndef HEADER_Sentence_hh
#define HEADER_Sentence_hh

#include <vector>
#include <fstream>
#include <iostream>

#include "Word.hh"
#include "ParamTable.hh"

typedef std::vector<Word> WordVector;

class Sentence
{
 public:

  Sentence(const WordVector &words, 
	   LabelExtractor &label_extractor, 
	   unsigned int degree);

  Sentence(std::istream &ifile, 
	   bool is_gold, 
	   LabelExtractor &label_extractor, 
	   ParamTable &pt, 
	   unsigned int degree, 
	   unsigned int &line_counter);

  const Word &at(unsigned int i) const;
  Word &at(unsigned int i);
  unsigned int size(void) const;
  unsigned int get_max_label_count(void) const;
  void set_label_guesses(const LabelExtractor &g, 
			 unsigned int count);
  void predict_lemma(LemmaExtractor &g, const LabelExtractor &e);

  void unset_lemma(void);
  void unset_label(void);

 private:
  WordVector sentence;
};

#endif // HEADER_Sentence_hh
