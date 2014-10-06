/**
 * @file    Tagger.cc                                                        
 * @Author  Miikka Silfverberg                                               
 * @brief   Class that can label a Sentence.                                  
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

#include "Tagger.hh"

#ifndef TEST_Tagger_cc

#include <sstream>

#include "PerceptronTrainer.hh"
#include "SGDTrainer.hh"
#include "Trellis.hh"

#define FINN_POS_ID_STRING "FinnPosModel"
const int ENDIANNESS_MARKER=1;

Tagger::Tagger(std::ostream &msg_out):
  msg_out(msg_out)
{}

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

#include <cassert>

void Tagger::train(std::istream &train_in,
		   std::istream &dev_in)
{
  msg_out << "Reading training data." << std::endl;
  Data train_data(train_in, 1, label_extractor, param_table, tagger_options.degree);
  train_data.clear_label_guesses(); 
    
  msg_out << "Training label guesser." << std::endl;
  label_extractor.train(train_data);

  msg_out << "Reading development data." << std::endl;
  Data dev_data(dev_in, 1, label_extractor, param_table, tagger_options.degree);
  dev_data.clear_label_guesses(); 

  msg_out << "Setting label guesses." << std::endl;
  train_data.set_label_guesses(label_extractor, 0, tagger_options.guess_count);
  dev_data.set_label_guesses(label_extractor, 1, tagger_options.guess_count);
  
  msg_out << "Estimating lemmatizer parameters." << std::endl;
  lemma_extractor.train(train_data, dev_data, label_extractor, msg_out);

  msg_out << "Estimating tagger parameters." << std::endl;
  //param_table.set_trained();

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

void Tagger::evaluate(std::istream &in)
{
  msg_out << "Evaluating." << std::endl;
  Data data(in, 1, label_extractor, param_table, tagger_options.degree);
  data.clear_label_guesses(); 

  Data data_copy(data);
  data_copy.unset_label();

  data_copy.set_label_guesses(label_extractor, 1, tagger_options.guess_count);

  TrellisVector trellises;
  
  for (unsigned int i = 0; i < data.size(); ++i)
    {
      trellises.push_back(Trellis(data_copy.at(i), label_extractor.get_boundary_label(), tagger_options.beam));
    }

  // Tag test data.
  for (unsigned int j = 0; j < trellises.size(); ++j)
    {
      trellises[j].set_maximum_a_posteriori_assignment(param_table);
    }
  
  float acc1 = data.get_acc(data_copy, lemma_extractor).label_acc;
  
  msg_out << "  Final test acc: " << acc1 * 100.00 << "%" << std::endl;
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

void Tagger::label(Data &data)
{
  //data.set_label_guesses(label_extractor, 1, tagger_options.guess_count);

  for (unsigned int i = 0; i < data.size(); ++i)
    { label(data.at(i)); }
}

StringVector Tagger::label(Sentence &s)
{
  s.unset_label();
  s.clear_label_guesses();
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
  msg_out << "Storing model." << std::endl;

  std::ostringstream str_out;

  tagger_options.store(str_out);
  label_extractor.store(str_out);
  lemma_extractor.store(str_out);
  param_table.store(str_out);

  std::string binary = str_out.str();

  write_val<std::string>(out, FINN_POS_ID_STRING);
  write_val<int>(out, ENDIANNESS_MARKER);
  write_val<unsigned int>(out, binary.size());
  write_val(out, binary);
}

void Tagger::load(std::istream &in)
{
  msg_out << "Loading model." << std::endl;

  std::string id_string = read_val<std::string>(in, false);

  if (id_string != FINN_POS_ID_STRING)
    { 
      throw BadBinary();
    }

  bool reverse_bytes = not homoendian(in, ENDIANNESS_MARKER);

  // Read file contents into stringstream.
  std::istringstream str_in;
  init_string_stream(in, str_in, reverse_bytes);

  tagger_options.load(str_in, msg_out, reverse_bytes);
  label_extractor.load(str_in, reverse_bytes);
  lemma_extractor.load(str_in, reverse_bytes);
  param_table.load(str_in, reverse_bytes); 
}

bool Tagger::operator==(const Tagger &another) const
{
  if (this == &another)
    { return 1; }
  
  return
    (tagger_options == another.tagger_options   and
     label_extractor == another.label_extractor and
     lemma_extractor == another.lemma_extractor and
     param_table == another.param_table);
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

  std::ostringstream tagger_out;
  tagger.store(tagger_out);
  std::istringstream tagger_in(tagger_out.str());
  Tagger tagger_copy(null_stream);
  tagger_copy.load(tagger_in);
  assert(tagger == tagger_copy);
}

#endif // TEST_Tagger_cc
