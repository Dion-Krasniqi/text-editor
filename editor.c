#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios og_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &og_termios);
  atexit(disableRawMode);

  struct termios raw = og_termios;
  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
	if (iscntrl(c)) {
		printf("%d\r\n", c);
	} else {
		printf("%d ('%c')\r\n", c, c);
	}
  }

  return 0;
}
