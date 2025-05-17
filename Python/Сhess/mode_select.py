import pygame
import sys
import tkinter as tk
from tkinter import simpledialog

def get_ip_from_window():
    root = tk.Tk()
    root.withdraw()
    ip = simpledialog.askstring("IP Address", "Enter host IP:")
    root.destroy()
    return ip

def select_mode(win):
    width, height = win.get_size()
    title_font = pygame.font.SysFont("arial", 32)
    btn_font = pygame.font.SysFont("arial", 24)

    btn_width, btn_height = 340, 50
    x = (width - btn_width) // 2
    spacing = 80
    option1 = pygame.Rect(x, 200, btn_width, btn_height)
    option2 = pygame.Rect(x, 200 + spacing, btn_width, btn_height)
    option3 = pygame.Rect(x, 200 + spacing * 2, btn_width, btn_height)

    while True:
        win.fill((230, 230, 255))
        title_surf = title_font.render("Choose mode:", True, (50, 50, 50))
        title_rect = title_surf.get_rect(center=(width // 2, 140))
        win.blit(title_surf, title_rect)

        for rect, label in [(option1, "Play 1-on-1"), (option2, "Play vs PC"), (option3, "Play over Network")]:
            pygame.draw.rect(win, (200, 200, 255), rect, border_radius=8)
            pygame.draw.rect(win, (100, 100, 200), rect, 2, border_radius=8)
            txt = btn_font.render(label, True, (0, 0, 0))
            win.blit(txt, txt.get_rect(center=rect.center))

        pygame.display.update()
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif e.type == pygame.MOUSEBUTTONDOWN:
                if option1.collidepoint(e.pos):
                    return ("local", None)
                if option2.collidepoint(e.pos):
                    return ("bot", None)
                if option3.collidepoint(e.pos):
                    return select_network_type(win)

def select_network_type(win):
    width, height = win.get_size()
    title_font = pygame.font.SysFont("arial", 32)
    btn_font = pygame.font.SysFont("arial", 24)

    btn_width, btn_height = 340, 50
    x = (width - btn_width) // 2
    host_btn = pygame.Rect(x, 240, btn_width, btn_height)
    join_btn = pygame.Rect(x, 240 + 80, btn_width, btn_height)

    while True:
        win.fill((230, 230, 255))
        title_surf = title_font.render("Network:", True, (50, 50, 50))
        title_rect = title_surf.get_rect(center=(width // 2, 180))
        win.blit(title_surf, title_rect)

        for rect, label in [(host_btn, "Host Game"), (join_btn, "Join Game")]:
            pygame.draw.rect(win, (200, 200, 255), rect, border_radius=8)
            pygame.draw.rect(win, (100, 100, 200), rect, 2, border_radius=8)
            txt = btn_font.render(label, True, (0, 0, 0))
            win.blit(txt, txt.get_rect(center=rect.center))

        pygame.display.update()
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif e.type == pygame.MOUSEBUTTONDOWN:
                if host_btn.collidepoint(e.pos):
                    return ("net_host", None)
                if join_btn.collidepoint(e.pos):
                    host = get_ip_from_window()
                    if host and host.strip(): 
                        return ("net_client", host)