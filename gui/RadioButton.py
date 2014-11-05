import pygame

from TextLine import TextLine, Borders, BG_COLOR, FOCUSED_COLOR, BORDER_COLOR,\
    get_item_width

from Widget import Widget 

PAD = 5
SELECT_COLOR = (200, 200, 200)

def get_radio_button_width(text, font):
    return font.get_height() + get_item_width(text, font) + 2 * PAD

class RadioButton(Widget):
    def __init__(self, pos, text, font, width = -1):
        font_height = font.get_height()

        text_line = TextLine((font_height + PAD, 0), 
                             text, 
                             font,
                             Borders(),
                             width - font_height - 2 * PAD,
                             0)

        text_line.activate()

        self.passive_surface = \
            pygame.Surface((font_height + text_line.dim[0] + 2 * PAD, 
                            font_height + PAD))
        self.passive_surface.fill(BG_COLOR)

        self.active_surface = \
            pygame.Surface((font_height + text_line.dim[0] + 2 * PAD, 
                            font_height + PAD))
        self.active_surface.fill(FOCUSED_COLOR)

        text_line.render(self.passive_surface, 1)
        text_line.set_focus((font_height + PAD + 1, 1), 0, 0, [])
        text_line.render(self.active_surface, 1)
     
        pygame.draw.circle(self.active_surface, 
                           BORDER_COLOR, 
                           (font_height / 2 + PAD/2, 
                            font_height / 2 + PAD/2),
                           font_height / 2)

        pygame.draw.circle(self.passive_surface, 
                           BORDER_COLOR, 
                           (font_height / 2 + PAD/2, 
                            font_height / 2 + PAD/2),
                           font_height / 2)

        self.font_height = font_height
        self.selected = 0

        Widget.__init__(self, pos, (font_height + text_line.dim[0], 
                                    font_height))

        self.surface = self.active_surface
        self.event = "Select: " + text

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            Widget.render(self)
            super_surface.blit(self.surface, self.pos)

            if self.selected:
                pygame.draw.circle(super_surface, 
                                   SELECT_COLOR,
                                   (self.pos[0] + self.font_height/2 + PAD/2, 
                                    self.pos[1] + self.font_height/2 + PAD/2),
                                   self.font_height / 4)

    def set_focus(self, mouse_pos, click, mouse_status, events, focused_list):
        if focused_list and click:
            self.selected = 0

        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            self.surface = self.active_surface

            if click:
                events.append(self.event)
                self.selected = 1

            return 1
        else:
            self.surface = self.passive_surface
            return 0
