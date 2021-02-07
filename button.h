#ifndef KAINOS_BUTTON
#define KAINOS_BUTTON
typedef struct _button button;
enum state {PRESS, RELESE, PUSHED, FREE};
button * init_button (int pin, bool pullup);
int button_tick (button * but);
struct simp_butt {
  byte state: 1;
  byte pin: 7;
};
enum state getState (button * but);
#endif
