"""@package ascii_conversion

 file    ascii2im.py
 Author  Miikka Silfverberg
 brief   Convert a bitmap image to ASCII.

 (C) Copyright 2014, Miikka Silfverberg
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and 
 limitations under the License. 
"""

import re
from sys import argv, stderr

import PIL
import PIL.ImageFont, PIL.ImageDraw

def get_html(string):
    res = ""

    res +='<!DOCTYPE html>'
    res +='<html>'
    res +='<head>'
    res +='<style>'
    res +='html {'
    res +='    background: #151515;'
    res +='    color: white;'
    res +='}'
    res +='table, th, td {'
    res +='    border: 0px solid black;'
    res +='    border-collapse: collapse;'
    res +='    font-family: Courier;'
    res +='}'
    res +='th, td {'
    res +='    padding: 0px;'
    res +='}'
    res +='td#red-fill {'
    res +='    background :#FE2E2E;'
    res +='}'
    res +='td#blue-fill {'
    res +='    background : #2E2EFE;'
    res +='}'
    res +='td#green-fill {'
    res +='    background : #00FF40;'
    res +='}'
    res +='td#yellow-fill {'
    res +='    background :#F7FE2E;'
    res +='}'
    res +='td#magenta-fill {'
    res +='    background : #FE2EF7;'
    res +='}'
    res +='td#white-fill {'
    res +='    background : #D8D8D8;'
    res +='}'
    res +='td#red-text {'
    res +='    color :#FE2E2E;'
    res +='}'
    res +='td#blue-text {'
    res +='    color : #2E2EFE;'
    res +='}'
    res +='td#green-text {'
    res +='    color : #00FF40;'
    res +='}'
    res +='td#yellow-text {'
    res +='    color :#F7FE2E;'
    res +='}'
    res +='td#magenta-text {'
    res +='    color : #FE2EF7;'
    res +='}'
    res +='</style>'
    res +='</head>'
    res +='<body>'
    res +='<table>'

    if '\\n' in string:
        rows = string.split('\\n')
    else:
        rows = string.split('\n')

    for row in rows:
        res +='<tr>'

        tokens = re.split("(\\\\e\[[0-9]*m)", row)

        style = "regular"

        for token in tokens:
            if re.match('\\\\e\[[0-9]*m', token):
                if token == '\\e[0m':
                    style = 'regular'
                if token == '\\e[91m':
                    style = 'red-text'
                elif token == '\\e[92m':
                    style = 'green-text'
                elif token == '\\e[93m':
                    style = 'yellow-text'
                elif token == '\\e[94m':
                    style = 'blue-text'
                elif token == '\\e[95m':
                    style = 'magenta-text'
                elif token == '\\e[97m':
                    style = 'regular'
                elif token == '\\e[101m':
                    style = 'red-fill'
                elif token == '\\e[102m':
                    style = 'green-fill'
                elif token == '\\e[103m':
                    style = 'yellow-fill'
                elif token == '\\e[104m':
                    style = 'blue-fill'
                elif token == '\\e[105m':
                    style = 'magenta-fill'
                elif token == '\\e[107m':
                    style = 'white-fill'
            else:
                chars = re.split('(.)', token)
                for char in filter(lambda x: len(x) > 0, chars):
                    res +='<td' + ' id="' + style + '">' + char +'</td>'
        res +='</tr>'

    res +='</table>'
    res +='</body>'
    res +='</html>'

    return res

def get_bitmap(string):    
    if '\\n' in string:
        rows = string.split('\\n')
    else:
        rows = string.split('\n')


    row_len = len(rows[0])

    if '\\n' in string:
        tokens0 = re.split("(\\\\e\[[0-9]*m)", rows[0])
        row_len = len(filter(lambda x: len(x) > 0 and x.find('e[') == -1, tokens0))

    bitmap = PIL.Image.new(mode="RGB", size=(10*row_len, 20*len(rows)))
    draw = PIL.ImageDraw.Draw(bitmap)
    font = PIL.ImageFont.truetype("Monospace.ttf", 20)

    coord = [0,0]

    for row in rows:

        tokens = re.split("(\\\\e\[[0-9]*m)", row)

        style = "regular"

        for token in tokens:
            if re.match('\\\\e\[[0-9]*m', token):
                if token == '\\e[0m':
                    style = 'regular'
                if token == '\\e[91m':
                    style = 'red-text'
                elif token == '\\e[92m':
                    style = 'green-text'
                elif token == '\\e[93m':
                    style = 'yellow-text'
                elif token == '\\e[94m':
                    style = 'blue-text'
                elif token == '\\e[95m':
                    style = 'magenta-text'
                elif token == '\\e[97m':
                    style = 'regular'
                elif token == '\\e[101m':
                    style = 'red-fill'
                elif token == '\\e[102m':
                    style = 'green-fill'
                elif token == '\\e[103m':
                    style = 'yellow-fill'
                elif token == '\\e[104m':
                    style = 'blue-fill'
                elif token == '\\e[105m':
                    style = 'magenta-fill'
                elif token == '\\e[107m':
                    style = 'white-fill'
            else:
                chars = re.split('(.)', token)

                for char in filter(lambda x: len(x) > 0, chars):
                    font_color = "white"

                    xy = (tuple(coord), (coord[0]+10, coord[1]+20))

                    if style == 'red-text':
                        font_color='red'
                    elif style == 'green-text':
                        font_color='green'
                    elif style == 'blue-text':
                        font_color='blue'
                    elif style == 'yellow-text':
                        font_color='yellow'
                    elif style == 'magenta-text':
                        font_color='magenta'                            
                    elif style == 'red-fill':
                        draw.rectangle(xy, fill="red")
                    elif style == 'green-fill':
                        draw.rectangle(xy, fill="green")
                    elif style == 'blue-fill':
                        draw.rectangle(xy, fill="blue")
                    elif style == 'yellow-fill':
                        draw.rectangle(xy, fill="yellow")
                    elif style == 'magenta-fill':
                        draw.rectangle(xy, fill="magenta")
                    elif style == 'white-fill':
                        draw.rectangle(xy, fill="white")

                    draw.text(coord, char, font=font,fill=font_color)
                    coord = [coord[0]+10,coord[1]]

        coord = [0, coord[1]+20]

    return bitmap

if len(argv) != 3:
    stderr.write('%s format file\n' %argv[0])
    exit(1)
    
fmt = argv[1]

if fmt == 'html':
    out = open(argv[2] + ".html", "w")
    out.write(get_html(open(argv[2]).read()))
    stderr.write("Wrote " + argv[2] + ".html\n")
else:
    bitmap = get_bitmap(open(argv[2]).read())
    stderr.write("Trying to write bitmap in %s format.\n" % fmt)
    bitmap.save(argv[2] + "." + fmt)
    stderr.write("Wrote " + argv[2] + "." + fmt + "\n")

