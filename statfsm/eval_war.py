from sys import argv
from itertools import izip, imap

sys_file = open(argv[1])
gold_file = open(argv[2])

tot = 0.0
corr = 0.0

for sys_line, gold_line in izip(imap(lambda x: x.strip(), sys_file),
                                imap(lambda x: x.strip(), gold_file)):
    if sys_line == gold_line:
        corr += 1
    tot += 1

print "word accuracy rate: %.4f" % (corr/tot)
