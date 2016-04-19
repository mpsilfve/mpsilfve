from sys import stdin

for line in stdin:
    line = line.strip()
    
    ip = ''
    op = ''

    line = line.replace('| }','|}')

    for p in line.split(' '):
        if p == '':
            continue
        i = ''
        o = ''

        if '}' in p: 
            i, o = p.split('}')
        else:
            i = p
        if '|' in i:
            i = i.replace('|','')
        if '|' in o:
            o = o.replace('|','')
        
        o = o.replace('_','')
        i = i.replace('_','')

        if '_' in p:
            ip += i
            op += o
        else:
            ip += i
            op += o
            ip = ip.replace('0','"0"')
            op = op.replace('0','"0"')
            if ip == '':
                ip = '0'
            if op == '':
                op = '0'
            print ip + ' : ' + op
            ip = ''
            op = ''

    if ip != '' or op != '':
        print ip + ' : ' + op

    print
