# -*- coding: utf-8 -*-

import libhfst
from sys import argv, stderr

base = "[\£ \£ £]*"
all_ustr_fsts = []
all_str_fsts = []
ustr_fst = libhfst.regex(base)
str_fst = libhfst.regex(base)
ustr_fsts = libhfst.regex(base)
str_fsts = libhfst.regex(base)

fst_count = 0

ustr = 1

for line in open(argv[1],'r'):
    stderr.write("LINE: %u\r" % fst_count)
    line = line.strip()

    if line == '':
        continue

    if line == 'UNSTRUCTURED FEATURES':
        ustr = 1
    elif line == 'STRUCTURED FEATURES':
        ustr = 0
    else:
        if ustr:
            if fst_count % 100 == 0:
                ustr_fsts.intersect(ustr_fst)
                ustr_fsts.minimize()
                ustr_fst = libhfst.regex(base)
            if fst_count % 500 == 0:                
                all_ustr_fsts.append(ustr_fsts)
                ustr_fsts = libhfst.regex(base)

            feat = libhfst.regex(line)
            ustr_fst.intersect(feat)

            if fst_count % 10 == 0:
                ustr_fst.minimize()
        else:
            if fst_count % 100 == 0:
                str_fsts.intersect(str_fst)
                str_fsts.minimize()
                str_fst = libhfst.regex(base)
            if fst_count % 500 == 0:                
                all_str_fsts.append(str_fsts)
                str_fsts = libhfst.regex(base)

            feat = libhfst.regex(line)
            str_fst.intersect(feat)

            if fst_count % 10 == 0:
                str_fst.minimize()

        fst_count += 1
stderr.write('\n')

ustr_fsts.intersect(ustr_fst)
all_ustr_fsts.append(ustr_fsts)

str_fsts.intersect(str_fst)
all_str_fsts.append(str_fsts)

ustr_model = libhfst.regex(base)
str_model = libhfst.regex(base)

for i, fst in enumerate(all_ustr_fsts):
    print "USTR: %u of %u" % (i, len(all_ustr_fsts))
    ustr_model.intersect(fst)
    ustr_model.minimize()

for i, fst in enumerate(all_str_fsts):
    print "STR: %u of %u" % (i, len(all_str_fsts))
    str_model.intersect(fst)
    str_model.minimize()

out = libhfst.create_hfst_output_stream(argv[1] + '.ustr',
                                        libhfst.TROPICAL_OPENFST_TYPE, 1)
out.write(ustr_model)

out = libhfst.create_hfst_output_stream(argv[1] + '.str',
                                        libhfst.TROPICAL_OPENFST_TYPE, 1)
out.write(str_model)
