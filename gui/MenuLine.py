from Widget import Widget
from Menu import Menu

class MenuLine(Widget):
    def __init__(self, pos, width, menu_texts, font, menu_width = -1):
        Widget.__init__(self, pos, (width, font.get_height()))

        self.menus = []

        pos_x = pos[0]

        for title, items in menu_texts:
            self.menus.append(Menu(pos, title, items, font, menu_width))
            pos_x += self.menus[-1].dim[0] + 1
