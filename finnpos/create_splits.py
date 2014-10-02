"""@package create_splits

 file    create_splits.py                                                   
 Author  Miikka Silfverberg                                                 
 brief   Divide input file into 80% training data, and 10 % development and 
         test data respectively. Assign the first eight of ten consecutive  
         sentences to the training set, the ninth to the development set    
         and the tenth to the training set.                                 
"""


"""
  (C) Copyright 2014, University of Helsinki                                 
  Licensed under the Apache License, Version 2.0 (the "License");            
  you may not use this file except in compliance with the License.           
  You may obtain a copy of the License at                                    
  http://www.apache.org/licenses/LICENSE-2.0                                 
  Unless required by applicable law or agreed to in writing, software        
  distributed under the License is distributed on an "AS IS" BASIS,          
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   
  See the License for the specific language governing permissions and        
  limitations under the License.                                             
"""

from sys import argv, stderr
from os  import linesep

EXIT_FAIL    = 1
EXIT_SUCCESS = 0

def main(argv, olog):
    if len(argv) != 2:
        olog.write(('USAGE: %s data_file' + linesep) % argv[0])
        return EXIT_FAIL
        
    ifile       = get_file(argv[1],              'r', olog)
    train_ofile = get_file("%s.train" % argv[1], 'w', olog)
    dev_ofile   = get_file("%s.dev" % argv[1],   'w', olog)
    test_ofile  = get_file("%s.test" % argv[1],  'w', olog)

    if not (ifile and train_ofile and dev_ofile and test_ofile):
        return EXIT_FAIL

    sentences = [[]]

    for line in map(lambda x: x.strip(), ifile):
        if len(line) == 0:
            sentences.append([])
        else:
            sentences[-1].append(line)

    for i, sentence in enumerate(sentences):
        # Pad sentences with and extra newline.
        sentence_str = linesep.join(sentence) + linesep + linesep

        if i % 10 == 9:
            test_ofile.write(sentence_str)
        elif i % 10 == 8:
            dev_ofile.write(sentence_str)
        else:
            train_ofile.write(sentence_str)

    ifile.close()
    train_ofile.close()
    dev_ofile.close()
    test_ofile.close()

    return EXIT_SUCCESS

def get_file(fn, mode, olog):
    try:
        return open(fn, mode)
    except IOError:
        if 'w' in mode:
            olog.write(("File %s can't be opened for writing!" + linesep) % fn)
        else:
            olog.write(("File %s can't be opened for reading!" + linesep) % fn)
        return None
    
if __name__ == '__main__':
    exit(main(argv, stderr))
