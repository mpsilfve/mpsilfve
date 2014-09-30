#ifndef HEADER_Tagger_hh
#define HEADER_Tagger_hh

#include <iostream>
#include <exception>

#include "Sentence.hh"
#include "Data.hh"
#include "ParamTable.hh"
#include "LabelExtractor.hh"
#include "LemmaExtractor.hh"

enum Estimator
  { AVG_PERC, ML };

enum Inference
  { MAP, MARGINAL };

enum Regularization
  { NONE, L1, L2 };

struct TaggerOptions
{
  TaggerOptions(Estimator estimator, 
		Inference inference, 
		unsigned int suffix_length, 
		unsigned int degree,
		unsigned int max_train_passes,
		unsigned int max_useless_passes,
		unsigned int guess_count,
		unsigned int beam,
		Regularization regularization = NONE,
		float delta = -1,
		float sigma = -1);

  Estimator estimator;
  Inference inference;
  unsigned int suffix_length;
  unsigned int degree;
  unsigned int max_train_passes;
  unsigned int max_useless_passes;
  unsigned int guess_count;
  unsigned int beam;
  Regularization regularization;
  float delta;
  float sigma;
};

struct NotImplemented : public std::exception
{};

class Tagger
{
public:
  Tagger(const TaggerOptions &tagger_options,
	 std::ostream &msg_out);

  Tagger(std::istream &in);

  void train(std::istream &train_in,
	     std::istream &dev_in);

  StringVector label(std::istream &in);
  StringVector label(Sentence &s);

private:
  unsigned int line_counter;

  TaggerOptions tagger_options;

  LabelExtractor label_extractor;
  LemmaExtractor lemma_extractor;

  ParamTable param_table;

  std::ostream &msg_out;

  StringVector labels_to_strings(const LabelVector &v);
};

#endif // HEADER_Tagger_hh
