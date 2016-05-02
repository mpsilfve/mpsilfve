#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sys import stdin
from itertools import imap
import re

def escape(label):
    if "SL:" in label:
        return label
    if label[0] == '"' or label == '%"':
        return str(label)
    if label == '0':
        return str('EPS')
    return str('"' + label + '"')

def tok(chars):
    return [escape(tok) for tok in re.split('("0"|%.|.)', chars) if tok != '']

def output_tok(chars):
    if 'SL:' in chars:
        return [chars]
    if '|' in chars:
        return [escape(tok) for tok in chars.split('|')]
    return [escape(chars)]

def split_ustr_expr(expr):
    fields = expr.split(' ')
    return (tok(fields[0]), output_tok(fields[1]), float(fields[-1]))

def split_str_expr(expr):
    fields = expr.split(' ')
    return ([[escape(t) for t in tok.split('|')] for tok in fields[:-1]], 
            float(fields[-1]))

def any_input():
    return "\£"

def any_output():
    return "\£"

def get_sublabel_o(o, centered):
    if centered:
        return any_output() + ' _ ' + any_output()
    else:
        return any_output() + ' ' + escape(o[3:]) + ' ' + any_output()

def get_o(o, centered):
    if centered:
        return '_'
    else:
        return ' '.join(o)

def compile(i = None, o = None, centered = 1):
    if i == None:
        i = any_input()

    if o == None:
        o = any_output()
    elif 'SL' in o[0]:
        o = get_sublabel_o(o[0], centered)
    else:
        o = get_o(o, centered)

    return "%s %s £" % (i, o)
        
def compile_context(identifier, chars, output):
    if identifier == 'FEAT1':
        return "%s ;" % compile(i = chars[0], o = output, centered = 1)
    elif identifier == 'FEAT2':
        assert(len(chars) == 2)
        return "%s %s ;" % (compile(i = chars[0], centered = 0),
                            compile(i = chars[1], o = output, centered = 1))
    elif identifier == 'FEAT3':
        assert(len(chars) == 2)
        return "%s %s ;" % (compile(i = chars[0], o = output, centered = 1),
                            compile(i = chars[1], centered = 0))
    elif identifier == 'FEAT4':
        assert(len(chars) == 3)
        return "%s %s %s ;" % (compile(i = chars[0], centered = 0),
                               compile(i = chars[1], o = output, centered = 1),
                               compile(i = chars[2], centered = 0))
    elif identifier == 'FEAT5':
        assert(len(chars) == 3)
        return "%s %s %s ;" % (compile(i = chars[0], centered = 0),
                               compile(i = chars[1], centered = 0),
                               compile(i = chars[2], o = output, centered = 1))
    elif identifier == 'FEAT6':
        assert(len(chars) == 3)
        return "%s %s %s ;" % (compile(i = chars[0], o = output, centered = 1),
                               compile(i = chars[1], centered = 0),
                               compile(i = chars[2], centered = 0))
    elif identifier == 'FEAT7':
        assert(len(chars) == 3)
        return "%s %s %s %s ;" % (compile(o = output, centered = 1),
                                  compile(i = chars[0], centered = 0),
                                  compile(i = chars[1], centered = 0),
                                  compile(i = chars[2], centered = 0))
    elif identifier == 'FEAT8':
        assert(len(chars) == 3)
        return "%s %s %s %s ;" % (compile(i = chars[0], centered = 0),
                                  compile(i = chars[1], centered = 0),
                                  compile(i = chars[2], centered = 0),
                                  compile(o = output, centered = 1))
    elif identifier == 'STRUCT':
        if len(chars) == 1:
            return "%s ;" % compile(o = output, centered = 1)
        elif len(chars) == 2:
            return "%s %s ;" % (compile(o = chars[0], centered = 0),
                                compile(o = output, centered = 1))
        elif len(chars) == 3:
            return "%s %s %s ;" % (compile(o = chars[0], centered = 0),
                                   compile(o = chars[1], centered = 0),
                                   compile(o = output, centered = 1))
        else:
            assert(0)
    else:
        assert(0)

def compile_center(o, w):
    if 'SL:' in o[0]:
        o = [escape(o[0][3:])]

    return "%s -> %s::%.0f" % (' '.join(o), ' '.join(o), -w)

ustr = 1

for line in imap(lambda x: x.strip(), stdin):
    if line == '':
        continue
    elif line == 'UNSTRUCTURED FEATURES':
        ustr = 1
        print line
    elif line == 'STRUCTURED FEATURES':
        ustr = 0
        print line
    else:
        context = None
        center = None
        if ustr:
            identifier = line[:5]
            expr = split_ustr_expr(line[6:])
            context = compile_context(identifier, expr[0], expr[1])
            center = compile_center(expr[1], expr[2])
        else:
            chars, weight = split_str_expr(line)
#            chars.reverse()
            context = compile_context('STRUCT', chars, chars[-1])
            center = compile_center(chars[-1], weight)
        print "%s || %s" % (center, context)
            
