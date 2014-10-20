from sys import argv, stderr, stdout
import PIL.Image as Im

def normalize_color(c):
    if c == (0, 0, 0):
        return c

    c_norm = (c[0]**2 + c[1]**2 + c[2]**2)**.5
    return tuple((x / c_norm for x in c))

light_colors = \
    [
    ("\e[107m", normalize_color((255, 255, 255))), 
    ("\e[101m", normalize_color((255, 150, 150))), 
    ("\e[102m", normalize_color((150, 255, 150))), 
    ("\e[103m", normalize_color((255, 255, 150))), 
    ("\e[104m", normalize_color((150, 150, 255))),
    ("\e[105m", normalize_color((255, 150, 255)))
    ]

dark_colors = \
    [
    ("\e[97m", normalize_color((255, 255, 255))), 
    ("\e[91m", normalize_color((255, 150, 150))), 
    ("\e[92m", normalize_color((150, 255, 150))), 
    ("\e[93m", normalize_color((255, 255, 150))), 
    ("\e[94m", normalize_color((150, 150, 255))),
    ("\e[95m", normalize_color((255, 150, 255)))
    ]

def diff(c1, c2):
    return ((c1[0] - c2[0])**2 + (c1[1] - c2[1])**2 + (c1[2] - c2[2])**2)**.5

def get_color(p, avg, TH = 256):
    c = normalize_color(p)

    min_diff = 1000
    min_i    = ""

    for i in range(len(dark_colors)):
        color = dark_colors[i][1]
        d = diff(c, color)
        if d < min_diff:
            min_diff = d
            min_i = i
            
    return dark_colors[min_i][0] if avg < TH else light_colors[min_i][0]

def get_area_average(image, i, j, x_step, y_step):
    sum_p = [0.0, 0.0, 0.0]
    count = 0
    istart = int(i*x_step)
    jstart = int(j*y_step)
    istop  = int((i+1)*x_step)
    jstop  = int((j+1)*y_step)

    for k in range(istart, min(istop, image.size[0])):
        for l in range(jstart, min(jstop, image.size[1])):
            p = image.getpixel((k, l))
            count += 1
            if type(p) == type(0):
                p = (p, p, p)
            sum_p[0] += p[0]
            sum_p[1] += p[1]
            sum_p[2] += p[2]
    
    if count == 0:
        return (0, 0, 0)

    return (int(sum_p[0]/count), int(sum_p[1]/count), int(sum_p[2]/count)) 

def get_component_average(p):
    sum_p = p[0] + p[1] + p[2]
    return sum_p / 3.0


if len(argv) < 3 or len(argv) > 5:
    stderr.write("USAGE: %s image_file WIDTH [0 | 1] [HEIGHT]" % argv[0])
    exit(1)

image = Im.open(argv[1])
WIDTH = int(argv[2])

show_color = 0
if len(argv) >= 4:
    show_color = int(argv[3])

x_step = image.size[0]*1.0/WIDTH 
y_step = 2.2*x_step 
height = int(image.size[1]/y_step)

if len(argv) == 5:
    height = int(argv[4])
    y_step = image.size[1]*1.0/height
 
CHAR_THS = \
    [
    ('`', 40),
    ('.', 55),
    (':', 70),
    ('*', 85),
    ('[', 100),
    ('w', 115),
    ('2', 130),
    ('E', 145),
    ('$', 160),
    ('@', 190),
    ('0', 220),
    ('Q', 250)
    ]

if show_color:
    CHAR_THS = \
        [
        ('`', 10), ('.', 15), ('-', 20), ('_', 25), ('\'', 30), (':', 35),
        (',', 40), (';', 44), ('^', 45), ('=', 47), ('+', 49), ('/', 51),
        ('\"', 52), ('|', 54), ('<', 56), (')', 58), ('i', 59), ('%', 61),
        ('c', 63), ('l', 65), ('s', 66), ('{', 68), ('*', 70), ('I', 72),
        ('?', 73), ('!', 75), ('[', 77), ('t', 79), ('a', 80), ('o', 82),
        ('z', 84), ('L', 86), ('u', 87), ('T', 89), ('J', 91), ('w', 93),
        ('f', 94), ('y', 96), ('3', 98), ('2', 100), ('F', 101), ('m', 103),
        ('q', 105), ('V', 107), ('E', 108), ('G', 110), ('Y', 112), ('k', 114),
        ('A', 115), ('U', 117), ('$', 119), ('@', 121), ('H', 122), ('N', 124),
        ('0', 126), ('Q', 128), ('`', 159), ('.', 161), ('-', 163), ('_', 165),
        ('\'', 166), (':', 168), (',', 170), (';', 172), ('^', 173), ('=', 175),
        ('+', 177), ('/', 179), ('\"', 180), ('|', 182), ('<', 184), (')', 186),
        ('i', 187), ('%', 189), ('c', 191), ('l', 193), ('s', 194), ('{', 196),
        ('*', 198), ('I', 200), ('?', 201), ('!', 203), ('[', 205), ('t', 207),
        ('a', 208), ('o', 210), ('z', 212), ('L', 214), ('u', 215), ('T', 217),
        ('J', 219), ('w', 221), ('f', 222), ('y', 224), ('3', 226), ('2', 228),
        ('F', 229), ('m', 231), ('q', 233), ('V', 235), ('E', 236), ('G', 238),
        ('Y', 240), ('k', 242), ('A', 243), ('U', 245), ('$', 247), ('@', 249),
        ('H', 250), ('N', 252), ('0', 254), ('Q', 256)
        ]

for j in range(0, height):
    for i in range(0, WIDTH):
        avg_pixel = get_area_average(image, i, j, x_step, y_step)
        avg = get_component_average(avg_pixel)

        color_id = (get_color(avg_pixel, avg, 128) if show_color else "")
        basic_id = ("\e[0m" if show_color else "")

        if not show_color:
            avg /= 2

        for char, th in CHAR_THS:
            if th >= 256 or avg < th:
                stdout.write(color_id+char+basic_id)
                break

    stdout.write('\\n' if show_color else '\n')
