#! /usr/bin/env python3

from sys import stdin
from collections import defaultdict
from re import sub

def get_word_shape(wf):
    wf = wf.replace('-','').replace('‐','')

    if len(wf) == 0:
        return 'noabc'

    if wf.islower():
        return 'abc'
    if wf.isupper():
        return 'ABC'
    if wf[0].isupper():
        return 'Abc'
    if wf.isalnum():
        return 'aBc'
    
    return 'noabc'

def fix(tag):
    # Subtag Cmp sometimes duplicated.
    tag = tag.replace('Cmp Cmp', 'Cmp')

    # Subtag Sup sometimes duplicated.
    tag = tag.replace('Sup Sup', 'Sup')

    # Subtag Abbr sometimes duplicated.
    tag = tag.replace('Abbr Abbr', 'Abbr')
        
    # Remove all derivation tags.
    tag = sub(r'\[DRV=[^\]]*]','',tag)

    return tag

wf = ''
analyses = set()
lemma_dict = defaultdict(lambda : set())
lex_dict = defaultdict(lambda : '')
lemma_component_counts = defaultdict(lambda : 0)

for line in stdin:
    line = line.strip()

    if line == "":        
        feats = get_word_shape(wf)
        print('"<' + wf + '>"')
        for analysis in analyses:
            label = analysis[1]

            for lemma in lemma_dict[label]:
                if label in lex_dict:
                    if not ((label, lemma) in lex_dict):
                        continue
                lem = '"' + lemma + '"'

                if wf.find('-') == -1:
                    lem = lem.replace('-','')
                
                if wf.find('‐') == -1:
                    lem = lem.replace('‐','')

                lex = ""
                if (label, lemma) in lex_dict:
                    lex = lex_dict[(label, lemma)]
                    
                if label.find('+?') != -1:
                    if wf == '<<s>>':
                        tag = '<s> Sent noabc'
                    else:
                        tag = ' '.join(filter(lambda x: len(x) > 0, 
                                              ['Missing', feats]))

                else:
                    print_label = label
                    if lex != '':
                        if ' ' in label:
                            print_label = label.replace(' ', ' ' + lex + ' ',1)
                        else:
                            print_label = label + ' ' + lex
                    tag = ' '.join(filter(lambda x: len(x) > 0, 
                                          filter(lambda x: x != '', 
                                                 [lem, print_label, feats])))
                    tag += ' #' + str(lemma_component_counts[lemma])
                print("\t%s" % tag)

        analyses = set()
        lemma_dict = defaultdict(lambda : set())
        lex_dict = defaultdict(lambda : '')
        lemma_component_counts = defaultdict(lambda : 0)
    else:
        wf, tag = line.split('\t')        
        tag = fix(tag)

        if tag.find('+?') != -1:
            analyses.add((wf, tag))
            lemma_dict[tag].add('')
        else:
            sub_tags = tag.split(' ')
            lemma = '_'.join(sub_tags[:wf.count(' ') + 1])
            
            if lemma != '#':
                component_count = lemma.count('#') + 1
 
                # Remove lemma segment boundaries
                lemma = lemma.replace('#','')
                
                # Remove duplicate dashes occurring some numeral
                # forms.
                lemma = lemma.replace('--','-')
                
                if not 'Prop' in sub_tags:
                    lemma = lemma.lower()

                if (lemma_component_counts[lemma] == 0 or 
                    lemma_component_counts[lemma] > component_count):
                    lemma_component_counts[lemma] = component_count
                    
            lex = ' '.join([x for x in sub_tags[1:] if x.find('Lex_') != -1 ])
            label = ' '.join([x for x in sub_tags[wf.count(' ') + 1:] if x.find('Lex_') == -1])

            if lex != '':
                lex_dict[(label, lemma)] = lex
                lex_dict[label] = lex

            lemma_dict[label].add(lemma)
            analyses.add((wf, label))
