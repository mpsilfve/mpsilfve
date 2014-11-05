import pygame
from Widget import Widget
from TextLine import TextLine, get_item_width, Borders,\
    BORDER_COLOR, FOCUSED_COLOR, BG_COLOR

BUTTON_MARGINALS = 10
BUTTON_PADDING = 10

class Button(Widget):
    def __init__(self, pos, text, font, width = -1):
        text_line = TextLine(pos, text, font, Borders(),
                             get_item_width(text, font) + BUTTON_PADDING, 1)

        width = max(width, text_line.dim[0])

        Widget.__init__(self, pos, (width + BUTTON_MARGINALS,
                                    text_line.dim[1] + BUTTON_MARGINALS))

        x_pos = BUTTON_MARGINALS / 2 + (width - text_line.dim[0]) / 2

        self.focused_surface = pygame.Surface(self.dim)
        self.focused_surface.fill(FOCUSED_COLOR)
        self.focused_surface.blit(text_line.focused_surface,
                                  (x_pos,
                                   BUTTON_MARGINALS / 2))
        
        self.unfocused_surface = pygame.Surface(self.dim)
        self.unfocused_surface.fill(BG_COLOR)
        self.unfocused_surface.blit(text_line.unfocused_surface,
                                    (x_pos,
                                     BUTTON_MARGINALS / 2))

        pygame.draw.line(self.focused_surface,BORDER_COLOR,
                         (0, BUTTON_MARGINALS / 4), 
                         (width + BUTTON_MARGINALS, BUTTON_MARGINALS / 4),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.unfocused_surface,BORDER_COLOR,
                         (0, BUTTON_MARGINALS / 4), 
                         (width + BUTTON_MARGINALS, BUTTON_MARGINALS / 4),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.focused_surface,BORDER_COLOR,
                         (0, self.dim[1] - BUTTON_MARGINALS / 4 - 1), 
                         (width + BUTTON_MARGINALS, 
                          self.dim[1] - BUTTON_MARGINALS / 4 - 1),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.unfocused_surface,BORDER_COLOR,
                         (0, self.dim[1] - BUTTON_MARGINALS / 4 - 1), 
                         (width + BUTTON_MARGINALS, 
                          self.dim[1] - BUTTON_MARGINALS / 4 - 1),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.unfocused_surface,BORDER_COLOR,
                         (BUTTON_MARGINALS / 4 , 0), 
                         (BUTTON_MARGINALS / 4, 
                          self.dim[1]),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.focused_surface,BORDER_COLOR,
                         (BUTTON_MARGINALS / 4, 0), 
                         (BUTTON_MARGINALS / 4, 
                          self.dim[1]),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.unfocused_surface,BORDER_COLOR,
                         (self.dim[0] - BUTTON_MARGINALS / 4-1 , 0), 
                         (self.dim[0] - BUTTON_MARGINALS / 4-1, 
                          self.dim[1]),
                         BUTTON_MARGINALS / 2)

        pygame.draw.line(self.focused_surface,BORDER_COLOR,
                         (self.dim[0] - BUTTON_MARGINALS / 4 - 1, 0), 
                         (self.dim[0] - BUTTON_MARGINALS / 4 - 1, 
                          self.dim[1]),
                         BUTTON_MARGINALS / 2)

        self.surface = self.unfocused_surface
 
        # FIXME
        self.event = text

    def set_focus(self, mouse_pos, click, mouse_status, events):
        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            self.surface = self.focused_surface
            
            if click:
                events.append(self.event)
                
            return 1
        else:
            self.surface = self.unfocused_surface
            return 0

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            Widget.render(self)
            super_surface.blit(self.surface, self.pos)
