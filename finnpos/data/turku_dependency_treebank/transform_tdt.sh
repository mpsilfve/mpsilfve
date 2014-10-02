#! /bin/bash

##############################################################################
#                                                                            #
# @file    transform_tdt.sh                                                  #
# @Author  Miikka Silfverberg                                                #
# @brief   Generate the input for ratna_feats.py from TDT in ConllX format.  #
#                                                                            #
##############################################################################

##############################################################################
#                                                                            #
# (C) Copyright 2014, University of Helsinki                                 #
# Licensed under the Apache License, Version 2.0 (the "License");            #
# you may not use this file except in compliance with the License.           #
# You may obtain a copy of the License at                                    #
# http://www.apache.org/licenses/LICENSE-2.0                                 #
# Unless required by applicable law or agreed to in writing, software        #
# distributed under the License is distributed on an "AS IS" BASIS,          #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
# See the License for the specific language governing permissions and        #
# limitations under the License.                                             #
#                                                                            #
##############################################################################

# Remove comment-lines.
grep -v "^#" | 

# Select word form, lemma, main pos and morphological features.
cut -f2,3,5,6 | 

# Join main pos and morhological features.
sed 's/	\([^	]*\)$/|\1/' | 

# Remove hash tags that separate parts of the lemma of compound words. 
awk -v var="$mycol_new" -F $'\t' 'BEGIN {OFS = FS} 
                                  { if ($2 != "#") 
                                      { gsub("#","",$2) } } 
                                  { print }'