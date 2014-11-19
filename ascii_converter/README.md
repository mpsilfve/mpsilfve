A Utility for Generating ASCII Images
=====================================

`im2ascii.py` - Convert a bitmap image to plain ASCII or ASCII with bash
                color codes.

`ascii2im.py` - Convert a plain ASCII image or ASCII image with bash
                color codes to a bitmap image.

Image to ASCII
--------------

To convert pelosi.jpg to an ASCII image of width 100 characters and
bash color codes, use

   `python im2ascii.py pelosi.jpg 100 1 > pelosi.ascii`

If you don't want colors, only plain ASCII, use

   `python im2ascii.py pelosi.jpg 100 0 > pelosi.ascii`

To view an ASCII image with color codes in the terminal (in bash
shell), use

   <code>echo -e \`cat pelosi.ascii\`</code>

To view a plain ASCII image, use cat or your favorite text editor.

ASCII to Image
--------------

To convert pelosi.ascii into a bitmap image in for example png-format
use

  `python ascii2im.py png pelosi.ascii`

This will create a png-file pelosi.ascii.png.

You can also generate an html-document like this

  `python ascii2im.py html pelosi.ascii`

The command generates an html-document with a single table
representing your ASCII image.