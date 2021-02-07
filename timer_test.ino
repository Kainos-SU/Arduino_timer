#include <LiquidCrystal.h>
#include "button.h"
#include "timer.h"

#define rs 12
#define en 11
#define d4 6
#define d5 5
#define d6 4
#define d7 3
#define BUT0 10
#define BUT1 9
#define BUT2 8
#define BUT3 7
#define ROWS 4
#define DEBOUNCE 5

bool cur = true;
button *knopca[ROWS] = {};
struct timer *tmp[ROWS] = {};
unsigned long long int prev = 0;
unsigned int counter = 0;


LiquidCrystal lcd (rs, en, d4, d5, d6, d7);

void setup() {
  knopca[0] = init_button(BUT0, true);
  knopca[1] = init_button(BUT1, true);
  knopca[2] = init_button(BUT2, true);
  knopca[3] = init_button(BUT3, true);
  for (int i = 0; i < ROWS; ++i) {
    tmp[i] = init_timer(1);
  }
  lcd.begin(16, ROWS);
}

void loop() {
  if (millis() - prev >= 10) {
    prev = millis();
    ++counter;
    if (counter == 100){
      for (int i = 0; i < ROWS; ++i) {
        timer_tick (tmp[i]);
      }
      counter = 0;
    }
    for (int i = 0; i < ROWS; ++i) {
      button_tick (knopca[i]);
    }
  }
  for (int i = 0; i < ROWS; ++i){
    enum state STATE = getState(knopca[i]);
    if (STATE == PRESS) {
      timer_start (tmp[i]);
    }
  }
  for (int i = 0; i < ROWS; ++i) {
    char *text = getTime(tmp[i]);
    if (text != NULL) {
      lcd.setCursor(0,0);
      lcd.print(text);
    }
  }
}
