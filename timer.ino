#include <stdlib.h>
#include <Arduino.h>
#include <stdio.h>

char time_format[] = "%d%d:%d%d:%d%d";
char time_data[sizeof (time_format) / sizeof(char)];

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

struct timer * init_timer (int n) {
  struct timer *t = (struct timer *) malloc(sizeof(struct timer) * n);
  for (int i = 0; i < n; ++i) {
    *t = (struct timer){(struct time_numbers){0,0,0,0,0,0}, 0, 1, 0};
    ++t;
  }
  return t - n; 
}

void timer_update (struct timer *t) {
  t->changed = 1;
  struct time_numbers *temp = &(t->data);
  ++temp->seconds_low;
  if (temp->seconds_low == 10){
    temp->seconds_low = 0;
    ++temp->seconds_high;
    if (temp->seconds_high == 6) {
      temp->seconds_high = 0;
      ++temp->minutes_low;
      if (temp->minutes_low == 10) {
        temp->minutes_low = 0;
        ++temp->minutes_high;
        if (temp->minutes_high == 6) {
          temp->minutes_high = 0;
          ++temp->hours_low;
          if (temp->hours_low == 9) {
            t->mode = 2;
          }
        }
      }
    }
  }
}

void timer_tick(struct timer *t) {
  if (t->mode == 1) {
    timer_update(t);
  }else if (t->mode == 3) {
    t->data = (struct time_numbers) {0,0,0,0,0,0};
    t->mode = 0;
  }
}

void timer_start (struct timer *t) {
  if (t->mode == 0) {
    t->mode = 1;
  }else if (t->mode == 1){
    t->mode = 2;
  }else if (t->mode == 2) {
    t->mode = 3;
  }
}

char *getTime (struct timer *t){
  if (t->changed == 1) {
    struct time_numbers *data = &(t->data);
    sprintf (time_data, time_format, data->hours_high, data->hours_low, data->minutes_high, data->minutes_low, data->seconds_high, data->seconds_low);
    return time_data;
  }
  else
    return NULL;
}
