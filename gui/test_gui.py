import pygame
from TextLine import TextLine, Borders
from Menu import Menu
from Button import Button
from TextLines import TextLines
from DialogBox import DialogBox
from RadioButtonList import RadioButtonList

pygame.display.init()
pygame.font.init()

font = pygame.font.SysFont("freemono", 15)

screen = pygame.display.set_mode((600, 400),
                                 pygame.RESIZABLE)

menu1 = Menu((0, 0), "Menu", 
             ["New game", "Load", "Save", "Quit"], font, 
             100)

menu2 = Menu((menu1.dim[0]+1, 0), "Menu", 
             ["New game", "Load", "Save", "Help", "Quit"], font, 
             100)

button1 = Button((100, 100), "OK", font, 70)
button2 = Button((200, 100), "Quit", font, 70)

dialog_box = DialogBox((100, 150), "Useless text that could be in a " +
                       "dialog box.",["OK", "Cancel", "Quit"], font, 300)

text_lines = TextLines((300, 300), 
                       "This is a pretty long line that won't fit in 200 px. "+
                       "Hence, it's going to be split.",
                       font, 
                       200,
                       0)

radio_buttons = RadioButtonList((450, 50), ["English",
                                            "French",
                                            "Spanish"],
                                font)

menu1.activate()
menu2.activate()
button1.activate()
button2.activate()
text_lines.activate()
dialog_box.activate()
radio_buttons.activate()

elements = [dialog_box, menu1, menu2, button1, button2, text_lines, 
            radio_buttons]

released = 1
events = []

while 1:
    old_elements = elements
    elements = []

    for element in old_elements:
        if not element.destroy:
            elements.append(element)

    pygame.event.pump()
    pos   = pygame.mouse.get_pos()
    mouse = pygame.mouse.get_pressed()[0]
    click = mouse
    if not click:
        released = 1

    if not released:
        click = 0

    if click:
        released = 0

    for element in elements:        
        if element.set_focus(pos, click, mouse, events):
            break

    screen.fill((50, 50, 50))

    for element in elements[::-1]:
        element.render(screen, 1)

    pygame.display.update()
    
    if "Quit" in events:
        break

    pygame.time.wait(50)

print events
