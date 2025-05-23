#include "kernel.h"

Terminal term;

void kernel_main(void) {
  terminal_initialize(&term);

  terminal_writestring(&term, "Sistema Kernel Inicializado\n");

  terminal_setcolor(&term, VGA_COLOR_YELLOW, VGA_COLOR_BLUE);
  terminal_writestring(&term, "Modo Texto VGA 80x25\n");

  // Demonstração de cores
  for (int bg = 0; bg < 16; bg++) {
    for (int fg = 0; fg < 16; fg++) {
      terminal_setcolor(&term, fg, bg);
      terminal_putchar(&term, 'X');
    }
    terminal_putchar(&term, '\n');
  }
}
