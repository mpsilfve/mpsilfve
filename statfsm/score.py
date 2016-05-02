from sys import stdin, argv
from itertools import ifilter, imap

struct = 0

unstruct_params = {}
struct_params = {}

def print_score(fields, unstruct_params, struct_params):
    label_t_2 = '_#_'
    label_t_1 = '_#_'
    
    score = 0
    fields += [('',[],'','_#_'),('',[],'','_#_')]
    for i,fs in enumerate(fields):
        label = fs[3]
        if i > 1:
            label_t_1 = fields[i-1][3]
        if i > 2:
            label_t_2 = fields[i-2][3]

        for feat_templ in fs[1]:            
            feat = "%s::%s" % (feat_templ, label)            
            if feat in unstruct_params:
                print feat, unstruct_params[feat]
#                continue
                score += unstruct_params[feat]
        if label in struct_params:
            print label, struct_params[label]
            score += struct_params[label]
            
        blabel = "%s::%s" % (label_t_1,label)
        if blabel in struct_params:
            print blabel, struct_params[blabel]
            score += struct_params[blabel]

        tlabel = "%s::%s::%s" % (label_t_2,label_t_1,label)
        if tlabel in struct_params:
            print tlabel, struct_params[tlabel]
            score += struct_params[tlabel]

        label_t_2 = label_t_1
        label_t_1 = label

    print score

for line in ifilter(lambda x: x != '', 
                    imap(lambda x: x.strip(), open(argv[1]))):
    if line == 'UNSTRUCTURED FEATURES':
        struct = 0
    elif line == 'STRUCTURED FEATURES':
        struct = 1
    else:
        if not struct:
            feat, label, param = line.split(' ')
            unstruct_params["%s::%s" % (feat, label)] = float(param)
        else:
            fields = line.split(' ')
            param = fields[-1]
            labels = fields[:-1]
            struct_params['::'.join(labels)] = float(param)

fields = []

for line in imap(lambda x: x.strip(), stdin):
    if line == '':
        print_score(fields, unstruct_params, struct_params)
        fields = []
    else:
        fields.append(line.split('\t'))
        fields[-1][1] = fields[-1][1].split(' ')

if fields != []:
    print_score(fields, unstruct_params, struct_params)
