#ifndef HEADER_path_utils_hh
#define HEADER_path_utils_hh

#include <hfst/HfstTransducer.h>

using hfst::HfstOneLevelPath;
using hfst::HfstOneLevelPaths;
using hfst::HfstTwoLevelPaths;
using hfst::HfstTwoLevelPath;
using hfst::HfstBasicTransducer;
using hfst::StringVector;
using hfst::implementations::HfstState;

void getstring(const HfstOneLevelPath &p, std::string &s);

bool get_first_match(HfstOneLevelPaths &paths, 
		     const std::string &string, 
		     HfstOneLevelPath &match);

bool align_paths(const HfstOneLevelPath &in, 
		 const HfstOneLevelPath &out, 
		 const HfstBasicTransducer &b,
		 HfstTwoLevelPath &aligned);

bool align_paths(StringVector::const_iterator it, 
		 StringVector::const_iterator iend,
		 StringVector::const_iterator ot, 
		 StringVector::const_iterator oend,
		 HfstState s,
		 const HfstBasicTransducer &b,
		 HfstTwoLevelPath &aligned);

void incr_path_weight(HfstTwoLevelPath &aligned_path, 
		      HfstBasicTransducer &bfst);

bool is_matching_pair(const std::string &in1, const std::string &in2,
		      const std::string &out1, const std::string &out2);

#endif // HEADER_path_utils_hh
