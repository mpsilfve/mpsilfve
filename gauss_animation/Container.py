from random import gauss, seed, random, randint

seed()

class Color:
    def __init__(self, c = None):
        if c == None:
            self.r = 0
            self.g = 0
            self.b = 0
        else:
            self.r = c.r
            self.g = c.b
            self.b = c.g
        
    def incr(self, r_incr, g_incr, b_incr):
        r_incr = max(min(255, r_incr), -255)
        g_incr = max(min(255, g_incr), -255)
        b_incr = max(min(255, b_incr), -255)

        self.r += r_incr 
        self.g += g_incr 
        self.b += b_incr 

        if self.r < 0:
            self.r *= -1

        if self.g < 0:
            self.g *= -1

        if self.b < 0:
            self.b *= -1

        if self.r > 255:
            self.r = 2*255 - self.r

        if self.g > 255:
            self.g = 2*255 - self.g

        if self.b > 255:
            self.b = 2*255 - self.b

    def set(self, r, g, b):
        self.r = r 
        self.g = g
        self.b = b

        if self.r < 0:
            self.r *= -1

        if self.g < 0:
            self.g *= -1

        if self.b < 0:
            self.b *= -1

        if self.r > 255:
            self.r = 2*255 - self.r

        if self.g > 255:
            self.g = 2*255 - self.g

        if self.b > 255:
            self.b = 2*255 - self.b

    def get_color(self):
        return (int(self.r), int(self.g), int(self.b))


    def get_steam_color(self, bg_c):
        magnitude = self.r/255.0
        
        if magnitude > 0.4:
            return (int(self.r), 
                    int(self.r), 
                    int(self.r))
        else:
            return bg_c

class Container:
    def __init__(self, height, width, std_dev):
        self.array = [ [Color() for i in xrange(height)] for j in xrange(width) ]
        self.width = width
        self.height = height
        self.std_dev = std_dev

    def get_color(self, x, y):
        if x < 0:
            x *= -1
        if x >= self.width:
            x = 2 * self.width - x - 1

        if y < 0:
            y *= -1
        if y >= self.height:
            y = 2 * self.height - y - 1

        return self.array[x][y]

    def iter(self):
#        new_array = [ [Color() for i in xrange(self.height)] for j in xrange(self.width) ]

        for x in xrange(self.width):
            for y in xrange(self.height):
               self.array[x][y].incr(gauss(0, self.std_dev),
                                     gauss(0, self.std_dev),
                                     gauss(0, self.std_dev))

        for x in xrange(self.width):
            for y in xrange(self.height):
                c = [0, 0, 0]

                x_t = -2

                if random() > 0.5:
                    x_t = 0

                y_t = -2
                
                if random() > 0.5:
                    y_t = 0

                x_t = -10
                y_t = -10
#                if x > self.width / 2 and y > self.height / 2:
#                    x_t = -2


#                if y > self.height / 2:
#                    y_t = -2

                for i in xrange(x_t, x_t + 3):
                    for j in xrange(y_t, y_t + 3):
                        cc = self.get_color(x + i, y + j)
                        c[0] += cc.r
                        c[1] += cc.g
                        c[2] += cc.b
#                        c[0] *= (cc.r)#**(1.0/9)
#                        c[1] *= (cc.g)#**(1.0/9) 
#                        c[2] *= (cc.b)#**(1.0/9) 

#                self.array[x][y] = Color()
#                self.array[x][y].set(c[0]**(1.0/9)+.9, c[1]**(1.0/9)+.9, c[2]**(1.0/9)+.9)
                self.array[x][y].set(c[0]/9.0, c[1]/9.0, c[2]/9.0)

#                self.array[x][y].incr(gauss(0, self.std_dev),
#                                     gauss(0, self.std_dev),
#                                     gauss(0, self.std_dev))

#        self.array = new_array

    def blit(self, surface, coord, step):
        for x in xrange(self.width):
            for y in xrange(self.height):
#                surface.set_at((coord[1] + y*step, coord[0] + x*step), 
#                               self.get_color(x, y).get_color())
                pygame.draw.rect(surface, 
#                                 self.get_color(x, y).get_steam_color((150,150,250)),
                                 self.get_color(x, y).get_color(),
                                 ((coord[1] + y*step, 
                                   coord[0] + x*step),
                                  (coord[1] + (y+1)*step, 
                                   coord[0] + (x+1)*step)))

    def randomize(self):
        for x in xrange(self.width):
            for y in xrange(self.height):
                c = self.get_color(x, y)
                c.r = randint(0, 155)
                c.g = randint(0, 155)
                c.b = randint(0, 155)

import pygame

def blurSurf(surface, amt):
    """
    Blur the given surface by the given 'amount'.  Only values 1 and greater
    are valid.  Value 1 = no blur.
    """
    if amt < 1.0:
        raise ValueError("Arg 'amt' must be greater than 1.0, passed in value is %s"%amt)
    scale = 1.0/float(amt)
    surf_size = surface.get_size()
    scale_size = (int(surf_size[0]*scale), int(surf_size[1]*scale))
    surf = pygame.transform.smoothscale(surface, scale_size)
    surf = pygame.transform.smoothscale(surf, surf_size)
    return surf

def weight(x, y, z, a, b):
    x *= 1.0
    y *= 1.0
    z *= 1.0
    return ((y - z)/(a - b) - (z - x*b/a)/(b - b**2/a))/((b - a)/(a - b) - (a - b**2/a)/(b - b**2/a))

def reweight(c, a, b):
    return [ weight(c[1],c[2],c[0],a,b), 
             weight(c[0],c[2],c[1],a,b), 
             weight(c[1],c[0],c[2],a,b) ]

def saturate(surf):
    surf_size = surface.get_size()

    for i in xrange(surf_size[0]):
        for j in xrange(surf_size[1]):
            c = list(surf.get_at((i,j)))
            c = reweight(c,0.4,0.3)
#            if c[0] == min(c):
#                c[1] += 0.05*c[0]
#                c[2] += 0.05*c[0]
#                c[0] *= 0.9
#            elif c[1] == min(c):
#                c[0] += 0.05*c[1]
#                c[2] += 0.05*c[1]
#                c[1] *= 0.9
#            else:
#                c[0] += 0.05*c[2]
#                c[1] += 0.05*c[2]
#                c[2] *= 0.9

            c[0] = max(0, min(255, int(c[0])))
            c[1] = max(0, min(255, int(c[1])))
            c[2] = max(0, min(255, int(c[2])))

            surf.set_at((i,j), c)

def th(surf):
    surf_size = surface.get_size()

    for i in xrange(surf_size[0]):
        for j in xrange(surf_size[1]):
            c = surf.get_at((i,j))
            magnitude = (c[0] + c[1] + c[2])/(3.0*255)

            if magnitude < 0.3:
                surf.set_at((i,j), (0,0,50))

if __name__=='__main__':
    pygame.init()
    pygame.display.init()
    pygame.display.set_mode((4*160, 4*90))

    surface = pygame.display.get_surface()

    container = Container(2*160,2*90,20)
    container.randomize()
#    container = Container(3*16,3*9,10)

    for i in xrange(1):
        print i
        container.iter()
        
        if i % 50 == 0:
            container.blit(surface, (0,0), 2)
#            th(surface)
            s = blurSurf(surface, 3)            
            surface.blit(s, (0,0))
            saturate(surface)

            pygame.display.update()

    i = 0

    while 1:        
        container.iter()

        container.blit(surface, (0,0), 2)
#            th(surface)
        s = blurSurf(surface, 3)            
        surface.blit(s, (0,0))
        saturate(surface)
        
        pygame.display.update()
#        pygame.image.save(surface, "animation/" + str(i) + ".png")
        i += 1
        
