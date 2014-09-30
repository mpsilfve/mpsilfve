#ifndef HEADER_PerceptronTrainer_hh
#define HEADER_PerceptronTrainer_hh

#include <exception>
#include <iostream>

#include "Trainer.hh"
#include "ParamTable.hh"
#include "Sentence.hh"
#include "Data.hh"

struct UnlabeledData : public std::exception
{};

class PerceptronTrainer : public Trainer
{
public:
  PerceptronTrainer(unsigned int max_passes,
		    unsigned int max_useless_passes,
		    ParamTable &pt,
		    unsigned int boundary_label,
		    const LemmaExtractor &le,
		    std::ostream &msg_out);
  
  void train(const Data &train_data, const Data &dev_data, 
	     unsigned int beam = -1);

  void train_lemmatizer(const Data &train_data, 
			const Data &dev_data,
			LemmaExtractor &lemma_e,
			const LabelExtractor &label_e);

private:
  float iter;

  ParamTable pos_params;
  ParamTable neg_params;

  void update(const Sentence &gold_s, 
	      const Sentence &sys_s);

  void lemmatizer_update(const Word &w, 
			 unsigned int sys_class, 
			 unsigned int gold_class,
			 LemmaExtractor &lemma_e,
			 const LabelExtractor &label_e);

  void set_avg_params(void);
};

#endif // HEADER_PerceptronTrainer_hh
