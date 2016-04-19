import sys
reload(sys)
sys.setdefaultencoding('utf-8')

from sys import stdin
from itertools import imap

for line in imap(lambda x: x.strip(), stdin):
    if line == '':
        print
    else:
        if line.count(' ') == 1:
            line += ' 0'
        i_o = line.split(' : ')        
        if len(i_o) == 1:
            i_o.append('0')
        i, o = i_o
        
        if i == ' ' or i == '':
            i = '0'

        pairs = [[c, '0'] for c in unicode(i)]
        pairs[-1][1] = o

        for p in pairs:
            if p[0] == ' ':
                p[0] = '0'
            print '%s : %s' % (p[0],p[1])

            
