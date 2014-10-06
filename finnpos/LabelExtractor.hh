/**
 * @file    LabelExtractor.hh                                                
 * @Author  Miikka Silfverberg                                               
 * @brief   Suffix-based label guesser                                       
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

#ifndef HEADER_LabelExtractor_hh
#define HEADER_LabelExtractor_hh

#include <string>
#include <vector>
#include <unordered_map>


#include "io.hh"
#include "exceptions.hh"

typedef std::vector<unsigned int> LabelVector;

class Data;

class LabelExtractor
{
 public:
  LabelExtractor(unsigned int max_suffix_len=10);
  virtual ~LabelExtractor(void);

  virtual void set_label_candidates(const std::string &word_form,
				    bool use_lexicon,
				    unsigned int count, 
				    LabelVector &target) const;
  void train(Data &data);
  unsigned int get_boundary_label(void) const;
  unsigned int get_label(const std::string &label_string);
  LabelVector get_labels(const StringVector &label_strings);
  const std::string &get_label_string(unsigned int label) const;
  unsigned int label_count(void) const;

  void store(std::ostream &out) const;

  void load(std::istream &in, bool reverse_bytes);

  bool operator==(const LabelExtractor &another) const;
private:
  typedef std::unordered_map<std::string, LabelVector> SubstringLabelMap;

  unsigned int max_suffix_len;
  std::unordered_map<std::string, unsigned int> label_map;
  StringVector string_map;
  SubstringLabelMap label_counts;
  SubstringLabelMap lexicon;
};

#endif // HEADER_LabelExtractor_hh
