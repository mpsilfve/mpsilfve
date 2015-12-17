#! /usr/bin/env python3

from sys import stdin

def print_cohort(wf, analyses):
    print(wf)
    for analysis in analyses:
        print(analysis)

wf = ''
analyses = []

for line in stdin:
    line = line.strip('\n')

    if line == '':
        print()
    elif line[0] == '"':
        analyses.sort()
        print_cohort(wf, analyses)

        wf = line
        analyses = []
    else:
        analyses.append(line)

if analyses != []:
    analyses.sort()
    print_cohort(wf, analyses)
