#ifndef KEYBOARD_H
#define KEYBOARD_H

void handlekey(char status, unsigned char key, int x, int y);
void keyup(unsigned char key, int x, int y);
void keypress(unsigned char key, int x, int y);
void escHandler();
#endif
