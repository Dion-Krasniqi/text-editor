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
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  while (1) {
	char c = '\0';
	read(STDIN_FILENO, &c, 1);
	if (iscntrl(c)) {
		printf("%d\r\n", c);
	} else {
		printf("%d ('%c')\r\n", c, c);
	}
	if (c == 'q') break;
  }

  return 0;
}
