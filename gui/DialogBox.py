import pygame

from Button import Button
from TextLine import BG_COLOR, BORDER_COLOR
from TextLines import TextLines
from Widget import Widget

B_BUFFER = 10
V_BUFFER = 20
H_BUFFER = 20

BOX_MARGINALS = 10

class DialogBox(Widget):
    def __init__(self, pos, text, button_labels, font, minimum_width):
        self.buttons = []
        self.grabbed = 0
        self.grabbed_pos = (0, 0)

        width = 0

        for button_label in button_labels:
            self.buttons.append(Button((0, 0), button_label, font))
            width += self.buttons[-1].dim[0]
            
        if len(self.buttons) > 0:
            width += B_BUFFER * (len(self.buttons) - 1)

        button_line_width = width
        width = max(width, minimum_width)

        text_lines = TextLines((H_BUFFER, 
                                V_BUFFER),
                               text,
                               font,
                               width)
        
        button_y_pos = pos[1] + text_lines.dim[1] + 2 * V_BUFFER
#        button_x_pos = pos[0] + H_BUFFER
        button_x_pos = (width - button_line_width + H_BUFFER) / 2
        button_x_pos += pos[0]

        height = button_y_pos - pos[1]

        if len(self.buttons) != 0:
            height += self.buttons[0].dim[1]
            height += V_BUFFER

        for button in self.buttons:
            button.pos = (button_x_pos, button_y_pos)
            button_x_pos += button.dim[0] + H_BUFFER

        dim = (2 * H_BUFFER + width, height)
               
        self.surface = pygame.Surface(dim)
        self.surface.fill(BORDER_COLOR)

        light_surface = pygame.Surface((dim[0] - BOX_MARGINALS, 
                                        dim[1] - BOX_MARGINALS))
        light_surface.fill(BG_COLOR)

        self.surface.blit(light_surface, (BOX_MARGINALS / 2,
                                          BOX_MARGINALS / 2))

        text_lines.render(self.surface, 1)

        Widget.__init__(self, pos, dim, self.buttons)
        
    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            super_surface.blit(self.surface, self.pos)

            for button in self.buttons:
                button.render(super_surface, force)

    def set_focus(self, mouse_pos, click, mouse_status, events):
        if not mouse_status:
            self.grabbed = 0
            self.grab_pos = None
 
        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            button_focus = 0

            for button in self.buttons:
                button_focus = button_focus or button.set_focus(mouse_pos, 
                                                                click, 
                                                                mouse_status, 
                                                                events)
            if button_focus:
                self.grabbed = 0
                self.grab_pos = None
            elif mouse_status:
                if not self.grabbed:
                    self.grabbed = 1
                    self.grab_pos = mouse_pos

            if self.grab_pos:
                self.displace((mouse_pos[0] - self.grab_pos[0],
                               mouse_pos[1] - self.grab_pos[1]))
                self.grab_pos = mouse_pos
            
            if button_focus and click:
                self.destroy = 1

            return 1
        else:
            return 0

    def activate(self):
        Widget.activate(self)

        for button in self.buttons:
            button.activate()

    def schedule_render(self):
        Widget.schedule_render(self)

        for button in self.buttons:
            button.schedule_render()
