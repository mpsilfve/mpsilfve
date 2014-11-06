import pygame
from RadioButton import RadioButton, get_radio_button_width
from Widget import Widget

class RadioButtonList(Widget):
    def __init__(self, pos, options, font, width = -1):
        if len(options) == 0:
            return None

        for option in options:
            width = max(width, get_radio_button_width(option, font))

        self.buttons = []

        pos_y = pos[1]

        for i in range(len(options)):
            self.buttons.append(RadioButton((pos[0], pos_y),
                                            options[i],
                                            font,
                                            width))

            pos_y += self.buttons[-1].dim[1] + 5

        Widget.__init__(self, pos, 
                        (width, len(self.buttons) * 
                         (5 + self.buttons[0].dim[1])))

    def activate(self):
        Widget.activate(self)
        for button in self.buttons:
            button.activate()

    def schedule_render(self):
        Widget.schedule_render(self)
        for button in self.buttons:
            button.schedule_render()

    def set_focus(self, mouse_pos, click, mouse_status, events):             
        focus = 0
        
        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            focus = 1

        for button in self.buttons:
            
            f = button.set_focus(mouse_pos, 
                                 click, 
                                 mouse_status, 
                                 events,
                                 focus)
            
            if not focus:
                assert(not f)

        return focus

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            Widget.render(self)

            for button in self.buttons:
                    button.render(super_surface, force)

