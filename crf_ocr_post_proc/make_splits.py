#! /usr/bin/env python3

from sys import argv, stderr

if len(argv) != 2:
    stderr.write('USAGE: %s data_file' % argv[0])
    exit(1)

ifile = open(argv[1])
otrain = open('%s.train' % argv[1], 'w')
odev = open('%s.dev' % argv[1], 'w')

ofile = otrain

entry_count = 0

for line in ifile:
    ofile.write(line)

    if line == '\n':
        entry_count += 1
        if entry_count % 10 == 9:
            ofile = odev
        else:
            ofile = otrain
