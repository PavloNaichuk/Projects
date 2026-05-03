import pygame
import sys
import tkinter as tk
from tkinter import simpledialog

BUTTON_BG        = (200, 200, 200)
BUTTON_TEXT      = (  0,   0,   0)
HIGHLIGHT_COLOR = (130, 100, 160)
HOVER_COLOR      = (190, 160, 200)
class Button:
    def __init__(self, rect, text, font, action):
        self.rect   = pygame.Rect(rect)
        self.text   = text
        self.font   = font
        self.action = action
        self.hover  = False
        self.active = False

    def handle_event(self, ev):
        if ev.type == pygame.MOUSEMOTION:
            self.hover = self.rect.collidepoint(ev.pos)
        elif ev.type == pygame.MOUSEBUTTONDOWN and ev.button == 1:
            if self.rect.collidepoint(ev.pos):
                self.action()
                self.active = True
        elif ev.type == pygame.MOUSEBUTTONUP and ev.button == 1:
            self.active = False

    def draw(self, surf):
        bg = HIGHLIGHT_COLOR if self.active else BUTTON_BG
        pygame.draw.rect(surf, bg, self.rect, border_radius=8)
        border_col   = HIGHLIGHT_COLOR if (self.hover or self.active) else BUTTON_TEXT
        border_width = 4 if (self.hover or self.active) else 2
        pygame.draw.rect(surf, border_col, self.rect, border_width, border_radius=8)
        txt = self.font.render(self.text, True, BUTTON_TEXT)
        surf.blit(txt, txt.get_rect(center=self.rect.center))
        
def get_ip_from_window():
    root = tk.Tk()
    root.withdraw()
    ip = simpledialog.askstring("IP Address", "Enter host IP:")
    root.destroy()
    return ip

def select_mode(win):
    width, height = win.get_size()
    title_font = pygame.font.SysFont("arial", 32)
    btn_font   = pygame.font.SysFont("arial", 24)
    result = {}

 
    def choose_local():   result.update(mode='local', net=None)
    def choose_bot():     result.update(mode='bot',   net=None)
    def choose_network(): result.update(mode='net',   net='ask')

    btn_w, btn_h = 340, 50
    x = (width - btn_w) // 2
    spacing = 80
    labels = [("Play 1-on-1", choose_local),
              ("Play vs PC",   choose_bot),
              ("Play over Network", choose_network)]
    buttons = [
        Button((x, 200 + i*spacing, btn_w, btn_h), txt, btn_font, cb)
        for i, (txt, cb) in enumerate(labels)
    ]

    clock = pygame.time.Clock()
    while True:
        win.fill((240,240,240))
        title = title_font.render("Choose mode:", True, (50,50,50))
        win.blit(title, title.get_rect(center=(width//2,140)))

        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                pygame.quit(); sys.exit()
            for btn in buttons:
                btn.handle_event(ev)

        for btn in buttons:
            btn.draw(win)

        pygame.display.update()
        clock.tick(60)

        if 'mode' in result:
            if result['net'] == 'ask':
                return ('net_client', get_ip_from_window())
            tc = select_time_control(win)
            return (result['mode'], result['net'], tc)


def select_network_type(win):
    width, height = win.get_size()
    title_font = pygame.font.SysFont("arial", 32)
    btn_font   = pygame.font.SysFont("arial", 24)
    result = {}

    def host():  result['type'] = ('net_host', None)
    def join():  result['type'] = ('net_client', get_ip_from_window())

    btn_w, btn_h = 340, 50
    x = (width - btn_w) // 2
    spacing = 80
    labels = [("Host Game", host),
              ("Join Game", join)]
    buttons = [
        Button((x, 240 + i*spacing, btn_w, btn_h), txt, btn_font, cb)
        for i, (txt, cb) in enumerate(labels)
    ]

    clock = pygame.time.Clock()
    while True:
        win.fill((240,240,240))
        title = title_font.render("Network:", True, (50,50,50))
        win.blit(title, title.get_rect(center=(width//2,180)))

        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                pygame.quit(); sys.exit()
            for btn in buttons:
                btn.handle_event(ev)

        for btn in buttons:
            btn.draw(win)

        pygame.display.update()
        clock.tick(60)

        if 'type' in result:
            return result['type']

def select_time_control(win):
    width, height = win.get_size()
    title_font = pygame.font.SysFont("arial", 32)
    btn_font   = pygame.font.SysFont("arial", 24)
    result = {}

    options = [
        ("3",    (3*60, 2)),
        ("5",    (5*60, 0)),
        ("10",   (10*60,5)),
        ("None", (0,     0)),
    ]
    btn_w, btn_h = 300, 50
    x = (width - btn_w) // 2
    spacing = 80

    buttons = [
        Button((x, 200 + i*spacing, btn_w, btn_h), label, btn_font,
               lambda tc=tc: result.update(tc=tc))
        for i, (label, tc) in enumerate(options)
    ]

    clock = pygame.time.Clock()
    while True:
        win.fill((240,240,240))
        title = title_font.render("Time Control:", True, (0,0,0))
        win.blit(title, title.get_rect(center=(width//2,120)))

        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                pygame.quit(); sys.exit()
            for btn in buttons:
                btn.handle_event(ev)

        for btn in buttons:
            btn.draw(win)

        pygame.display.update()
        clock.tick(60)

        if 'tc' in result:
            return result['tc']
