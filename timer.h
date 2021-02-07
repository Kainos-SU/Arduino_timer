#ifndef KAINOS_BUTTON
#define KAINOS_BUTTON

struct time_numbers {
  byte seconds_low: 4;
  byte seconds_high: 3;
  byte minutes_low: 4;
  byte minutes_high: 4;
  byte hours_low: 4;
  byte hours_high: 4;
};



struct timer {
  struct time_numbers data;
  byte mode: 2;
  /*
   * mode:
   * 0 - очікування пуску таймера
   * 1 - таймер запущений
   * 2 - таймер зупинений
   * 3 - таймер обнулений
   */
  byte changed: 1;
  //Значення таймеру змінилось
  byte hour_count: 1;
};

struct timer * init_timer (int n);
void timer_update (struct timer *t);
void timer_tick(struct timer *t);
void timer_start (struct timer *t);
char *getTime (struct timer *t);


#endif
