#include "Trainer.hh"

#ifndef TEST_Trainer_cc

Trainer::Trainer(unsigned int max_passes,
		 unsigned int max_useless_passes,
		 ParamTable &pt,
		 unsigned int boundary_label,
		 const LemmaExtractor &lemma_extractor):
  max_passes(max_passes),
  max_useless_passes(max_useless_passes),
  pt(pt),
  boundary_label(boundary_label),
  lemma_extractor(lemma_extractor)
{}

#else // TEST_Trainer_cc

int main (void)
{

}

#endif // TEST_Trainer_cc
