#ifndef HEADER_Trainer_hh
#define HEADER_Trainer_hh

#include "Trainer.hh"
#include "Data.hh"
#include "ParamTable.hh"

class Trainer
{
public:
  Trainer(unsigned int max_passes,
	  unsigned int max_useless_passes,
	  ParamTable &pt,
	  unsigned int boundary_label,
	  const LemmaExtractor &lemma_extractor);
  
  virtual void train(const Data &train_data, 
		     const Data &dev_data,
		     unsigned int beam) = 0;

  void train_lemmatizer(const Data &train_data, 
			const Data &dev_data,
			LemmaExtractor &lemma_e,
			const LabelExtractor &label_e);
  
protected:
  unsigned int max_passes;
  unsigned int max_useless_passes;

  ParamTable &pt;

  unsigned int boundary_label;
  const LemmaExtractor &lemma_extractor;
};

#endif // HEADER_Trainer_hh
