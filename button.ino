#include <stdlib.h>
#include "button.h"

typedef struct _button{
  enum state state: 2;
  enum state current: 2;
  byte debounce: 7;
  byte pullup: 1;
  byte pin;
}button;

button * init_button (int pin, bool pullup) {
  button *result = (button *) malloc(sizeof(button));
  result->pin = pin;
  result->debounce = 0;
  result->pullup = pullup;
  pinMode(result->pin, INPUT);
  int state = digitalRead(result->pin);
  if (pullup){
    if (state = HIGH) {
      result->state = FREE;
      result->current = FREE;
    }else {
      result->state = PUSHED;
      result->current = PUSHED;
    }
  } else {
    if (state) {
      result->state = PUSHED;
      result->current = PUSHED;
    } else {
      result->state = FREE;
      result->current = FREE;
    }
  }

  return result;
}

int button_tick (button * but) {
  but->current = (enum state)digitalRead(but->pin);
    if ((but->state == FREE && but->current == but->pullup)) {
      but->current = FREE;
      but->debounce = 0;
      return but->current;
    } else if ((but->state == PUSHED && but->current != but->pullup)) {
      but->current = PUSHED;
      but->debounce = 0;
      return but->current;
    }else {
      ++(but->debounce);
    }

    if (but->debounce == DEBOUNCE){
      if (but->state == FREE && but->current != but->pullup) {
        but->state = PRESS;
        but->debounce = 0;
      }else if (but->state == PUSHED && but->current == but->pullup) {
        but->state = RELESE;
        but->debounce = 0;
      }
    }
    return but->state;
}

enum state getState (button * but) {
  enum state result = but->state;
  if (but->state == PRESS || but->state == RELESE) {
    if (but->state == PRESS) {
      but->state = PUSHED;
    }
    else {
      but->state = FREE;
    }
  }

  return result;
}
