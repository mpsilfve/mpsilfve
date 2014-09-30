#include "TaggerOptions.hh"

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

#include "exceptions.hh"

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
      
      if (line.find("estimator=") != std::string::npos)
	{ estimator = get_estimator(strip(line, "estimator=")); }
      else if (line.find("inference=") != std::string::npos)
	{ inference = get_inference(strip(line, "inference=")); }
      else if (line.find("suffix_length=") != std::string::npos)
	{ suffix_length = get_uint(strip(line, "suffix_length=")); }
      else if (line.find("degree=") != std::string::npos)
	{ degree = get_uint(strip(line, "degree=")); }
      else if (line.find("max_train_passes=") != std::string::npos)
	{ max_train_passes = get_uint(strip(line, "max_train_passes=")); }
      else if (line.find("max_useless_passes=") != std::string::npos)
	{ max_useless_passes = get_uint(strip(line, "max_useless_passes=")); }
      else if (line.find("guess_count=") != std::string::npos)
	{ guess_count = get_uint(strip(line, "guess_count=")); }
      else if (line.find("beam=") != std::string::npos)
	{ beam = get_uint(strip(line, "beam=")); }
      else if (line.find("regularization=") != std::string::npos)
	{ regularization = get_regularization(strip(line, "regularization=")); }
      else if (line.find("delta=") != std::string::npos)
	{ delta = get_float(strip(line, "delta=")); }
      else if (line.find("sigma=") != std::string::npos)
	{ sigma = get_float(strip(line, "sigma=")); }
      else
	{ throw SyntaxError(); }
      
      ++counter;
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

int main(void)
{

}

#endif // TEST_TaggerOptions_cc
