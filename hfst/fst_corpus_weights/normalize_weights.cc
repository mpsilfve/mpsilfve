#include <iostream>
#include <cstdlib>

#include <hfst/HfstTransducer.h>

#include "weight_utils.hh"

using hfst::HfstBasicTransducer;
using hfst::HfstTransducer;
using hfst::HfstInputStream;
using hfst::HfstOutputStream;
using hfst::TROPICAL_OPENFST_TYPE;

#define ADD_ALPHA 1

int main(int argc, char * argv[])
{
  if (argc != 2)
    {
      std::cerr << "USAGE: " << argv[0] << " in_fst_file out_fst_file"
		<< std::endl;
      exit(1);
    }

  hfst::HfstInputStream in(argv[1]);
  hfst::HfstTransducer t(in);
  hfst::HfstBasicTransducer b(t);

  laplace_smooth(ADD_ALPHA, b);
  normalize_locally(b);
  prob2tropical(b);

  HfstTransducer tt(b, TROPICAL_OPENFST_TYPE);
  hfst::HfstOutputStream out(argv[2], TROPICAL_OPENFST_TYPE);
  out << tt;
}
