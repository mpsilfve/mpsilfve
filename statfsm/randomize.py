from sys import stdin
from random import shuffle

lines = stdin.read().split('\n')
shuffle(lines)

for line in lines:
    print line
