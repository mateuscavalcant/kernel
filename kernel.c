#include "kernel.h"
#include <stddef.h> // Para size_t

/* Funções básicas de VGA */
unsigned char vga_entry_color(vga_color fg, vga_color bg) {
  return fg | bg << 4;
}

unsigned short vga_entry(unsigned char uc, unsigned char color) {
  return (unsigned short)uc | (unsigned short)color << 8;
}

/* Funções do terminal */
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

  /* Quebra de linha automática */
  if (term->cursor_x >= VGA_WIDTH) {
    term->cursor_x = 0;
    term->cursor_y++;
  }

  /* Scroll da tela */
  if (term->cursor_y >= VGA_HEIGHT) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
      for (size_t x = 0; x < VGA_WIDTH; x++) {
        term->buffer[(y - 1) * VGA_WIDTH + x] = term->buffer[y * VGA_WIDTH + x];
      }
    }

    /* Limpa última linha */
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      term->buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
          vga_entry(' ', vga_entry_color(term->fg_color, term->bg_color));
    }

    term->cursor_y = VGA_HEIGHT - 1;
  }
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

/* Função principal do kernel */
void kernel_main(void) {
  Terminal term;
  terminal_initialize(&term);

  /* Exemplo de uso básico */
  terminal_writestring(&term, "Kernel em C com header!\n");

  /* Mudando cores */
  terminal_setcolor(&term, VGA_COLOR_YELLOW, VGA_COLOR_BLUE);
  terminal_writestring(&term, "Texto colorido!\n");

  /* Voltando às cores padrão */
  terminal_setcolor(&term, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  terminal_writestring(&term, "Mais mensagens do kernel...\n");

  /* Demonstração de cores */
  for (int bg = 0; bg < 16; bg++) {
    for (int fg = 0; fg < 16; fg++) {
      terminal_setcolor(&term, fg, bg);
      terminal_putchar(&term, 'X');
    }
    terminal_putchar(&term, '\n');
  }
}
