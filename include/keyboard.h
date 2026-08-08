#ifndef KEYBOARD_H
#define KEYBOARD_H

int keyboard_open(const char *device_path);
void keyboard_listen(int fd);
void keyboard_close(int fd);

#endif
