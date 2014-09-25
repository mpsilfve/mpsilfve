#ifndef HEADER_ParamTable_hh
#define HEADER_ParamTable_hh

#include <unordered_map>
#include <vector>
#include <string>

#include "io.hh"


typedef std::vector<unsigned int> FeatureTemplateVector;

const long MAX_LABEL = 50000;

class Word;

typedef std::unordered_map<long, float> ParamMap;

class ParamTable
{
public:
   virtual ~ParamTable(void);

  unsigned int get_feat_template(const std::string &feat_template_string);
  FeatureTemplateVector get_feat_templates(StringVector &feat_template_strings);

  float get_unstruct(unsigned int feature_template, unsigned int label) const;
  float get_struct(unsigned int label) const;
  float get_struct(unsigned int plabel, unsigned int label) const;
  float get_struct(unsigned int pplabel, unsigned int plabel, unsigned int label) const;

  float get_all_struct_fw(unsigned int pplabel, unsigned int plabel, unsigned int label) const;
  float get_all_struct_bw(unsigned int pplabel, unsigned int plabel, unsigned int label) const;
  float get_all_unstruct(const Word &word, unsigned int label) const;

  void update_all_struct_fw(unsigned int pplabel, unsigned int plabel, unsigned int label, float update);
  void update_all_struct_bw(unsigned int pplabel, unsigned int plabel, unsigned int label, float update);
  void update_all_unstruct(const Word &word, unsigned int label, float update);

  void update_unstruct(unsigned int feature_template, unsigned int label, float ud);
  void update_struct(unsigned int label, float ud);
  void update_struct(unsigned int plabel, unsigned int label, float ud);
  void update_struct(unsigned int pplabel, unsigned int plabel, unsigned int label, float ud);

  ParamMap::iterator get_unstruct_begin(void);
  ParamMap::iterator get_struct_begin(void);
  ParamMap::iterator get_unstruct_end(void);
  ParamMap::iterator get_struct_end(void);

private:
  typedef std::unordered_map<std::string, unsigned int> FeatureTemplateMap;

  FeatureTemplateMap feature_template_map;
  ParamMap unstruct_param_table;
  ParamMap struct_param_table;

  long get_unstruct_param_id(unsigned int feature_template, unsigned int label) const;
  long get_struct_param_id(unsigned int label) const;
  long get_struct_param_id(unsigned int plabel, unsigned int label) const;
  long get_struct_param_id(unsigned int pplabel, unsigned int plabel, unsigned int label) const;
};

#endif // HEADER_ParamTable_hh
