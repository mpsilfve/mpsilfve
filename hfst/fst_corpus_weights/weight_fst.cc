#include <iostream>

#include <hfst/HfstTransducer.h>

#include "path_utils.hh"
#include "StreamReader.hh"
#include "string_utils.hh"

using hfst::HfstInputStream;
using hfst::HfstOutputStream;
using hfst::HfstTransducer;
using hfst::implementations::HfstBasicTransducer;
using hfst::TROPICAL_OPENFST_TYPE;
using hfst::HfstTokenizer;
using hfst::HfstTwoLevelPath;
using hfst::HfstTokenizer;

void weight_string_pair(const StringVector &fields, 
			HfstBasicTransducer &bfst, 
			const HfstTransducer &fst,
			const HfstTokenizer &tok)
{
  const std::string &inputstr  = fields[0];
  const std::string &outputstr = fields[1];

  HfstOneLevelPaths * paths = fst.lookup_fd(inputstr);
  HfstOneLevelPath match;
  if (not get_first_match(*paths, outputstr, match))
    { 
      std::cerr << "String pair \"" << inputstr << ":" << outputstr << "\""
		<< " is not recognized." << std::endl
		<< "Skipping!" << std::endl << std::endl; 
      return;
    }
  delete paths;

  // Tokenize input string into utf8 symbols.
  StringVector inputtok = tok.tokenize_one_level(inputstr);

  // Get the path of symbol pairs corresponding to the input and
  // output string.
  HfstTwoLevelPath aligned_path;
  align_paths(HfstOneLevelPath(0, inputtok), match, bfst, aligned_path);

  // Incement the weight of all transitions on the path as well as the
  // final state.
  incr_path_weight(aligned_path, bfst);
}

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cerr << "USAGE: cat string_pairs | " 
		<< argv[0] << " input_fst_file output_fst_file"
		<< std::endl;
      exit(1);
    }

  std::cerr << "Reading input fst from " << argv[1] << "."
	    << std::endl;
  HfstInputStream in(argv[1]);
  HfstTransducer fst(in);
  // FIXME: Doesn't know how to tokenize input multichar symbols!
  HfstTokenizer tok;
  HfstBasicTransducer bfst(fst);

  StreamReader stdin_reader;
  std::cerr << "Reading string pairs from STDIN."
	    << std::endl;
  while(stdin_reader.readline())
    {
      const std::string &line = stdin_reader.getline();

      StringVector fields;
      split(line, "\t", fields);
      
      if (fields.size() != 2)
	{
	  std::cerr << "Syntax error on line " << stdin_reader.getlinenumber()
		    << std::endl;
	  exit(1);
	}

      // Skip input strings which weren't recognized.
      if (fields[1] == "+?")
	{ continue; }

      weight_string_pair(fields, bfst, fst, tok);
    }

  std::cerr << "Writing weighted fst to " << argv[2] << std::endl;
  HfstTransducer res(bfst, TROPICAL_OPENFST_TYPE);
  HfstOutputStream out(argv[2], TROPICAL_OPENFST_TYPE);
  out << res;
}
