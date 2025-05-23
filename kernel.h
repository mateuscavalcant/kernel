#ifndef KERNEL_H
#define KERNEL_H
#include <__stddef_size_t.h>
// Constantes do sistema
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Cores disponíveis no modo VGA
typedef enum {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_YELLOW = 14,
  VGA_COLOR_WHITE = 15
} vga_color;

// Estrutura para representar o terminal
typedef struct {
  unsigned short *buffer;
  int cursor_x;
  int cursor_y;
  vga_color fg_color;
  vga_color bg_color;
} Terminal;

// Protótipos de funções
unsigned char vga_entry_color(vga_color fg, vga_color bg);
unsigned short vga_entry(unsigned char uc, unsigned char color);
void terminal_initialize(Terminal *term);
void terminal_setcolor(Terminal *term, vga_color fg, vga_color bg);
void terminal_putentry(Terminal *term, char c);
void terminal_putchar(Terminal *term, char c);
void terminal_write(Terminal *term, const char *data, size_t size);
void terminal_writestring(Terminal *term, const char *data);
void kernel_main(void);

#endif
