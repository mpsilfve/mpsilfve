#include "LemmaExtractor.hh"

std::string lowercase(const std::string &word);

std::string uppercase(const std::string &word);

std::string init_uppercase(const std::string &word);

typedef std::pair<std::string, std::string> StringPair;

StringPair get_minimal_suffix_edit(const std::string &word1, 
				   const std::string &word2);

bool has_digit(const std::string &word);

bool has_upper(const std::string &word);

#ifndef TEST_LemmaExtractor_cc

#include <ctype.h>
#include <cstring>
#include <cfloat>
#include <unordered_set>
#include <locale>
#include <cassert>

#include "PerceptronTrainer.hh"
#include "Word.hh"

#define PADDING "^^^^^^^^^^" 

std::locale loc;

LemmaExtractor::LemmaExtractor(void):
  class_count(1)
{}

LemmaExtractor::~LemmaExtractor(void)
{}

bool LemmaExtractor::is_known_wf(const std::string &word_form) const
{
  static_cast<void>(word_form);

  return 0;
}

void LemmaExtractor::train(const Data &train_data, const Data &dev_data, const LabelExtractor &le)
{
  PerceptronTrainer trainer(20, 20, param_table, -1, *this);
  trainer.train_lemmatizer(train_data, dev_data, *this, le);
}

std::string lowercase(const std::string &word)
{
  std::string lc_word = word;

  for (unsigned int i = 0; i < lc_word.size(); ++i)
    {
      lc_word[i] = tolower(lc_word[i]);
    }

  size_t pos = 0;

  while ((pos = lc_word.find("Å")) != std::string::npos)
    {
      lc_word.replace(pos, strlen("Å"), "å");
    }

  pos = 0;

  while ((pos = lc_word.find("Ä")) != std::string::npos)
    {
      lc_word.replace(pos, strlen("Ä"), "ä");
    }

  pos = 0;

  while ((pos = lc_word.find("Ö")) != std::string::npos)
    {
      lc_word.replace(pos, strlen("Ö"), "ö");
    }

  return lc_word;
}

std::string uppercase(const std::string &word)
{
  std::string uc_word = word;

  for (unsigned int i = 0; i < uc_word.size(); ++i)
    {
      uc_word[i] = toupper(uc_word[i]);
    }

  size_t pos = 0;

  while ((pos = uc_word.find("å")) != std::string::npos)
    {
      uc_word.replace(pos, strlen("å"), "Å");
    }

  pos = 0;

  while ((pos = uc_word.find("ä")) != std::string::npos)
    {
      uc_word.replace(pos, strlen("ä"), "Ä");
    }

  pos = 0;

  while ((pos = uc_word.find("ö")) != std::string::npos)
    {
      uc_word.replace(pos, strlen("ö"), "Ö");
    }

  return uc_word;
}

std::string init_uppercase(const std::string &word)
{
  std::string uc_word = word;

  if (uc_word.find("å") != std::string::npos)
    { 
      uc_word.replace(0, 2, "Å");
    }
  else if (uc_word.find("ä") != std::string::npos)
    { 
      uc_word.replace(0, 2, "Ä");
    }
  else if (uc_word.find("ö") != std::string::npos)
    { 
      uc_word.replace(0, 2, "Ö");
    }
  else
    {
      uc_word[0] = toupper(uc_word[0]);
    }

  return uc_word;
}

std::string suffix(const std::string word, size_t start)
{
  if (start >= word.size())
    { return ""; }

  return word.substr(start);
}

StringPair get_minimal_suffix_edit(const std::string &word1,
				   const std::string &word2)
{
  unsigned int start = 0;

  for (unsigned int i = 0; i < word1.size(); ++i)
    {
      if (i >= word2.size())
	{ break; }
      
      if (word1[i] != word2[i])
	{ break; }

      ++start;
    }

  return StringPair(suffix(word1, start),
		    suffix(word2, start));
}

unsigned int LemmaExtractor::get_class_number(const std::string &word, 
					      const std::string &lemma)
{
  std::string lc_word = lowercase(word);
  std::string lc_lemma = lowercase(lemma);

  StringPair suffixes = get_minimal_suffix_edit(lc_word, lc_lemma);

  if (suffix_map.count(suffixes.first) == 0)
    { static_cast<void>(suffix_map[suffixes.first]); }

  const std::string &wf_suffix = suffixes.first;
  const std::string &lemma_suffix = suffixes.second;

  if (suffix_map[wf_suffix].count(lemma_suffix) == 0)
    { 
      suffix_map[wf_suffix][lemma_suffix] = class_count;
      id_map[class_count] = StringPair(wf_suffix, lemma_suffix);
      ++class_count;
    }

  return suffix_map[wf_suffix][lemma_suffix];
}

unsigned int LemmaExtractor::get_class_number(const std::string &word, 
					      const std::string &lemma) const
{
  std::string lc_word = lowercase(word);
  std::string lc_lemma = lowercase(lemma);

  StringPair suffixes = get_minimal_suffix_edit(lc_word, lc_lemma);

  const std::string wf_suffix = suffixes.first;
  const std::string lemma_suffix = suffixes.second;

  if (suffix_map.count(wf_suffix) == 0)
    { throw UnknownSuffixPair(); }
  
  if (suffix_map.find(wf_suffix)->second.count(lemma_suffix) == 0)
    { throw UnknownSuffixPair(); }

  return suffix_map.find(wf_suffix)->second.find(lemma_suffix)->second;
}

void LemmaExtractor::set_class_candidates(const std::string &word,
					  LabelVector &class_vector) const
{
  std::unordered_set<unsigned int> potential_classes;
  
  std::string lc_word = lowercase(word);

  for (unsigned int i = 0; i < lc_word.size() + 1; ++i)
    {
      std::string substr = lc_word.substr(i);

      if (suffix_map.count(substr) != 0)
	{
	  const ClassIDMap &clm = suffix_map.find(substr)->second;

	  for (ClassIDMap::const_iterator it = clm.begin(); 
	       it != clm.end(); 
	       ++it)
	    { potential_classes.insert(it->second); }
	}
    }

  class_vector.assign(potential_classes.begin(), potential_classes.end());
}

bool has_upper(const std::string &word)
{
  if (word.find("Ä") != std::string::npos or
      word.find("Å") != std::string::npos or
      word.find("Ö") != std::string::npos)
    { return 1; }

  const char * c_word = word.c_str();
  
  const char * upper_pt = std::use_facet< std::ctype<char> >(loc).scan_is 
    (std::ctype<char>::upper, c_word, c_word + word.size() + 1);

  return upper_pt - c_word < static_cast<long>(word.size());
}

bool has_digit(const std::string &word)
{
  const char * c_word = word.c_str();
  
  const char * digit_pt = std::use_facet< std::ctype<char> >(loc).scan_is 
    (std::ctype<char>::digit, c_word, c_word + word.size() + 1);

  return digit_pt - c_word < static_cast<long>(word.size());
}

Word * LemmaExtractor::extract_feats(const std::string &word_form, 
				     const std::string &label)
{
  FeatureTemplateVector feats;

  feats.push_back( feat_dict["WORD=" + word_form] );

  std::string padded_word_form = PADDING + word_form;

  for (unsigned int i = padded_word_form.size() - 10; 
       i <= padded_word_form.size();
       ++i)
    {
      feats.push_back( feat_dict["SUFFIX=" + padded_word_form.substr(i)] );
    }

  feats.push_back( feat_dict["LABEL=" + label] );

  if (has_upper(word_form))
    { feats.push_back( feat_dict["UC"] ); }

  if (has_digit(word_form))
    { feats.push_back( feat_dict["DIGIT"] ); }

  return new Word(word_form, feats, LabelVector(), "");
}

std::string LemmaExtractor::get_lemma(const std::string &word_form, 
				      unsigned int klass) const
{
  if (id_map.count(klass) == 0)
    { throw UnknownClass(); }

  const StringPair &word_and_lemma_suffixes = id_map.find(klass)->second;

  const std::string &word_suffix = word_and_lemma_suffixes.first;
  const std::string &lemma_suffix = word_and_lemma_suffixes.second;
  
  size_t pos = word_form.rfind(word_suffix);

  assert(pos != std::string::npos);
  
  return word_form.substr(0, pos) + lemma_suffix;
}

unsigned int LemmaExtractor::get_lemma_candidate_class(const Word &w,
						       ParamTable * pt)
{
  if (pt == 0)
    { pt = &param_table; }

  LabelVector lemma_class_candidates;
  set_class_candidates(w.get_word_form(), lemma_class_candidates);

  unsigned int max_score = -FLT_MAX;
  unsigned int max_class = -1;

  for (unsigned int i = 0; i < lemma_class_candidates.size(); ++i)
    {
      unsigned int klass = lemma_class_candidates[i];

      float score = pt->get_all_unstruct(w, klass);

      if (score > max_score)
	{
	  max_score = score;
	  max_class = klass;
	}
    }
  
  return max_class;
}

unsigned int LemmaExtractor::get_lemma_candidate_class
(const std::string &word_form, 
 const std::string &label)
{
  Word * w = extract_feats(word_form, label);

  unsigned int klass = get_lemma_candidate_class(*w);

  delete w;

  return klass;
}

std::string LemmaExtractor::get_lemma_candidate(const std::string &word_form, 
						const std::string &label)
{
  return get_lemma(word_form, 
		   get_lemma_candidate_class(word_form, 
					     label)); 
}  

#else // TEST_LemmaExtractor_cc

struct TEST_LemmaExtractor
{
  LemmaExtractor &le;

  TEST_LemmaExtractor(LemmaExtractor &le):
    le(le)
  {}

  unsigned int get_class_number(const std::string &word, 
				const std::string &lemma)
  { return le.get_class_number(word, lemma); }

  void set_class_candidates(const std::string &word,
			    LabelVector &class_vector) const
  { le.set_class_candidates(word, class_vector); }

  std::string get_lemma(const std::string &word_form, 
			unsigned int klass) const
  { return le.get_lemma(word_form, klass); }
};

#include <cassert>
#include <sstream>

#include "Data.hh"

int main(void)
{
  assert(lowercase("koira") == "koira");
  assert(lowercase("KOIRA") == "koira");
  assert(lowercase("Koira") == "koira");
  assert(lowercase("KoirA") == "koira");

  assert(uppercase("koira") == "KOIRA");
  assert(uppercase("KOIRA") == "KOIRA");
  assert(uppercase("Koira") == "KOIRA");
  assert(uppercase("KoirA") == "KOIRA");

  assert(init_uppercase("koira") == "Koira");
  assert(init_uppercase("KOIRA") == "KOIRA");
  assert(init_uppercase("Koira") == "Koira");
  assert(init_uppercase("KoirA") == "KoirA");

  assert(lowercase("äijän") == "äijän");
  assert(lowercase("ÄIJÄN") == "äijän");
  assert(lowercase("Äijän") == "äijän");
  assert(lowercase("Äijän") == "äijän");

  assert(uppercase("äijän") == "ÄIJÄN");
  assert(uppercase("ÄIJÄN") == "ÄIJÄN");
  assert(uppercase("Äijän") == "ÄIJÄN");
  assert(uppercase("Äijän") == "ÄIJÄN");

  assert(init_uppercase("äijän") == "Äijän");
  assert(init_uppercase("ÄIJÄN") == "ÄIJÄN");
  assert(init_uppercase("Äijän") == "Äijän");
  assert(init_uppercase("Äijän") == "Äijän");

  assert(get_minimal_suffix_edit("koira", "koira") == StringPair("",""));
  assert(get_minimal_suffix_edit("koira", "koiran") == StringPair("","n"));
  assert(get_minimal_suffix_edit("äijä", "äijä") == StringPair("",""));
  assert(get_minimal_suffix_edit("äijän", "äijää") == StringPair("n","ä"));
  assert(get_minimal_suffix_edit("laidun", "laitumen") == 
	 StringPair("dun","tumen"));

  LemmaExtractor le;

  TEST_LemmaExtractor tle(le);

  assert(tle.get_class_number("koira", "koira") == 1);
  assert(tle.get_class_number("KOIRA", "KOIRA") == 1);
  assert(tle.get_class_number("Koira", "Koira") == 1);

  assert(tle.get_class_number("isä", "isä") == 1);
  assert(tle.get_class_number("ISÄ", "ISÄ") == 1);
  assert(tle.get_class_number("Isä", "Isä") == 1);

  assert(tle.get_class_number("koiran", "koira") == 2);
  assert(tle.get_class_number("KOIRAN", "KOIRA") == 2);
  assert(tle.get_class_number("Koiran", "Koira") == 2);

  assert(tle.get_class_number("isän", "isä") == 2);
  assert(tle.get_class_number("ISÄN", "ISÄ") == 2);
  assert(tle.get_class_number("Isän", "Isä") == 2);

  assert(tle.get_class_number("isästä", "isä") == 3);
  assert(tle.get_class_number("ISÄSTÄ", "ISÄ") == 3);
  assert(tle.get_class_number("Isästä", "Isä") == 3);
  
  assert(tle.get_lemma("isä", 1) == "isä");
  assert(tle.get_lemma("isän", 1) == "isän");
  assert(tle.get_lemma("isän", 2) == "isä");

  assert(tle.get_lemma("kissa", 1) == "kissa");
  assert(tle.get_lemma("kissan", 1) == "kissan");
  assert(tle.get_lemma("kissan", 2) == "kissa");
  assert(tle.get_lemma("kissastä", 1) == "kissastä");
  assert(tle.get_lemma("kissastä", 3) == "kissa");

  LabelVector labels;
  tle.set_class_candidates("äidin", labels);

  assert(labels.size() == 2);
  assert(labels[0] == 1 or labels[0] == 2);
  assert(labels[1] == 1 or labels[1] == 2);
  assert(labels[0] != labels[1]);

  labels.clear();
  tle.set_class_candidates("äidistä", labels);

  assert(labels.size() == 2);
  assert(labels[0] == 3 or labels[0] == 1);
  assert(labels[1] == 3 or labels[1] == 1);
  assert(labels[0] != labels[1]);

  labels.clear();
  tle.set_class_candidates("ÄIDISTÄ", labels);

  assert(labels.size() == 2);
  assert(labels[0] == 3 or labels[0] == 1);
  assert(labels[1] == 3 or labels[1] == 1);
  assert(labels[0] != labels[1]);

  assert(not has_upper(""));
  assert(not has_upper("koira"));
  assert(has_upper("Koira"));
  assert(not has_upper("äiti"));
  assert(has_upper("Äiti"));
  assert(has_upper("äitI"));
  assert(has_upper("A"));
  assert(has_upper("Å"));
  assert(has_upper("Ä"));
  assert(has_upper("Ö"));

  assert(not has_digit(""));
  assert(not has_digit("koira"));
  assert(not has_digit("äiti"));
  assert(has_digit("2"));
  assert(has_digit("koira1"));
  assert(has_digit("1koira"));
  assert(has_digit("äiti1"));
  assert(has_digit("1äiti"));

  std::string contents("\n"
		       "The\tWORD=The\tthe\tDT\t_\n"
		       "dogs\tWORD=dogs\tdog\tNN\t_\n"
		       ".\tWORD=.\t.\t.\t_\n"
		       "\n"
		       "\n"
		       "The\tWORD=The\tthe\tDT\t_\n"
		       "dogs\tWORD=dogs\tdog\tNN\t_\n"
		       ".\tWORD=.\t.\t.\t_\n");

  std::istringstream in(contents);

  ParamTable pt;

  LabelExtractor label_extractor(10);

  Data train_data(in, 1, label_extractor, pt, 2);
  Data dev_data(train_data);

  LemmaExtractor lemma_extractor;
  
  lemma_extractor.train(train_data, dev_data, label_extractor);

  //assert(lemma_extractor.get_lemma_candidate("hog", "NN") == "hog");
  assert(lemma_extractor.get_lemma_candidate("hogs", "NN") == "hog");
  //  assert(lemma_extractor.get_lemma_candidate("hog", "DT") == "hog");
  //assert(lemma_extractor.get_lemma_candidate("hogs", "DT") == "hogs");
}

#endif // TEST_LemmaExtractor_cc
