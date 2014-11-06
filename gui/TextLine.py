import pygame
from Widget import Widget

MARGINALS     = 4
TEXT_COLOR    = (255, 255, 255)
BG_COLOR      = (150, 150, 150)
FOCUSED_COLOR = (130, 130, 130)
BORDER_COLOR  = (100, 100, 100)

def get_item_width(text, font):
    return font.size(text)[0] + MARGINALS + 2

class Borders:
    def __init__(self, up = 0, down = 0, left = 0, right = 0):
        self.up    = up
        self.down  = down
        self.left  = left
        self.right = right

class TextLine(Widget):
    def __init__(self, pos, text, font, borders = Borders(), width = -1,
                 center = 0):
        dim = font.size(text)
        text_width = dim[0]

        if dim[0] + MARGINALS < width:
            dim = (width, dim[1])

        dim = (dim[0] + MARGINALS + 2, dim[1] + MARGINALS + 2)

        Widget.__init__(self, pos, dim)

        self.text = text
        # FIXME
        self.event = text
        self.font = font

        text_surface = self.font.render(self.text, 1, TEXT_COLOR)
        self.unfocused_surface = pygame.Surface(self.dim)
        self.unfocused_surface.fill(BG_COLOR)

        self.focused_surface = pygame.Surface(self.dim)
        self.focused_surface.fill(FOCUSED_COLOR)

        if borders.up:
            pygame.draw.line(self.unfocused_surface, BORDER_COLOR, 
                             (0,      0), 
                             (dim[0], 0), MARGINALS / 2)

            pygame.draw.line(self.focused_surface, BORDER_COLOR, 
                             (0,      0), 
                             (dim[0], 0), MARGINALS / 2)

        if borders.down:
            pygame.draw.line(self.unfocused_surface, BORDER_COLOR, 
                             (0,      dim[1] - MARGINALS / 2), 
                             (dim[0], dim[1] - MARGINALS / 2), MARGINALS / 2)

            pygame.draw.line(self.focused_surface, BORDER_COLOR, 
                             (0,      dim[1] - MARGINALS / 2), 
                             (dim[0], dim[1] - MARGINALS / 2), MARGINALS / 2)

        if borders.left:
            pygame.draw.line(self.unfocused_surface, BORDER_COLOR, 
                             (0,      0), 
                             (0, dim[1]), MARGINALS / 2)

            pygame.draw.line(self.focused_surface, BORDER_COLOR, 
                             (0,      0), 
                             (0, dim[1]), MARGINALS / 2)

        if borders.right:
            pygame.draw.line(self.unfocused_surface, BORDER_COLOR, 
                             (dim[0] - MARGINALS / 2,      0), 
                             (dim[0] - MARGINALS / 2, dim[1]), MARGINALS / 2)

            pygame.draw.line(self.focused_surface, BORDER_COLOR, 
                             (dim[0] - MARGINALS / 2,      0), 
                             (dim[0] - MARGINALS / 2, dim[1]), MARGINALS / 2)

        if center:
            x_pos = (dim[0] - text_width) / 2 
            self.unfocused_surface.blit(text_surface, 
                                        (x_pos, MARGINALS / 2 + 1))
            
            self.focused_surface.blit(text_surface, 
                                      (x_pos, MARGINALS / 2 + 1))
        else:
            self.unfocused_surface.blit(text_surface, 
                                        (MARGINALS / 2 + 1, MARGINALS / 2 + 1))

            self.focused_surface.blit(text_surface, 
                                      (MARGINALS / 2 + 1, MARGINALS / 2 + 1))

        self.surface = self.unfocused_surface

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            Widget.render(self)
            super_surface.blit(self.surface, self.pos)

    def set_focus(self, mouse_pos, click, mouse_status, events):
        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            self.surface = self.focused_surface

            if mouse_status:
                events.append(self.event)

            return 1
        else:
            self.surface = self.unfocused_surface
            return 0

    def unfocus(self):
        self.surface = self.unfocused_surface
