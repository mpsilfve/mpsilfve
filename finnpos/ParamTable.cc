/**
 * @file    ParamTable.cc                                                
 * @Author  Miikka Silfverberg                                               
 * @brief   Parameter table for structured and unstructured parameters. Used 
 *          by both Tagger and LemmaExtractor.                                 
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

#include "ParamTable.hh"

#ifndef TEST_ParamTable_cc

#include "Word.hh"

ParamTable::ParamTable(void):
  trained(0)
{}

ParamTable::~ParamTable(void)
{}

ParamTable &ParamTable::operator=(const ParamTable &another)
{
  if (this == &another)
    { return *this; }

  trained              = another.trained;
  feature_template_map = another.feature_template_map;
  unstruct_param_table = another.unstruct_param_table;
  struct_param_table   = another.struct_param_table;

  return *this;
}

void ParamTable::set_trained(void)
{
  trained = 1;
}

unsigned int ParamTable::get_feat_template
(const std::string &feat_template_string)
{
  if (feature_template_map.find(feat_template_string) == 
      feature_template_map.end())
    { 
      feature_template_map[feat_template_string] = feature_template_map.size();
    }

  return feature_template_map[feat_template_string];
}

FeatureTemplateVector ParamTable::get_feat_templates
(StringVector &feat_template_strings)
{
  FeatureTemplateVector feat_templates;

  for (unsigned int i = 0; i < feat_template_strings.size(); ++i)
    {
      if (trained and feature_template_map.count(feat_template_strings[i]) == 0)
	{ 
	  continue; 
	}
     
      feat_templates.push_back(get_feat_template(feat_template_strings[i]));
    }

  return feat_templates;
}

long ParamTable::get_unstruct_param_id(unsigned int feature_template, 
				       unsigned int label) const
{
  return (MAX_LABEL + 1) * feature_template + label;
}

long ParamTable::get_struct_param_id(unsigned int label) const
{
  return 
    (MAX_LABEL + 1) * (MAX_LABEL + 1) * (MAX_LABEL + 1) +
    (MAX_LABEL + 1) * (MAX_LABEL + 1) +
    label;
}

long ParamTable::get_struct_param_id(unsigned int plabel, unsigned int label) 
  const
{
  return 
    (MAX_LABEL + 1) * (MAX_LABEL + 1) * (MAX_LABEL + 1) +
    plabel * (MAX_LABEL + 1) +
    label;
}

long ParamTable::get_struct_param_id(unsigned int pplabel, 
				     unsigned int plabel, 
				     unsigned int label) const
{
  return 
    pplabel * (MAX_LABEL + 1) * (MAX_LABEL + 1) +
    plabel * (MAX_LABEL + 1) +
    label;
}

float ParamTable::get_unstruct(unsigned int feature_template, 
			       unsigned int label) const
{
  long id = get_unstruct_param_id(feature_template, label);
  
  ParamMap::const_iterator it = unstruct_param_table.find(id);

  if (it == unstruct_param_table.end())
    { return 0; }

  return it->second;
}

float ParamTable::get_struct(unsigned int label) const
{
  long id = get_struct_param_id(label);
  
  ParamMap::const_iterator it = struct_param_table.find(id);

  if (it == struct_param_table.end())
    { return 0; }

  return it->second;
}

float ParamTable::get_struct(unsigned int plabel, unsigned int label) const
{
  long id = get_struct_param_id(plabel, label);
  
  ParamMap::const_iterator it = struct_param_table.find(id);

  if (it == struct_param_table.end())
    { return 0; }

  return it->second;
}

float ParamTable::get_struct(unsigned int pplabel,
			     unsigned int plabel, 
			     unsigned int label) const
{
  long id = get_struct_param_id(pplabel, plabel, label);
  
  ParamMap::const_iterator it = struct_param_table.find(id);

  if (it == struct_param_table.end())
    { return 0; }

  return it->second;
}

float ParamTable::get_all_unstruct(const Word &word, unsigned int label) const
{
  float res = 0;

  for (unsigned int i = 0; i < word.get_feature_template_count(); ++i)
    {
      res += get_unstruct(word.get_feature_template(i), label);
    }

  return res;
}

float ParamTable::get_all_struct_fw(unsigned int pplabel, 
				    unsigned int plabel, 
				    unsigned int label) const
{
  return 
    get_struct(pplabel, plabel, label) +
    get_struct(plabel, label) +
    get_struct(label);
}

float ParamTable::get_all_struct_bw(unsigned int pplabel, 
				    unsigned int plabel, 
				    unsigned int label) const
{
  return 
    get_struct(pplabel, plabel, label) +
    get_struct(plabel, label) +
    get_struct(label);
}

void ParamTable::update_unstruct(unsigned int feature_template, 
				 unsigned int label, 
				 float ud)
{
  unstruct_param_table[get_unstruct_param_id(feature_template, label)] += ud;
}

void ParamTable::update_struct(unsigned int label, float ud)
{
  struct_param_table[get_struct_param_id(label)] += ud;
}

void ParamTable::update_struct(unsigned int plabel, 
			       unsigned int label, 
			       float ud)
{
  struct_param_table[get_struct_param_id(plabel, label)] += ud;
}

void ParamTable::update_struct(unsigned int pplabel, 
			       unsigned int plabel, 
			       unsigned int label, 
			       float ud)
{
  struct_param_table[get_struct_param_id(pplabel, plabel, label)] += ud;
}

void ParamTable::update_all_struct_fw(unsigned int pplabel, unsigned int plabel, unsigned int label, float update)
{
  update_struct(label, update);
  update_struct(plabel, label, update);
  update_struct(pplabel, plabel, label, update); 
}

void ParamTable::update_all_struct_bw(unsigned int pplabel, unsigned int plabel, unsigned int label, float update)
{
  update_struct(pplabel, plabel, label, update);
  update_struct(plabel, label, update);
  update_struct(label, update);
}

void ParamTable::update_all_unstruct(const Word &word, unsigned int label, float update)
{
  for (unsigned int i = 0; i < word.get_feature_template_count(); ++i)
    {
      update_unstruct(word.get_feature_template(i), label, update);
    }
}

ParamMap::iterator ParamTable::get_unstruct_begin(void)
{
  return unstruct_param_table.begin();
}

ParamMap::iterator ParamTable::get_struct_begin(void)
{
  return struct_param_table.begin();
}

ParamMap::iterator ParamTable::get_unstruct_end(void)
{
  return unstruct_param_table.end();
}

ParamMap::iterator ParamTable::get_struct_end(void)
{
  return struct_param_table.end();
}

#include <cassert>

void ParamTable::store(std::ostream &out) const
{
  write_val(out, trained);
  write_map(out, feature_template_map);
  write_map(out, unstruct_param_table);
  write_map(out, struct_param_table);
}

void ParamTable::load(std::istream &in, bool reverse_bytes)
{
  read_val<bool>(in, trained, reverse_bytes);
  read_map(in, feature_template_map, reverse_bytes);
  read_map(in, unstruct_param_table, reverse_bytes);
  read_map(in, struct_param_table, reverse_bytes);
}

bool ParamTable::operator==(const ParamTable &another) const
{
  if (this == &another)
    { return 1; }

  return
    (trained == another.trained and
     feature_template_map == another.feature_template_map and
     unstruct_param_table == another.unstruct_param_table and
     struct_param_table == another.struct_param_table);
}

#else // TEST_ParamTable_cc

#include <sstream>
#include <cassert>

int main(void)
{
  ParamTable pt;

  unsigned int label_foo = pt.get_feat_template("FOO");
  unsigned int label_bar = pt.get_feat_template("BAR");
  
  assert(label_foo != label_bar);

  assert(label_foo == pt.get_feat_template("FOO"));
  assert(label_bar == pt.get_feat_template("BAR"));

  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 0) == 0);
  pt.update_unstruct(pt.get_feat_template("FOO"), 0, 1);
  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 0) == 1);

  assert(pt.get_unstruct(pt.get_feat_template("BAR"), 0) == 0);
  pt.update_unstruct(pt.get_feat_template("BAR"), 0, 2);
  assert(pt.get_unstruct(pt.get_feat_template("BAR"), 0) == 2);

  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 1) == 0);
  pt.update_unstruct(pt.get_feat_template("FOO"), 1, 3);
  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 1) == 3);

  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 0) == 1);
  pt.update_unstruct(pt.get_feat_template("FOO"), 0, 1);
  assert(pt.get_unstruct(pt.get_feat_template("FOO"), 0) == 2);

  assert(pt.get_struct(0) == 0);
  pt.update_struct(0, 1);
  assert(pt.get_struct(0) == 1);

  assert(pt.get_struct(1) == 0);
  pt.update_struct(1, 2);
  assert(pt.get_struct(1) == 2);

  assert(pt.get_struct(0, 0) == 0);
  pt.update_struct(0, 0, 1);
  assert(pt.get_struct(0, 0) == 1);

  assert(pt.get_struct(0, 1) == 0);
  pt.update_struct(0, 1, 2);
  assert(pt.get_struct(0, 1) == 2);

  assert(pt.get_struct(0, 0, 0) == 0);
  pt.update_struct(0, 0, 0, 1);
  assert(pt.get_struct(0, 0, 0) == 1);

  assert(pt.get_struct(0, 1, 0) == 0);
  pt.update_struct(0, 1, 0, 2);
  assert(pt.get_struct(0, 1, 0) == 2);  

  std::ostringstream pt_out;
  pt.store(pt_out);
  std::istringstream pt_in(pt_out.str());
  ParamTable pt_copy;
  pt_copy.load(pt_in, false);
  assert(pt_copy == pt);
}

#endif // TEST_ParamTable_cc
