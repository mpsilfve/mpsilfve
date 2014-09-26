#include "Data.hh"
#include "LemmaExtractor.hh"
#include "LabelExtractor.hh"
#include "ParamTable.hh"

#include <iostream>

int main(int argc, char * argv[])
{
  static_cast<void>(argc);

  LabelExtractor label_e(10);

  ParamTable pt;

  Data train_data(argv[1], 1, label_e, pt,2);
  Data dev_data(argv[2], 1, label_e, pt,2);

  LemmaExtractor lemma_e;
  lemma_e.train(train_data, dev_data, label_e);

  float corr = 0;
  float total = 0;

  for (unsigned int i = 0; i < dev_data.size(); ++i)
    {
      for (unsigned int j = 0; j < dev_data.at(i).size(); ++j)
	{
	  const Word &w = dev_data.at(i).at(j);
	  
	  std::string word = w.get_word_form();
	  std::string label = label_e.get_label_string(w.get_label());
	  std::string gold_lemma = w.get_lemma();
	  std::string sys_lemma = lemma_e.get_lemma_candidate(word, label);

	  std::cout << word << ' ' << gold_lemma << ' ' << sys_lemma << std::endl;

	  if (gold_lemma == sys_lemma)
	    { ++corr; }

	  ++total;
	}
    }

  std::cout << corr/total << std::endl;
}
