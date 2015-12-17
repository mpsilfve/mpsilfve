#! /usr/bin/env python3

from sys import stdin, argv, stderr
from pickle import load
from re import match

replacements = { 
    'parempansa' : ['\t"parempi" A Cmp Sg Gen Px3 abc',
                    '\t"parka" A Cmp Pl Nom Px3 abc',
                    '\t"parka" A Cmp Sg Nom Px3 abc'],
    'aiemmin' : ['\t"aiemmin" Adv Cmp abc',
                 '\t"aiempi" A Cmp Pl Ins abc',
                 '\t"aiempi" A Cmp Sg Nom abc'],
    'paremmin' : ['\t"paremmin" Adv Cmp abc',
                  '\t"parempi" A Cmp Sg Nom abc',
                  '\t"parka" A Cmp Pl Ins abc'],
    '&' : ['\t"&" Adv noabc',
           '\t"&" CC noabc']
    }

def get_wf(line):
    return line[2:-2]

def replace_and_print_cohort(wf, analyses):    
    if wf == '':
        return
    elif wf in replacements:
        analyses = replacements[wf]
    elif match('^[0-9]+[.]$', wf):
        analyses = [('\t"%s" Num Ord noabc' % wf)]
    elif match('^[0-9][0-9,.]*$', wf):
        analyses = [('\t"%s" Num Card noabc' % wf)]
    elif len(analyses) > 0 and analyses[0].find("Missing ") != -1:
        analyses = ["\tMissing"]

    print('"<%s>"' % wf)
    for line in analyses:
        print(line)

wf = ''
analyses = []

for line in stdin:
    line = line.strip('\n')

    if line == '':
        print()
    elif line[0] == '"':
        replace_and_print_cohort(wf, analyses)

        wf = get_wf(line)
        analyses = []
    else:
        analyses.append(line)

if analyses != []:    
    replace_and_print_cohort(wf, analyses)
