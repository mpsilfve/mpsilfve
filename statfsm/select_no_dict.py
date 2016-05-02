from sys import stdin
from itertools import imap

first = 1

for line in imap(lambda x:x.strip(), stdin):
    if line == '<SEP>':
        first = 1
    else:
        if first:
            print line
        first = 0
