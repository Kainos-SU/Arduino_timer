#include <stdlib.h>
#define DEBOUNCE 20
#define LONGPRESS 100

typedef struct BUTTON {
  unsigned int pull : 1;
  unsigned int short_push : 1;
  unsigned int long_push : 1;
  unsigned int prev : 1;
  unsigned int cur : 1;
  unsigned int state : 1;
  unsigned int temp : 10;
  byte pin;
};

struct BUTTON * button_init (int pin, bool pull) {
  struct BUTTON *result = (struct BUTTON *)malloc (sizeof(struct BUTTON));
  *result = (struct BUTTON){pull, 0, 0, pull, pull, 0, 0, pin};
  pinMode (result->pin, INPUT);
  return result;
}

void tick_button (struct BUTTON * button) {
  button->cur = digitalRead(button->pin);
  ++button->temp;
  if (button->cur == button->prev && button->cur != button->pull && !button->state) {
    if (button->temp == DEBOUNCE){
      button->state = true;
      button->temp = 0;
    }
  } else if (button-> state && button->cur == button->pull) {
    if (button->temp < LONGPRESS) {
      button->long_push = false;
      button->short_push = true;
    } else {
      button->long_push = true;
      button->short_push = false;
    }
    button->state = false;
  }

  if (button->cur != button->prev) {
    button->temp = 0;
  }

  button->prev = button->cur;
}

bool isShortPush (struct BUTTON * but) {
  if (but->short_push) {
    but->short_push = false;
    but->long_push = false;
    return true;
  }
  return false;
}

bool isLongPush (struct BUTTON * but) {
  if (but->long_push) {
    but->long_push = false;
    but->short_push = false;
    return true;
  }
  return false;
}
