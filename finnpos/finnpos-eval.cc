/**
 * @file    finnpos-eval.cc                       
 * @Author  Miikka Silfverberg                                               
 * @brief   Compare two tagged files.
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

#include <iostream>

#include "Data.hh"
#include "ParamTable.hh"

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cerr << "USAGE: " << argv[0] << " sys_tagged_file << gold_tagged_file"
		<< std::endl;
    }

  ParamTable param_table;
  LabelExtractor label_extractor;
  LemmaExtractor lemma_extractor;

  Data sys_data(argv[1], 1, label_extractor, param_table, 2);
  Data gold_data(argv[2], 1, label_extractor, param_table, 2);

  Acc acc = gold_data.get_acc(sys_data, lemma_extractor);

  std::cout << "Comparing " << argv[1] << " and " << argv[2] << " (gold standard)." 
	    << std::endl;
  std::cout << std::endl;
  std::cout << "Label accuracy: " << acc.label_acc << std::endl;
  std::cout << "Label accuracy for IV words: " << acc.iv_label_acc << std::endl;
  std::cout << "Label accuracy for OOV words: " << acc.oov_label_acc << std::endl;
  std::cout << std::endl;
  std::cout << "Lemma accuracy: " << acc.lemma_acc << std::endl;
  std::cout << "Lemma accuracy for IV words: " << acc.iv_lemma_acc << std::endl;
  std::cout << "Lemma accuracy for OOV words: " << acc.oov_lemma_acc << std::endl;
}
