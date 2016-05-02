#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sys import argv, stdin, stdout, stderr
from collections import defaultdict
from itertools import imap

NBEST=80

import libhfst

def escape(label):
    if "SL:" in label:
        return label
    if label[0] == '"' or label == '%"':
        return str(label)
    if label == '0':
        return str('EPS')
    if label == 'EPS':
        return label
    return str('"' + label + '"')

def get_sorted_paths(re):
    paths = re.extract_paths()
    wpaths = []
    for k, p in paths.iteritems():
        wpaths.append((p[0][1],p[0][0]))
    wpaths.sort()
    return wpaths

TH=0.95

trcounts = [line.split('\t') for line in open(argv[1]).read().split('\n')]
icounts = defaultdict(lambda : 0.0)
iocounts = defaultdict(lambda : defaultdict(lambda: 0.0))

for count, i, o  in filter(lambda x: len(x) > 1, trcounts):
    if i == '0':
        i = 'EPS'
    if o == '0':
        o = 'EPS'
    icounts[i] += float(count)
    iocounts[i][o] += float(count)

for i in iocounts:
    for o in iocounts[i]:
        iocounts[i][o] /= icounts[i]

outputs = defaultdict(lambda : [])

for i in iocounts:
    odistr = sorted([(iocounts[i][o], o) for o in iocounts[i]])
    odistr.reverse()

    tot = 0

    for m, o in odistr:
        outputs[i].append(o)
        tot += m
        if m >= TH:
            break

outputs['#'].append('#')
outputs['_#_'].append('_#_')

out = libhfst.create_hfst_output_stream("",
                                        libhfst.TROPICAL_OPENFST_TYPE, 1)

ustr_model = libhfst.HfstInputStream(argv[2]).read()
str_model = libhfst.HfstInputStream(argv[3]).read()

for i, line in enumerate(imap(lambda x: x.strip(), stdin)):
    stderr.write("LINE: %u\r" % i)
    expr = ''

    if line == '':
        continue
    chars = ('_#_ _#_ # ' + line.replace('0','"0"') + ' # _#_ _#_').split(' ')
    
    for char in chars:
        expr += ('%s [%s] £ ' % (escape(char),
                                 '|'.join([escape(c) for c in outputs[char]])))
    re = libhfst.regex(expr)
    re.compose(ustr_model)
    re.minimize()
    re.compose(str_model)
    re.minimize()
    re.n_best(NBEST)
    for p in get_sorted_paths(re):
        print p[1]
    print "<SEP>"
    stdout.flush()
stderr.write('\n')
