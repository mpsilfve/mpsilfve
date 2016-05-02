import sys
reload(sys)
sys.setdefaultencoding('utf-8')

from sys import stdin

for line in stdin:
    line = line.strip()

    if line == '':
        print line
    else:
        i, sep, o = line.split(' ')
        o = [c for c in unicode(o)]
        o = '|SL'.join(o)
        if '|' in o:
            o = 'SL' + o
        print ' '.join([i, sep, o])
