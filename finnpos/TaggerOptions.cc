#include "TaggerOptions.hh"

#ifndef TEST_TaggerOptions_cc

TaggerOptions::TaggerOptions(Estimator estimator, 
			     Inference inference, 
			     unsigned int suffix_length, 
			     unsigned int degree,
			     unsigned int max_train_passes,
			     unsigned int max_useless_passes,
			     unsigned int guess_count,
			     unsigned int beam,
			     Regularization regularization,
			     float delta,
			     float sigma):
  estimator(estimator),
  inference(inference),
  suffix_length(suffix_length),
  degree(degree),
  max_train_passes(max_train_passes),
  max_useless_passes(max_useless_passes),
  guess_count(guess_count),
  beam(beam),
  regularization(regularization),
  delta(delta),
  sigma(sigma)
{
}

#else // TEST_TaggerOptions_cc

int main(void)
{

}

#endif // TEST_TaggerOptions_cc
