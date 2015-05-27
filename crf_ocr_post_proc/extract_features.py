#! /usr/bin/env python3

from sys import stdin, stderr
from re import match

def extract_feats(input_word, gold_word):
    if len(input_word) != len(gold_word):
        print(input_word + " " + gold_word)
        exit(1)

    if len(input_word) == 0:
        return

    iletters = []
    gletters = []

    if match("<.*[|].*>", input_word[0]):
        for i_and_g in input_word:
            i_and_g = i_and_g[1:-1]
            i, g = i_and_g.split('|')
            iletters.append(i)
            gletters.append(g)

    for i in range(len(input_word)):
        il = input_word[i]
        gl = gold_word[i]

        f1 = 'FEAT1:' + input_word[i]
        f2 = 'FEAT2:' + ''.join(input_word[i-1:i+1])
        f3 = 'FEAT3:' + ''.join(input_word[i:i+2])
        f4 = 'FEAT4:' + ''.join(input_word[i-1:i+2])

        feats = [f1, f2, f3, f4]

        if iletters:
            f1 = 'IFEAT1:' + iletters[i]
            f2 = 'IFEAT2:' + ''.join(iletters[i-1:i+1])
            f3 = 'IFEAT3:' + ''.join(iletters[i:i+2])
            f4 = 'IFEAT4:' + ''.join(iletters[i-1:i+2])
            f5 = 'GFEAT1:' + gletters[i]
            f6 = 'GFEAT2:' + ''.join(gletters[i-1:i+1])
            f7 = 'GFEAT3:' + ''.join(gletters[i:i+2])
            f8 = 'GFEAT4:' + ''.join(gletters[i-1:i+2])

            feats += [f1, f2, f3, f4, f5, f6, f7, f8]
            
        print('%s\t%s\t%s\t%s\t%s' % (il, ' '.join(feats), il, gl, '_'))

    print('')

gold_word = []
input_word = []

for line in stdin:
    line = line.strip()

    if line == '':
        extract_feats(input_word, gold_word)
        input_word = []
        gold_word = []
    else:
        input_l, _sep, gold_l = line.split(' ')

        gold_word.append(gold_l)
        input_word.append(input_l)
