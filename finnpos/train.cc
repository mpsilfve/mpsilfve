#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "io.hh"
#include "Tagger.hh"

int main(int argc, char * argv[])
{
  if (argc != 5)
    {
      std::cerr <<  "USAGE: " << argv[0] << " config_file train_file dev_file"
		<< std::endl;

      exit(1);
    }

  std::string config_fn = argv[1];  
  std::string train_fn  = argv[2];  
  std::string dev_fn    = argv[3];  
  std::string output_fn = argv[4];  

  std::ifstream config_in(config_fn);
  std::ifstream train_in(train_fn);
  std::ifstream dev_in(dev_fn);

  std::ofstream out(output_fn);

  if (not check(config_fn, config_in, std::cerr) or
      not check(train_fn,   train_in, std::cerr) or
      not check(dev_fn,       dev_in, std::cerr) or
      not check(output_fn,       out, std::cerr))
    { exit(1); }

  Tagger tagger(config_in, std::cerr);
  tagger.train(train_in, dev_in);
  tagger.store(out);
}
