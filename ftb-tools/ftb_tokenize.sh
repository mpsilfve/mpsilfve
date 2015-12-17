#! /bin/bash


sed 's/$/ sentenceends/'        | 
hfst-proc2 tokenizer.pmatch.fst |
grep -v "^$"                    |
grep -v "^[|]$"                 |
sed 's/sentenceends//' 
