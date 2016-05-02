from sys import stdin, argv
from itertools import imap

import libhfst

dictionary = libhfst.HfstInputStream(argv[1]).read()

first_candidate = None
dict_candidate = None

for line in imap(lambda x:x.strip(), stdin):
    if line == '<SEP>':
        if dict_candidate != None:
            print dict_candidate
        else:
            print first_candidate
        dict_candidate = None
        first_candidate = None
    else:
        if first_candidate == None:
            first_candidate = line
        if dict_candidate == None and len(dictionary.lookup(line)) != 0:
            dict_candidate = line

if dict_candidate != None:
    print dict_candidate
elif first_candidate != None:
    print first_candidate
