#ifndef HEADER_Data_hh
#define HEADER_Data_hh

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Sentence.hh"
#include "LabelExtractor.hh"
#include "ParamTable.hh"

typedef std::vector<Sentence> SentenceVector;

struct Acc
{
  float label_acc;
  float iv_label_acc;
  float oov_label_acc;
  float lemma_acc;
  float iv_lemma_acc;
  float oov_lemma_acc;
};

struct IncompatibleData
{};

/*

  Input data format:

  Example:

    These	WORD=The SF=e	the	DT	_
    dogs	WORD=dog SF=g	dog	NN	_
    sleep	WORD=sleep SF=p	sleep	VBN	_

    Those	WORD=Those SF=e	those	PRON	_
    don't	WORD=don't SF=t	do	VBN	_

  Each line should be empty or should have the form:

  "TOKEN\tFEAT1 ... FEATm\tLEMMA\tLABEL1 ... LABELn\tANNOTATIONS"

  TOKEN is a non-empty word form, punctuation or such.
  FEATi is a non-empty features template name
  LEMMA can be a word or "_" for empty.
  LABELi can be a word or "_" for empty.
  ANNOTATONS are additional annotations or "_" for empty.

  None of the fields can contain tabs. Moreover, FEATi and LABELi
  can't contain spaces.

  Two sentences are separated by an empty line.
 */

class Data
{
 public:
  static bool silent;
 
  Data(const std::string &fn, bool is_gold, LabelExtractor &extractor, ParamTable &pt, unsigned int degree);
  Data(std::istream &in, bool is_gold, LabelExtractor &extractor, ParamTable &pt, unsigned int degree);

  const Sentence &at(unsigned int i) const;
  Sentence &at(unsigned int i);

  unsigned int size(void) const;

  void set_label_guesses(const LabelExtractor &g, 
			 unsigned int count);

  void predict_lemma(LemmaExtractor &g, const LabelExtractor &e);

  void unset_lemma(void);
  void unset_label(void);

  Acc get_acc(const Data &other, const LemmaExtractor &lemma_extractor) const;

 private:
  SentenceVector data;

  void init(const std::string &fn, std::istream &in, bool is_gold, LabelExtractor &extractor, ParamTable &pt, unsigned int degree);
};

#endif // HEADER_Data_hh
