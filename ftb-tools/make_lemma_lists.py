#! /usr/bin/env python3

from sys import stdin
from collections import defaultdict
from pickle import dump


def get_wf(line):
    return line[2:-2]

def split_analysis(line):
    line = line.strip()
    toks = line.split(' ')
    return toks[0][1:-1], ' '.join(toks[1:])

wf = ''
lemma_lists = {}

for line in stdin:
    line = line.strip('\n')

    if line == '':
        print()
    elif line[0] == '"':
        wf = get_wf(line)
    else:
        lemma, tag = split_analysis(line)

        if not wf in lemma_lists:
            lemma_lists[wf] = set()

        lemma_lists[wf].add(lemma)

out = open('lemma_list', 'wb')
dump(lemma_lists, out)
