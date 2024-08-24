#include "usart.h"
#include <stdint.h>

#define BUFFER_LEN 16 // idx_max: u8
#define BUFFER_SIZE 255

uint8_t buffer[BUFFER_LEN][BUFFER_SIZE] = {0}, idx = 0;

extern int vsnprintf(char *, unsigned int, const char *, __builtin_va_list);

void uprintf(const char *format, ...) {
  __builtin_va_list arg;
  __builtin_va_start(arg, format);
  uint8_t len = vsnprintf((char *)(buffer[idx]), BUFFER_SIZE, format, arg);
  __builtin_va_end(arg);

  HAL_UART_Transmit_DMA(&huart1, buffer[idx++], len);

  if (idx >= BUFFER_LEN)
    idx = 0;
}

int _write(int file, char *ptr, int len) {
  // todo: write to file
  return -1;
}

int _read(int file, char *ptr, int len) {
  // todo: read file
  return 0;
}

int _close(int file) {
  // todo: close file
  return 0;
}

int _fstat(int file, void *st) {
  // todo: get file status
  return -1;
}

int _lseek(int file, int ptr, int dir) {
  // todo: seek file
  return -1;
}

int _isatty(int file) {
  // todo: check if file is a tty
  return 0;
}
