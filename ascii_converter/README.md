A Utility for Generating ASCII Images
=====================================

This utility includes two tools:

<code>im2ascii.py</code> - Convert a bitmap image to plain ASCII or ASCII with bash
              color codes.

<code>ascii2im.py</code> - Convert a plain ASCII image or ASCII image with bash
              color codes to a bitmap image.

USAGE:

Image to ASCII
--------------

To convert pelosi.jpg to an ASCII image of width 100 characters and
bash color codes, use

   <code>python im2ascii.py pelosi.jpg 100 1 > pelosi.ascii</code>

If you don't want colors, only plain ASCII, use

   <code>python im2ascii.py pelosi.jpg 100 0 > pelosi.ascii</code>

To view an ASCII image with color codes in the terminal (in bash
shell), use

   <code>echo -e `cat pelosi.ascii`</code>

To view a plain ASCII image, use cat or your favorite editor.

ASCII to Image
--------------

To convert pelosi.ascii into a bitmap image in for example png-format
use

  <code>python ascii2im.py png pelosi.ascii</code>

This will create a png-file pelosi.ascii.png.

You can also generate an html-document like this

  <code>python ascii2im.py html pelosi.ascii</code>

The command generates an html-document with a single table
representing your ASCII image.