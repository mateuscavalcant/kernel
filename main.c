#include "kernel.h"
#include <sys/types.h>

void kernel_main(void);

void kernel_main(void) {
  Terminal term;
  terminal_initialize(&term);

  terminal_writestring(&term, "Sistema Kernel Inicializado\n");

  terminal_setcolor(&term, VGA_COLOR_YELLOW, VGA_COLOR_BLUE);
  terminal_writestring(&term, "Modo Texto VGA 80x25\n");

  terminal_setcolor(&term, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  terminal_writestring(&term, "Terminal configurado com sucesso!\n\n");

  // Demonstração de cores
  for (int bg = 0; bg < 16; bg++) {
    for (int fg = 0; fg < 16; fg++) {
      terminal_setcolor(&term, fg, bg);
      terminal_putchar(&term, 'X');
    }
    terminal_putchar(&term, '\n');
  }
  terminal_setcolor(&term, VGA_COLOR_WHITE, VGA_COLOR_RED);
  terminal_writestring(&term, "\nSistema Pronto!\n");

  show_splash_screen(&term);

  while (1) {
    u_int8_t scancode = term.read_key_scancode();
    if (scancode == term.ENTER_SCANCODE) {
      terminal_writestring(&term, "Reiniciando sistemas...\n");
      term.reboot_system();
    }
  }
}
