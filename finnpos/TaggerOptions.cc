/**
 * @file    TaggerOptions.cc                                                 
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

#include "TaggerOptions.hh"
#include "exceptions.hh"
#include "io.hh"

std::string strip(const std::string &str, const std::string &prefix); 
Estimator get_estimator(const std::string &str);
Inference get_inference(const std::string &str);
Regularization get_regularization(const std::string &str);
unsigned int get_uint(const std::string &str); 
unsigned int get_float(const std::string &str);

#ifndef TEST_TaggerOptions_cc

#include <string>
#include <cassert>
#include <cstdlib>

const char * estimator_id = "estimator=";
const char * inference_id = "inference=";
const char * suffix_length_id = "suffix_length=";
const char * degree_id = "degree=";
const char * max_train_passes_id = "max_train_passes=";
const char * max_useless_passes_id = "max_useless_passes=";
const char * guess_count_id = "guess_count=";
const char * beam_id = "beam=";
const char * beam_mass_id = "beam_mass=";
const char * regularization_id = "regularization=";
const char * delta_id = "delta=";
const char * sigma_id = "sigma=";

std::string despace(const std::string &line)
{
  std::string res; 

  for (unsigned int i = 0; i < line.size(); ++i)
    { 
      if (line[i] != ' ' and line[i] != '\t' and line[i] != '\r')
	{ res += line[i]; }
    }

  return res;
}

TaggerOptions::TaggerOptions(void)
{}

TaggerOptions::TaggerOptions(Estimator estimator, 
			     Inference inference, 
			     unsigned int suffix_length, 
			     unsigned int degree,
			     unsigned int max_train_passes,
			     unsigned int max_useless_passes,
			     unsigned int guess_count,
			     unsigned int beam,
			     float beam_mass,
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
  beam_mass(beam_mass),
  regularization(regularization),
  delta(delta),
  sigma(sigma)
{
}

TaggerOptions::TaggerOptions(std::istream &in, unsigned int &counter):
  estimator(AVG_PERC),
  inference(MAP),
  suffix_length(10),
  degree(2),
  max_train_passes(50),
  max_useless_passes(3),
  guess_count(10),
  beam(-1),
  beam_mass(-1),
  regularization(NONE),
  delta(-1),
  sigma(-1)
{
  while (in)
    {
      std::string line;
      
      std::getline(in, line);

      line = despace(line);
 
      ++counter;

      if (line.empty())
	{ continue; }

      if (line.find(estimator_id) != std::string::npos)
	{ estimator = get_estimator(strip(line, estimator_id)); }
      else if (line.find(inference_id) != std::string::npos)
	{ inference = get_inference(strip(line, inference_id)); }
      else if (line.find(suffix_length_id) != std::string::npos)
	{ suffix_length = get_uint(strip(line, suffix_length_id)); }
      else if (line.find(degree_id) != std::string::npos)
	{ degree = get_uint(strip(line, degree_id)); }
      else if (line.find(max_train_passes_id) != std::string::npos)
	{ max_train_passes = get_uint(strip(line, max_train_passes_id)); }
      else if (line.find(max_useless_passes_id) != std::string::npos)
	{ max_useless_passes = get_uint(strip(line, max_useless_passes_id)); }
      else if (line.find(guess_count_id) != std::string::npos)
	{ guess_count = get_uint(strip(line, guess_count_id)); }
      else if (line.find(beam_id) != std::string::npos)
	{ beam = get_uint(strip(line, beam_id)); }
      else if (line.find(beam_mass_id) != std::string::npos)
	{ beam_mass = get_uint(strip(line, beam_mass_id)); }
      else if (line.find(regularization_id) != std::string::npos)
	{ regularization = get_regularization(strip(line, regularization_id)); }
      else if (line.find(delta_id) != std::string::npos)
	{ delta = get_float(strip(line, delta_id)); }
      else if (line.find(sigma_id) != std::string::npos)
	{ sigma = get_float(strip(line, sigma_id)); }
      else
	{ throw SyntaxError(); }
    }
}

void TaggerOptions::store(std::ostream &out) const
{
  std::vector<std::string> field_names;
  std::vector<int>         fields;

  field_names.push_back("estimator");
  field_names.push_back("inference");
  field_names.push_back("suffix_length");
  field_names.push_back("degree");
  field_names.push_back("max_train_passes");
  field_names.push_back("max_useless_passes");
  field_names.push_back("guess_count");
  field_names.push_back("beam");
  field_names.push_back("beam_mass");
  field_names.push_back("regularization");
  field_names.push_back("delta");
  field_names.push_back("sigma");

  fields.push_back(static_cast<int>(estimator));
  fields.push_back(static_cast<int>(inference));
  fields.push_back(static_cast<int>(suffix_length));
  fields.push_back(static_cast<int>(degree));
  fields.push_back(static_cast<int>(max_train_passes));
  fields.push_back(static_cast<int>(max_useless_passes));
  fields.push_back(static_cast<int>(guess_count));
  fields.push_back(static_cast<int>(beam));
  fields.push_back(static_cast<int>(beam_mass));
  fields.push_back(static_cast<int>(regularization));
  fields.push_back(static_cast<int>(delta));
  fields.push_back(static_cast<int>(sigma));
  
  write_vector(out, field_names);
  write_vector(out, fields);
}

void TaggerOptions::load(std::istream &in, std::ostream &msg_out, bool reverse_bytes)
{
  std::vector<std::string> field_names;
  std::vector<int>         fields;

  read_vector(in, field_names, reverse_bytes);
  read_vector(in, fields, reverse_bytes);

  if (in.fail())
    { throw ReadFailed(); }

  if (field_names.size() != fields.size())
    { throw BadBinary(); }

  for (unsigned int i = 0; i < field_names.size(); ++i)
    {
      if (field_names[i] == "estimator")
	{ estimator = static_cast<Estimator>(fields[i]); }
      else if (field_names[i] == "inference")
	{ inference = static_cast<Inference>(fields[i]); }
      else if (field_names[i] == "suffix_length")
	{ suffix_length = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "degree")
	{ degree = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "max_train_passes")
	{ max_train_passes = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "max_useless_passes")
	{ max_useless_passes = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "guess_count")
	{ guess_count = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "beam")
	{ beam = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "beam_mass")
	{ beam_mass = static_cast<unsigned int>(fields[i]); }
      else if (field_names[i] == "regularization")
	{ regularization = static_cast<Regularization>(fields[i]); }
      else if (field_names[i] == "delta")
	{ delta = static_cast<float>(fields[i]); }
      else if (field_names[i] == "sigma")
	{ sigma = static_cast<float>(fields[i]); }
      else
	{
	  msg_out << "Found unknown parameter name " 
		  << field_names[i] << ". "
		  << "Please, update your FinnPos version."
		  << std::endl;
	}
    }
}

std::string strip(const std::string &str, const std::string &prefix)
{
  size_t prefix_start = str.find(prefix);

  assert(prefix_start != std::string::npos);

  return str.substr(prefix_start + prefix.size());
}

Estimator get_estimator(const std::string &str)
{
  if (str.find("AVG_PERC") == 0)
    { return AVG_PERC; }
  else if (str.find("ML") == 0)
    { return ML; }
  else
    { throw SyntaxError(); }
}

Inference get_inference(const std::string &str)
{
  if (str.find("MAP") == 0)
    { return MAP; }
  else if (str.find("MARGINAL") == 0)
    { return MARGINAL; }
  else
    { throw SyntaxError(); }
}

Regularization get_regularization(const std::string &str)
{
  if (str.find("NONE") == 0)
    { return NONE; }
  else if (str.find("L1") == 0)
    { return L1; }
  else if (str.find("L2") == 0)
    { return L2; }
  else
    { throw SyntaxError(); }
}

unsigned int get_uint(const std::string &str)
{ 
  int i = atoi(str.c_str());
  
  if (i < 0)
    { throw NumericalRangeError(); }

  return static_cast<unsigned int>(i); 
}

unsigned int get_float(const std::string &str)
{
  float f = atof(str.c_str());
  
  if (f < 0)
    { throw NumericalRangeError(); }

  return f; 
}

bool TaggerOptions::operator==(const TaggerOptions &another) const
{
  if (this == &another)
    { return 1; }

  return 
    (estimator == another.estimator               and
     inference == another.inference               and
     suffix_length == another.suffix_length       and
     degree == another.degree                     and
     max_train_passes == another.max_train_passes and
     guess_count == another.guess_count           and
     beam == another.beam                         and
     beam_mass == another.beam_mass               and
     regularization == another.regularization     and
     delta == another.delta                       and
     sigma == another.sigma);
}

#else // TEST_TaggerOptions_cc

#include <sstream>
#include <cassert>

int main(void)
{
  std::string empty_opt_str;
  std::istringstream empty_opt_file(empty_opt_str);

  unsigned int counter;

  TaggerOptions empty_options(empty_opt_file,counter);
  
  assert(empty_options.estimator == AVG_PERC   &&
	 empty_options.inference == MAP        &&
	 empty_options.suffix_length == 10     &&
	 empty_options.degree == 2             &&
	 empty_options.max_train_passes == 50  &&
	 empty_options.max_useless_passes == 3 &&
	 empty_options.guess_count == 10       &&
	 empty_options.beam == static_cast<unsigned int>(-1)              &&
	 empty_options.regularization == NONE  &&
	 empty_options.delta == -1             &&
	 empty_options.sigma == -1);

  counter = 0;

  std::string opt_str = 
    "sigma = 1\n"
    "delta = 2\n"
    "regularization=L1\n"
    "beam=3\n"
    "guess_count=4\n"
    "max_useless_passes =5\n"
    "max_train_passes =6\n"
    "degree =7\n"
    "suffix_length =8\n"
    "inference =MARGINAL\n"
    "estimator=ML\n"
    "beam_mass=6"
    ;

  std::istringstream opt_file(opt_str);
  TaggerOptions options(opt_file,counter);

  assert(options.estimator == ML);
  assert(options.inference == MARGINAL);
  assert(options.suffix_length == 8);
  assert(options.degree == 7);
  assert(options.max_train_passes == 6);
  assert(options.max_useless_passes == 5);
  assert(options.guess_count == 4);
  assert(options.beam == 3);
  assert(options.regularization == L1);
  assert(options.delta == 2);
  assert(options.sigma == 1);
  assert(options.beam_mass == 6);
  counter = 0;

  try
    {
    std::string err_opt_str = 
      "sigma = 1\n"
      "delta = 2\n"
      "regularization=L1\n"
      "beam=3\n"
      "guess_count=4\n"
      "max_useless_passeSSS =5\n"
      "max_train_passes =6\n"
      "degree =7\n"
      "suffix_length =8\n"
      "inference =MARGINAL\n"
      "estimator=ML\n"
      ;

    std::istringstream err_opt_file(err_opt_str);
    TaggerOptions err_options(err_opt_file,counter);
    assert(0);
    }
  catch (const SyntaxError &e)
    {
      assert(counter == 6);
    } 

  std::ostringstream opt_out;
  options.store(opt_out);
  std::istringstream opt_in(opt_out.str());
  TaggerOptions options_copy;
  options_copy.load(opt_in, std::cerr, false);
  assert(options == options_copy);
  
}

#endif // TEST_TaggerOptions_cc
