#! /usr/bin/env python3

from sys import stdin, argv
from pickle import load

def get_wf(line):
    return line[2:-2]

def split_analysis(line):
    line = line.strip()
    toks = line.split(' ')
    if toks[0] == 'Missing':
        return toks[0], ' '.join(toks[1:])
    else:
        return toks[0][1:-1], ' '.join(toks[1:])

def print_cohort(wf, analyses):    
    if wf == '':
        return
    print('"<%s>"' % wf)
    for lemma, tag in analyses:
        if lemma != 'Missing':
            lemma = '"' + lemma + '"'
        print("\t%s %s" % (lemma, tag))

def get_part_count(tag):
    if tag.find('#') == -1:
        return 1
    else:
        return int(tag[tag.find('#') + 1:])

if len(argv) != 2:
    stderr.write("USAGE: cat data | %s lemma_lists\n" % argv[0])
    exit(1)

lemma_lists = load(open(argv[1], "rb"))

wf = ''
analyses = []
tag_to_lemma = {}

for line in stdin:
    line = line.strip('\n')

    if line == '':
        print()
    elif line[0] == '"':
        print_cohort(wf, analyses)

        wf = get_wf(line)
        analyses = []
        tag_to_lemma = {}
    else:
        lemma, tag = split_analysis(line)
        lemma_part_count = get_part_count(tag)

        if wf in lemma_lists:
            if lemma != 'Missing' and not lemma in lemma_lists[wf]:
                continue
            analyses.append((lemma, tag))
        else:
            if (tag in tag_to_lemma and 
                lemma_part_count >= tag_to_lemma[tag][1]):
                continue
            analyses.append((lemma, tag))
            tag_to_lemma[tag] = (lemma, lemma_part_count)

if analyses != []:    
    print_cohort(wf, analyses)
