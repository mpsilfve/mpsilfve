#! /usr/bin/env python3

from sys import stdin

def output_word(ocr_word, gold_word):
    label = 'O'

    if ocr_word != []:
        for i in range(len(ocr_word)):
            if ocr_word[i] == '0':
                label = 'INSERT-%s' % gold_word[i]
            else:
                print ("<%s|%s> : %s" % (ocr_word[i], gold_word[i], label))
                label = 'O'
        print()

ocr_word = []
gold_word = []

for line in stdin:
    line = line.strip()

    if line == '':
            output_word(ocr_word, gold_word)
            ocr_word = []
            gold_word = []
    else:
        ocr_letter, _sep, gold_letter = line.split(' ')
        ocr_word.append(ocr_letter)
        gold_word.append(gold_letter)

if ocr_word != []:
    output_word(ocr_word, gold_word)
