import pygame
from Widget import Widget
from TextLine import TextLine, get_item_width, Borders

class TextLines(Widget):
    def __init__(self, pos, text, font, width, center = 0):
        self.lines = []

        words = text.split(' ')
        start = 0
        stop  = 0

        height = font.get_height()

        while start < len(words):
            while (get_item_width(' '.join(words[start:stop]), font) < 
                   width and stop < len(words) + 1):
                stop += 1

            l = ' '.join(words[start:stop - 1])

            self.lines.append\
                (TextLine((pos[0], pos[1] + len(self.lines) * (height + 1)), 
                          l, 
                          font, 
                          Borders(), 
                          width,
                          center))

            start = stop - 1

        Widget.__init__(self, pos, (width, len(self.lines) * (height + 1)))

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            for line in self.lines:
                line.render(super_surface, force)
