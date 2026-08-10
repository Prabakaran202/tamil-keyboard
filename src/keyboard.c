#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "keyboard.h"

struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode); // ப்ரோக்ராம் முடியும்போது தானாகவே பழைய நிலைக்கு மாற
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // எழுத்துக்கள் திரையில் தெரியாமல், பட்டனை மட்டும் பிடிக்க
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char get_keypress() {
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    return c;
}
