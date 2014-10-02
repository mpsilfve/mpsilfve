/**
 * @file    LabelExtractor.cc                                                
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

#include "LabelExtractor.hh"

std::string get_suffix(const std::string &wf, unsigned int length);

#ifndef TEST_LabelExtractor_cc

#include <algorithm>
#include <unordered_set>

#include "Data.hh"

#define PADDING "^^^^^^^^^^" 
#define PADDING_LEN 10 

LabelExtractor::LabelExtractor(unsigned int max_suffix_len):
  max_suffix_len(max_suffix_len)
{
  static_cast<void>(get_label("_#_"));
}

LabelExtractor::~LabelExtractor(void)
{}

unsigned int LabelExtractor::get_boundary_label(void) const
{ return 0; }

unsigned int LabelExtractor::get_label(const std::string &label_string)
{
  if (label_map.find(label_string) == label_map.end())
    {
      unsigned int id = label_map.size();
      label_map[label_string] = id;
      string_map.push_back(label_string);
    }

  return label_map[label_string];
}

unsigned int LabelExtractor::label_count(void) const
{
  return string_map.size();
}

LabelVector LabelExtractor::get_labels(const StringVector &label_strings)
{
  LabelVector res;

  for (unsigned int i = 0; i < label_strings.size(); ++i)
    { 
      res.push_back(get_label(label_strings[i]));
    }

  return res;
}

std::string get_suffix(const std::string &wf, unsigned int length)
{
  return (PADDING + wf).substr(PADDING_LEN + wf.size() - length);
}

typedef std::unordered_map<std::string, std::unordered_map<unsigned int,
							   unsigned int> >
StringLabelCounts;

struct CmpPairs
{
  bool operator() (const std::pair<unsigned int, unsigned int> &p1,
		   const std::pair<unsigned int, unsigned int> &p2) const
  { 
    return p1.second > p2.second;
  }
};

void LabelExtractor::train(Data &data)
{
  StringLabelCounts inverse_label_counts;
  StringLabelCounts lexicon_counts;

  for (unsigned int i = 0; i < data.size(); ++i)
    {
      for (unsigned int j = 0; j < data.at(i).size(); ++j)
	{
	  const Word &w = data.at(i).at(j);

	  std::string wf = w.get_word_form();

	  if (wf == BOUNDARY_WF)
	    { continue; }

	  unsigned int label = w.get_label();

	  ++lexicon_counts[wf][label];

	  if (label == static_cast<unsigned int>(NO_LABEL))
	    {
	      throw LabelNotSet();
	    }
	  
	  for (unsigned int k = 0; k <= max_suffix_len; ++k)
	    {
	      ++inverse_label_counts[get_suffix(wf, k)][label];
	    }
	}
    }
  
  for (StringLabelCounts::const_iterator it = inverse_label_counts.begin();
       it != inverse_label_counts.end();
       ++it)
    {
      const std::string &sub_string = it->first;

      std::vector<std::pair<unsigned int, unsigned int> > l_counts;

      for (std::unordered_map<unsigned int, unsigned int>::const_iterator jt =
	     it->second.begin();
	   jt != it->second.end();
	   ++jt)
	{ 
	  l_counts.push_back
	    (std::pair<unsigned int, unsigned int>(jt->first, jt->second));
	} 

      std::sort(l_counts.begin(), l_counts.end(), CmpPairs());
      
      LabelVector &v = label_counts[sub_string];

      for (unsigned int i = 0; i < l_counts.size(); ++i)
	{
	  v.push_back(l_counts[i].first);

	}
    }

  for (StringLabelCounts::const_iterator it = lexicon_counts.begin();
       it != lexicon_counts.end();
       ++it)
    {
      LabelVector &v = lexicon[it->first];

      for (std::unordered_map<unsigned int, unsigned int>::const_iterator jt =
	     it->second.begin();
	   jt != it->second.end();
	   ++jt)
	{ 
	  v.push_back(jt->first);
	} 
    }
}

void LabelExtractor::set_label_candidates(const std::string &word_form, 
					  bool use_lexicon,
					  unsigned int count, 
					  LabelVector &target) const
{
  if (word_form == BOUNDARY_WF)
    { 
      target.push_back(get_boundary_label());
      return;
    }

  if (use_lexicon and lexicon.count(word_form) != 0)
    {
      std::unordered_set<unsigned int> label_set(target.begin(), target.end());
      SubstringLabelMap::const_iterator it = lexicon.find(word_form);
      label_set.insert(it->second.begin(), it->second.end());
      target.assign(label_set.begin(), label_set.end());
      return;
    }

  std::unordered_set<unsigned int> label_set(target.begin(), target.end());

  for (int i = max_suffix_len; i >= 0; --i)
    {
      std::string suffix = get_suffix(word_form, i);

      if (label_counts.count(suffix) != 0)
	{
	  const LabelVector &v = label_counts.find(suffix)->second;

	  for (unsigned int j = 0; j < v.size(); ++j)
	    {
	      if (label_set.size() >= count)
		{
		  break;
		}

	      label_set.insert(v.at(j));
	    }
	}

      if (label_set.size() >= count)
	{
	  break;
	}
    }

  target.assign(label_set.begin(), label_set.end());
}

const std::string &LabelExtractor::get_label_string(unsigned int label) const
{
  if (label >= string_map.size())
    { 
      throw IllegalLabel(); 
    }

  return string_map.at(label);
}

#else // TEST_LabelExtractor_cc

#include <sstream>
#include <string>
#include <cassert>
#include <unordered_set>

#include "ParamTable.hh"
#include "Data.hh"

int main(void)
{
  std::string contents("\n"
		       "The\tWORD=The\tthe\tDT\t_\n"
		       "dog\tWORD=dog\tdog\tNN\t_\n"
		       ".\tWORD=.\t.\t.\t_\n"
		       "\n"
		       "\n"
		       "The\tWORD=The\tthe\tDT\t_\n"
		       "dog\tWORD=dog\tdog\tNN\t_\n"
		       ".\tWORD=.\t.\t.\t_\n"
		       "\n"
		       "\n"
		       ".\tWORD=.\t.\t.\t_\n");

  std::istringstream in(contents);

  LabelExtractor le(2);

  ParamTable pt;

  Data data(in, 1, le, pt, 2);

  le.train(data);

  LabelVector labels1;
  LabelVector labels2;
  LabelVector labels3;
  LabelVector labels4;

  le.set_label_candidates("hog", 0, 1, labels1);
  le.set_label_candidates("hog", 0, 2, labels2);
  le.set_label_candidates("hog", 0, 3, labels3);
  le.set_label_candidates("hog", 0, 4, labels4);

  assert(labels1.size() == 1);
  std::unordered_set<unsigned int> labelS1(labels1.begin(), labels1.end());
  std::unordered_set<unsigned int> labelSS1;
  labelSS1.insert(le.get_label("NN"));
  assert(labelS1 == labelSS1);

  assert(labels2.size() == 2);
  std::unordered_set<unsigned int> labelS2(labels2.begin(), labels2.end());
  std::unordered_set<unsigned int> labelSS2;
  labelSS2.insert(le.get_label("NN"));
  labelSS2.insert(le.get_label("."));
  assert(labelS2 == labelSS2);

  assert(labels3.size() == 3);
  std::unordered_set<unsigned int> labelS3(labels3.begin(), labels3.end());
  std::unordered_set<unsigned int> labelSS3;
  labelSS3.insert(le.get_label("NN"));
  labelSS3.insert(le.get_label("."));
  labelSS3.insert(le.get_label("DT"));
  assert(labelS3 == labelSS3);

  assert(labels4.size() == 3);
  std::unordered_set<unsigned int> labelS4(labels4.begin(), labels4.end());
  std::unordered_set<unsigned int> labelSS4;
  labelSS4.insert(le.get_label("NN"));
  labelSS4.insert(le.get_label("."));
  labelSS4.insert(le.get_label("DT"));
  assert(labelS4 == labelSS4);

  LabelVector dog_labels;
  le.set_label_candidates("dog", 1, 5, dog_labels);
  assert(dog_labels.size() == 1);
}

#endif // TEST_LabelExtractor_cc
