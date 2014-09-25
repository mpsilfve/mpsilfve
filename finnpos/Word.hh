#ifndef HEADER_Word_hh
#define HEADER_Word_hh

#include <vector>
#include <string>
#include <exception>

#include "LabelExtractor.hh"
#include "LemmaExtractor.hh"
#include "ParamTable.hh"

#define BOUNDARY_WF "_#_"

#define NO_STRING ""
#define NO_LABEL -1

struct NoLabel : public std::exception
{};

class Word
{
public:
  // Initialize to boundary words
  Word(unsigned int boundary_label);
  
  // Initialize using a label list.
  Word(const std::string &word_form,
       const FeatureTemplateVector &feature_templates,
       const LabelVector &labels,
       const std::string &annotations);

  void set_label_guesses(const LabelExtractor &g, unsigned int count);
  void predict_lemma(LemmaExtractor &g, const LabelExtractor &e);

  void set_lemma(const std::string &lemma);
  void set_label(unsigned int label);

  unsigned int get_feature_template_count(void) const;
  unsigned int get_feature_template(unsigned int feature_template_index) const;

  unsigned int get_label_count(void) const;
  unsigned int get_label(unsigned int label_index) const;

  unsigned int get_label(void) const;

  std::string get_lemma(void) const;

  std::string get_word_form(void) const;

  void unset_lemma(void);
  void unset_label(void);

 private:
  std::string           word_form;
  std::string           lemma;
  std::string           annotations;

  unsigned int          label;
  
  LabelVector           label_candidates;
  FeatureTemplateVector feature_templates;
};

#endif // HEADER_Word_hh
