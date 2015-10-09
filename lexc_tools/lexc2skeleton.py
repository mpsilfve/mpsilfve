#! /usr/bin/env python3

from sys import stdin, stderr, argv
from collections import namedtuple
from re import sub, match, split, findall
from getopt import getopt

class LexcEntry:
    """
    Stores one lexicon entry.
    """
    input        = ''
    output       = ''
    continuation = ''
    info         = ''

def print_usage(prog_name):
    stderr.write(
        'USAGE cat file.lexc | %s (help | with-flags)? > skeleton.lexc\n' %   
        prog_name)
    
def print_help(prog_name):
    print_usage(prog_name)
    stderr.write('\n'
                 + 'Transform a lexc-file into a skeleton file which only\n' 
                 + 'preserves the continuation classes of the lexicon.\n'
                 + '\n\n'
                 + '-h                         Print help.\n'
                 + '-f                         Include flags in the\n'
                 + '                           skeleton lexicon.\n\n')

def remove_re_whitespace(lexicon_entry):
    """
    Remove all spaces inside regular expressions on a line but
    preserve all other spaces. 

    Ugly but what can you do...

    Example

    input:
    begin< a* ?* b* >end< ? * > LEX ;

    output:
    begin<a*?*b*>end<?*> LEX ;
    """
    in_re = 0
    result = ''
    
    for i, c in enumerate(lexicon_entry):
        if c == '<' and (i == 0 or lexicon_entry[i-1] != '%'):
            in_re = 1
        elif c == '>' and (i == 0 or lexicon_entry[i-1] != '%'):
            in_re = 0

        if c == ' ' and in_re:
            pass
        else:
            result += c

    return result

def get_fields(lexicon_entry):
    """ 
    Split a lexicon entry into
    
    1. An input string.
    2. And output string.
    3. A continuation class.
    4. An info string.
    """

    # Remove spaces from regular expression entries because they mess
    # up split().
    lexicon_entry = remove_re_whitespace(lexicon_entry)

    # Remove the semi-colons at the end of the line.
    lexicon_entry = sub('(?<!%);.*','', lexicon_entry)

    #  Remove all escaped spaces wich mess up split().
    lexicon_entry = lexicon_entry.replace('% ','')

    fields = lexicon_entry.split()
    result = LexcEntry()
    
    if len(fields) == 1:
        result.continuation = fields[0]

    if len(fields) >= 2:
        if match(r'(?<!%):', fields[0]):
            result.input, result.output = split(r'(?<!%):', fields[0])
        else:
            result.input  = fields[0]
            result.output = fields[0]
        result.continuation = fields[1] 

    if len(fields) >= 3:
        result.info = fields[2]

    return result

def get_flag_string(string):
    return ''.join(findall(r'[@][UPRNDC][.][^.@]+(?:[.][^.@]+)[@]', string))

def is_lexicon_identifier(string):
    return string.find('LEXICON ') != -1

def get_lexicon_name(string):
    return string.split()[1]

if __name__=='__main__':
    lines = []    
    lexicon_names = []
    
    opts = []

    include_flags = 0

    if '-h' in argv:
        print_help(argv[0])
        exit(0)
    elif '-f' in argv:
        include_flags = 1

    # Loop through the data once to collect all lexicon names. They
    # need to be inserted into Multichar_Symbols at the start of the
    # lexc skeleton file.
    stderr.write("Collecting lexicon names.\n")
    for line in stdin:
        # Remove comments.
        line = sub(r'(?<!%)!.*', '', line)
        line = line.strip()        
        
        if line == '':
            pass

        lines.append(line)

        if is_lexicon_identifier(line):
            lexicon_name = \
                lexicon_names.append(get_lexicon_name(line))
    
    lex_count = 0
    lexicon_name = ''

    for line in lines:
        # Declare all lexicon names multi-character symbols.
        if line.find('Multichar_Symbols') != -1:
            print(line)
            for name in lexicon_names:
                print(name)
            continue

        if is_lexicon_identifier(line):
            lex_count += 1
            stderr.write("Lexicon %u of %u\r" % (lex_count, 
                                                 len(lexicon_names)))
            lexicon_name = get_lexicon_name(line)
        elif lexicon_name:
            fields = get_fields(line)
            
            if fields.continuation != '':
                if include_flags:
                    line = "%s%s %s ;" % (lexicon_name,
                                          get_flag_string(line),
                                          fields.continuation)
                else:
                    line = "%s %s ;" % (lexicon_name,
                                        fields.continuation)
        print(line)
        
    stderr.write('\n')
