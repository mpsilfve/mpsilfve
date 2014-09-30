#ifndef HEADER_TaggerOptions_hh
#define HEADER_TaggerOptions_hh

#include <iostream>

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
  
  TaggerOptions(std::istream &in, unsigned int &counter);

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

#endif // HEADER_TaggerOptions_hh
