///////////////////////////////////////////////////////////////////////////////
// @file    Tagger.cc                                                        //
// @Author  Miikka Silfverberg                                               //
// @brief   Class that can label sentences.                                  //
///////////////////////////////////////////////////////////////////////////////

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

#include "Tagger.hh"

#ifndef TEST_Tagger_cc

#include "PerceptronTrainer.hh"
#include "SGDTrainer.hh"
#include "Trellis.hh"

Tagger::Tagger(const TaggerOptions &tagger_options, 
	       std::ostream &msg_out):
  line_counter(0),
  tagger_options(tagger_options),
  label_extractor(tagger_options.suffix_length),
  msg_out(msg_out)
{
  if (tagger_options.estimator == AVG_PERC)
    {
      if (tagger_options.regularization != NONE or 
	  tagger_options.delta != -1            or 
	  tagger_options.sigma != -1)
	{ 
	  msg_out << "Warning! Averaged perceptron doesn't utilize delta," 
		  << std::endl
		  << "sigma or regularization. Options will be discarded." 
		  << std::endl 
		  << std::endl;
	}
    }
}

Tagger::Tagger(std::istream &tagger_opt_in, std::ostream &msg_out):
  line_counter(0),
  tagger_options(tagger_opt_in, line_counter),
  label_extractor(tagger_options.suffix_length),
  msg_out(msg_out)
{
  if (tagger_options.estimator == AVG_PERC)
    {
      if (tagger_options.regularization != NONE or 
	  tagger_options.delta != -1            or 
	  tagger_options.sigma != -1)
	{ 
	  msg_out << "Warning! Averaged perceptron doesn't utilize delta," 
		  << std::endl
		  << "sigma or regularization. Options will be discarded." 
		  << std::endl 
		  << std::endl;
	}
    }

  line_counter = 0;
}


void Tagger::train(std::istream &train_in,
		   std::istream &dev_in)
{
  msg_out << "Reading training data." << std::endl;
  Data train_data(train_in, 1, label_extractor, param_table, tagger_options.degree);
  
  msg_out << "Training label guesser." << std::endl;
  label_extractor.train(train_data);

  msg_out << "Reading development data." << std::endl;
  Data dev_data(dev_in, 1, label_extractor, param_table, tagger_options.degree);

  msg_out << "Setting label guesses." << std::endl;
  train_data.set_label_guesses(label_extractor, 0, tagger_options.guess_count);
  dev_data.set_label_guesses(label_extractor, 0, tagger_options.guess_count);
  
  msg_out << "Estimating lemmatizer parameters." << std::endl;
  lemma_extractor.train(train_data, dev_data, label_extractor, msg_out);

  msg_out << "Estimating tagger parameters." << std::endl;

  if (tagger_options.estimator == AVG_PERC)
    {
      PerceptronTrainer trainer(tagger_options.max_train_passes, 
				tagger_options.max_useless_passes, 
				param_table, 
				label_extractor.get_boundary_label(), 
				lemma_extractor,
				msg_out);

      trainer.train(train_data, dev_data, tagger_options.beam);
    }
  else
    { throw NotImplemented(); }
}

StringVector Tagger::label(std::istream &in)
{
  while (1)
    {
      if (not in)
	{ return StringVector(); }

      Sentence s(in, 
		 0, 
		 label_extractor, 
		 param_table, 
		 tagger_options.degree,
		 line_counter);

      s.unset_label();

      if (s.size() > 0)
	{ return label(s); }
    }
}

StringVector Tagger::label(Sentence &s)
{
  s.set_label_guesses(label_extractor, 1, tagger_options.guess_count);
  Trellis trellis(s, label_extractor.get_boundary_label(), tagger_options.beam);
  trellis.set_maximum_a_posteriori_assignment(param_table);

  return labels_to_strings(trellis.get_maximum_a_posteriori_assignment(param_table));
}

StringVector Tagger::labels_to_strings(const LabelVector &v)
{
  StringVector res;

  for (unsigned int i = 0; i < v.size(); ++i)
    { 
      if (v[i] != label_extractor.get_boundary_label())
	{
	  res.push_back(label_extractor.get_label_string(v[i])); 
	}
    }

  return res;
}

#include <cassert>

void Tagger::store(std::ostream &out) const
{
  // FIXME
  static_cast<void>(out);
  assert(0);
}

#else // TEST_Tagger_cc
#include <sstream>
#include <cassert>

int main(void)
{
  std::string train_contents("\n"
			     "The\tWORD=The\tthe\tDT\t_\n"
			     "dog\tWORD=dog SUF=og\tdog\tNN\t_\n"
			     ".\tWORD=.\t.\t.\t_\n"
			     "\n"
			     "\n"
			     "The\tWORD=The\tthe\tDT\t_\n"
			     "dog\tWORD=dog SUF=og\tdog\tNN\t_\n"
			     ".\tWORD=.\t.\t.\t_\n");
  
  std::istringstream train_in(train_contents);

  std::string dev_contents("\n"
			   "The\tWORD=The\tthe\tDT\t_\n"
			   "dog\tWORD=dog SUF=og\tdog\tNN\t_\n"
			   ".\tWORD=.\t.\t.\t_\n"
			   "\n"
			   "\n"
			   "The\tWORD=The\tthe\tDT\t_\n"
			   "dog\tWORD=dog SUF=og\tdog\tNN\t_\n"
			   ".\tWORD=.\t.\t.\t_\n");
  
  std::istringstream dev_in(dev_contents);

  TaggerOptions tagger_options(AVG_PERC, MAP, 10, 2, 20, 3, 20, -1, NONE, 0, 0);
  std::ostringstream null_stream;
  Tagger tagger(tagger_options, null_stream);
  
  tagger.train(train_in, dev_in);

  std::string test_contents("\n"
			    "The\tWORD=The\tthe\tNN DT\t_\n"
			    "hog\tWORD=hog SUF=og\thog\tNN DT\t_\n"
			    ".\tWORD=.\t.\t.\t_\n"
			    "\n"
			    "\n"
			    "The\tWORD=The\tthe\tNN DT\t_\n"
			    "hog\tWORD=hog SUF=og\thog\tNN DT\t_\n"
			    ".\tWORD=.\t.\t.\t_\n");
  
  std::istringstream test_in(test_contents);

  StringVector labels = tagger.label(test_in);

  assert(labels[0] == "DT");
  assert(labels[1] == "NN");
  assert(labels[2] == ".");

  labels = tagger.label(test_in);

  assert(labels[0] == "DT");
  assert(labels[1] == "NN");
  assert(labels[2] == ".");
}

#endif // TEST_Tagger_cc
