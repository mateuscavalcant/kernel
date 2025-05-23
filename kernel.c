#include "kernel.h"
#include <stddef.h>
#include <stdint.h>

// Funções básicas de VGA
unsigned char vga_entry_color(vga_color fg, vga_color bg) {
  return fg | bg << 4;
}

unsigned short vga_entry(unsigned char uc, unsigned char color) {
  return (unsigned short)uc | (unsigned short)color << 8;
}

// Funções do terminal
void terminal_initialize(Terminal *term) {
  term->buffer = (unsigned short *)VGA_ADDRESS;
  term->cursor_x = 0;
  term->cursor_y = 0;
  term->fg_color = VGA_COLOR_WHITE;
  term->bg_color = VGA_COLOR_BLACK;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      term->buffer[index] =
          vga_entry(' ', vga_entry_color(term->fg_color, term->bg_color));
    }
  }
}

void terminal_setcolor(Terminal *term, vga_color fg, vga_color bg) {
  term->fg_color = fg;
  term->bg_color = bg;
}

void terminal_putentry(Terminal *term, char c) {
  const size_t index = term->cursor_y * VGA_WIDTH + term->cursor_x;
  term->buffer[index] =
      vga_entry(c, vga_entry_color(term->fg_color, term->bg_color));
}

void terminal_putchar(Terminal *term, char c) {
  if (c == '\n') {
    term->cursor_x = 0;
    term->cursor_y++;
  } else if (c == '\t') {
    term->cursor_x = (term->cursor_x + 8) & ~(8 - 1);
  } else {
    terminal_putentry(term, c);
    term->cursor_x++;
  }

  // Quebra de linha automática
  if (term->cursor_x >= VGA_WIDTH) {
    term->cursor_x = 0;
    term->cursor_y++;
  }

  // Scroll da tela
  if (term->cursor_y >= VGA_HEIGHT) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
      for (size_t x = 0; x < VGA_WIDTH; x++) {
        term->buffer[(y - 1) * VGA_WIDTH + x] = term->buffer[y * VGA_WIDTH + x];
      }
    }

    // Limpa última linha
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      term->buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
          vga_entry(' ', vga_entry_color(term->fg_color, term->bg_color));
    }

    term->cursor_y = VGA_HEIGHT - 1;
  }
}

// Função para exibir a tela de apresentação
void show_splash_screen(Terminal *term) {
  terminal_setcolor(term, VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  terminal_writestring(term,
                       "  +-----------------------------------------------+\n");
  terminal_writestring(term,
                       "  |           ::KERNEL WAIO::                |\n");
  terminal_writestring(term,
                       "  +-----------------------------------------------+\n");
  terminal_setcolor(term, VGA_COLOR_CYAN, VGA_COLOR_BLACK);
  terminal_writestring(term, "  |         BEM-VINDO          |\n");
  terminal_writestring(term,
                       "  +-----------------------------------------------+\n");
  terminal_setcolor(term, VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
  terminal_writestring(term,
                       "  |      PRESSIONE [ENTER] PARA REINICIAR         |\n");
  terminal_writestring(
      term, "  +-----------------------------------------------+\n\n");

  // Centraliza o cursor abaixo da tela de apresentação
  term->cursor_x = 0;
  term->cursor_y = 8;
}

void terminal_write(Terminal *term, const char *data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    terminal_putchar(term, data[i]);
  }
}

void terminal_writestring(Terminal *term, const char *data) {
  while (*data != '\0') {
    terminal_putchar(term, *data++);
  }
}

uint8_t read_key_scancode() {
  uint8_t scancode = 0;
  while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
    ;
  scancode = inb(KEYBOARD_DATA_PORT);
  return scancode;
}

void reboot_system() { outb(KEYBOARD_STATUS_PORT, 0xFE); }

inline uint8_t inb(uint16_t port) {
  uint8_t result;
  asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}
inline void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}
