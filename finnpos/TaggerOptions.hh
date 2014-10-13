/**
 * @file    TaggerOptions.hh                                                 
 * @Author  Miikka Silfverberg                                               
 * @brief   Class for reading config files.                                  
 */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// (C) Copyright 2014, University of Helsinki                                //
// Licensed under the Apache License, Version 2.0 (the "License");           //
// you may not use this file except in compliance with the License.          //
// You may obtain a copy of the License at                                   //
// http://www.apache.org/licenses/LICENSE-2.0                                //
// Unless required by applicable law or agreed to in writing, software       //
// distributed under the License is distributed on an "AS IS" BASIS,         //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  //
// See the License for the specific language governing permissions and       //
// limitations under the License.                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

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
  Estimator estimator;
  Inference inference;
  unsigned int suffix_length;
  unsigned int degree;
  unsigned int max_train_passes;
  unsigned int max_useless_passes;
  unsigned int guess_count;
  unsigned int beam;
  float beam_mass;
  Regularization regularization;
  float delta;
  float sigma;

  TaggerOptions(void);

  TaggerOptions(Estimator estimator, 
		Inference inference, 
		unsigned int suffix_length, 
		unsigned int degree,
		unsigned int max_train_passes,
		unsigned int max_useless_passes,
		unsigned int guess_count,
		unsigned int beam,
		float beam_mass,
		Regularization regularization = NONE,
		float delta = -1,
		float sigma = -1);
  
  TaggerOptions(std::istream &in, unsigned int &counter);

  void store(std::ostream &out) const;

  void load(std::istream &in, std::ostream &msg_out, bool reverse_bytes);

  bool operator==(const TaggerOptions &another) const;
};

#endif // HEADER_TaggerOptions_hh
