#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

// Constantes do sistema
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Portas de hardware
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Códigos de teclado importantes
#define BACKSPACE_SCANCODE 0x0E
#define ESCAPE_SCANCODE 0x01

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

// Estrutura do terminal
typedef struct Terminal {
  uint16_t *buffer;
  uint16_t cursor_x;
  uint16_t cursor_y;
  vga_color fg_color;
  vga_color bg_color;
  uint8_t read_key_scancode();
  const int ENTER_SCANCODE = 0x1c;
  void reboot_system();
} Terminal;

// Interface de funções do terminal
void terminal_initialize(Terminal *term);
void terminal_clear(Terminal *term);
void terminal_setcolor(Terminal *term, vga_color fg, vga_color bg);
void terminal_putchar(Terminal *term, char c);
void terminal_write(Terminal *term, const char *data, size_t size);
void terminal_writestring(Terminal *term, const char *data);
void terminal_printf(Terminal *term, const char *format, ...);
void show_splash_screen(Terminal *term);

// Funções de manipulação de hardware
uint8_t keyboard_read_scancode(void);
char keyboard_read_char(void);
void reboot_system(void);
inline uint8_t inb(uint16_t port);
inline void outb(uint16_t port, uint8_t value);

// Função principal do kernel
void kernel_main(void);

#endif // KERNEL_H
