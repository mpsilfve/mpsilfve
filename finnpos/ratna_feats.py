###############################################################################
# @file    ratna_feats.py                                                     #
# @Author  Miikka Silfverberg                                                 #
# @brief   Extract Ratnaparkhi 1996 features from an input-file.              #
#                                                                             #
#          (Adwait Ratnaparkhi: A Maximum Entropy Model for Part-of-Speech    #
#           Tagging, EMNLP 1996)                                              #
###############################################################################

###############################################################################
#                                                                             #
#  (C) Copyright 2014, University of Helsinki                                 #
#  Licensed under the Apache License, Version 2.0 (the "License");            #
#  you may not use this file except in compliance with the License.           #
#  You may obtain a copy of the License at                                    #
#  http://www.apache.org/licenses/LICENSE-2.0                                 #
#  Unless required by applicable law or agreed to in writing, software        #
#  distributed under the License is distributed on an "AS IS" BASIS,          #
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#  See the License for the specific language governing permissions and        #
#  limitations under the License.                                             #
#                                                                             #
###############################################################################

from sys import stdin, stdout, stderr
from re  import match
from os  import linesep

EXIT_FAIL    = 1
EXIT_SUCCESS = 0

# Read input from ifile and write extracted features to ofile. This function 
# doesn't check its argument streams.
def main(ifname, ifile, ofname, ofile, olog):
    olog.write(('Reading from %s. Writing to %s.' + linesep) 
               % 
               (ifname, ofname))

    sentences = [[]]

    for i, line in enumerate(map(lambda x: x.strip(), ifile)):
        if len(line) == 0:
            sentences.append([])
        else:
            fields = line.split('\t')

            if len(fields) != 3:
                olog.write(("Line %u in file %s: Incorrect field count %u. " +
                           "Should be 3." + linesep) 
                           % (i + 1, ifname, len(fields)))

                return EXIT_FAIL

            sentences[-1].append(fields)

    for sentence in filter(None, sentences):
        for i in range(len(sentence)):
            wf, lemma, label = sentence[i]
            
            features = []

            features.append('PPWORD=' + get_wf(i - 2, sentence))
            features.append('PWORD='  + get_wf(i - 1, sentence))
            features.append('WORD='   + wf)
            features.append('NWORD='  + get_wf(i + 1, sentence))
            features.append('NNWORD=' + get_wf(i + 2, sentence))

            features += get_suffixes(wf)
            features += get_prefixes(wf)

            features.append(has_uc(wf))
            features.append(has_digit(wf))
            features.append(has_dash(wf))

            feat_str = " ".join(filter(None, features))

            ofile.write(("%s\t%s\t%s\t%s\t_" + linesep) 
                        % 
                        (wf, feat_str, lemma, label))

        ofile.write(linesep)

    return EXIT_SUCCESS

def get_wf(i, sentence):
    return "_#_" if i < 0 or i + 1 > len(sentence) else sentence[i][0]

def get_suffixes(wf):
    return [ "%u-SUFFIX=%s" % (i, wf[-i:]) for i in range(1, 11) ]

def get_prefixes(wf):
    return [ "%u-PREFIX=%s" % (i, wf[:i]) for i in range(1, 11) ]

def has_uc(wf):
    return "HAS_UC" if match(".*([A-Z]|Å|Ä|Ö).*", wf) else None

def has_digit(wf):
    return "HAS_DIGIT" if match(".*[0-9].*", wf) else None

def has_dash(wf):
    return "HAS_DASH" if match(".*-.*", wf) else None

if __name__ == "__main__":
    exit(main("STDIN", stdin, "STDOUT", stdout, stderr))
