#include <iostream>

#include "Data.hh"
#include "PerceptronTrainer.hh"
#include "ParamTable.hh"
#include "LabelExtractor.hh"
#include "LemmaExtractor.hh"
#include "Trellis.hh"

int main(int argc, char * argv[])
{
  static_cast<void>(argc);

  ParamTable param_table;
  LabelExtractor label_extractor(10);
  LemmaExtractor lemma_extractor;

  Data train_data(argv[1], 1, label_extractor, param_table, 2);
  label_extractor.train(train_data);

  train_data.set_label_guesses(label_extractor, 5);

  Data   dev_data(argv[2], 1, label_extractor, param_table, 2);
  dev_data.set_label_guesses(label_extractor, 5);

  Data  gold_test_data(argv[3], 1, label_extractor, param_table, 2);
  gold_test_data.set_label_guesses(label_extractor, 5);

  Data test_data(gold_test_data);
  test_data.unset_label();
  std::cerr << gold_test_data.at(0).at(3).get_label_count() << std::endl;

  TrellisVector test_trellises;
  populate(test_data, test_trellises, label_extractor.get_boundary_label(), 5);

  for (unsigned int i = 0; i < test_trellises.size(); ++i)
    { test_trellises.at(i).set_maximum_a_posteriori_assignment(param_table); }

  std::cerr << test_data.get_acc(gold_test_data, lemma_extractor).label_acc 
	    << std::endl;

  PerceptronTrainer trainer(50, 3, param_table, 
			    label_extractor.get_boundary_label(), 
			    lemma_extractor);

  trainer.train(train_data, dev_data, 5);

  for (unsigned int i = 0; i < test_trellises.size(); ++i)
    { test_trellises.at(i).set_maximum_a_posteriori_assignment(param_table); }

  std::cerr << test_data.get_acc(gold_test_data, lemma_extractor).label_acc 
	    << std::endl;

}
