#include "TaggerOptions.hh"
#include "exceptions.hh"

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

TaggerOptions::TaggerOptions(std::istream &in, unsigned int &counter):
  estimator(AVG_PERC),
  inference(MAP),
  suffix_length(10),
  degree(2),
  max_train_passes(50),
  max_useless_passes(3),
  guess_count(10),
  beam(20),
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
	 empty_options.beam == 20              &&
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
}

#endif // TEST_TaggerOptions_cc
