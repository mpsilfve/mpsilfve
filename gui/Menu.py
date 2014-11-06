from TextLine import TextLine, get_item_width, Borders
from Widget import Widget

class Menu(Widget):
    def __init__(self, pos, title, items, font, width = -1):
        width = max(width, get_item_width(title, font))
        for item in items:
            width = max(width, get_item_width(item, font))

        closed_title_borders = Borders(1, 1, 1, 1)
        self.title_line = TextLine(pos, title, font, 
                                   closed_title_borders, width)
        
        open_title_borders = Borders(1, 0, 1, 1)
        self.item_lines = [TextLine(pos, title, font, 
                                    open_title_borders, width)]

        item_borders = Borders(0, 0, 1, 1)
        last_item_borders = Borders(0, 1, 1, 1)

        for i in range(len(items)):
            if i + 1 == len(items):
                self.item_lines.append\
                    (TextLine((pos[0], 
                               pos[1] + (i + 1) * self.title_line.dim[1]),
                              items[i],
                              font,
                              last_item_borders,
                              width))
            else:
                self.item_lines.append\
                    (TextLine((pos[0], 
                               pos[1] + (i + 1) * self.title_line.dim[1]),
                              items[i],
                              font,
                              item_borders,
                              width))

        Widget.__init__(self, pos, (width, self.title_line.dim[1]))

        self.is_open = 1
        self.close()

    def open(self):
        if not self.is_open:
            self.schedule_render()    
            self.is_open = 1
            for item in self.item_lines:
                item.activate()
                item.schedule_render()
            self.dim = (self.dim[0], self.title_line.dim[1] * 
                        len(self.item_lines))

    def close(self):
        if self.is_open:
            self.schedule_render()
            self.is_open = 0
            self.title_line.activate()
            self.title_line.schedule_render()
            self.dim = self.title_line.dim
            for item in self.item_lines:
                item.unfocus()

    def render(self, super_surface, force = 0):
        if force or (self.need_render and self.active):
            Widget.render(self)

            if self.is_open:
                for item in self.item_lines:
                    item.render(super_surface, force)
            else:
                self.title_line.render(super_surface, force)

    def set_focus(self, mouse_pos, click, mouse_status, events):
        if Widget.set_focus(self, mouse_pos, click, mouse_status, events):
            self.open()
            
            for item in self.item_lines:
                focus = item.set_focus(mouse_pos, click, mouse_status, events)

                if mouse_status and focus:
                    self.close()

            return 1
        else:
            self.close()
            return 0
