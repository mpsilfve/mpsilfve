#! /bin/bash

sed 's/^..*$/#&#/'    | # Add word boundary symbols.
sed 's/./&\n/g'       | # Move each letter on its own line.
sed 's/0/"0"/g'       | # Escape zeros.
sed 's/^..*$/& : &/'  | # Add dummy labels because extract_feats.py requires 
                        # labeled input.
./extract_features.py | 
./untag.sh                                 | # Remove dummy labels.
finnpos-label klk-substitution.final.model | # Make substitutions.
cut -f1,4                                  | # Preserve only input and output 
                                             # symbol.
sed 's/\(.*\)	\(.*\)/<\1|\2> : O/' | # Prepare data for insertion feature 
                                       # extraction and add dummy labels.
./extract_features.py                | 
./untag.sh                              | # Remove dummy labels.
finnpos-label klk-insertion.final.model | # Perform insertion.
cut -f1,4                               | # Preserve only input and output  
                                          # symbol.
sed 's/[^   ]*[|]\([^>]*\)>/\1/'              | # Decode insertions.
sed 's/^\([^     ]*\)    INSERT-\(.*\)/\2\1/' | 
cut -f1                                       | 
tr -d '\n'      | # Go from letter/line to word/line format.
sed 's/##/\n/g' | 
tr -d '#'       | 
sed 's/"0"/#/g' | # Remove epsilons and handle zeros.
tr -d '0'       | 
tr '#' '0'