def displace_pos(pos, displacement):
    return (pos[0] + displacement[0], pos[1] + displacement[1])

class Widget:
    def __init__(self, pos, dim, sub_widgets = []):
        self.pos         = pos
        self.dim         = dim
        self.active      = 0
        self.need_render = 0
        self.sub_widgets = sub_widgets
        self.destroy     = 0

    def set_pos(self, pos):
        self.pos = pos

    def activate(self):
        self.active      = 1
        self.need_render = 1

    def deactivate(self):
        self.active = 0

    def set_focus(self, mouse_pos, click, mouse_status, events):
        if not self.active:
            return 0
        
        return (self.pos[0]       < mouse_pos[0]         and 
                mouse_pos[0] < self.pos[0] + self.dim[0] and
                self.pos[1]       < mouse_pos[1]         and 
                mouse_pos[1] < self.pos[1] + self.dim[1])

    def schedule_render(self):
        self.need_render = 1

    def render(self):
        self.need_render = 0

    def displace(self, displacement):
        self.pos = displace_pos(self.pos, displacement)

        for widget in self.sub_widgets:
            widget.displace(displacement)
