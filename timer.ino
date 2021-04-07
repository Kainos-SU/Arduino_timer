#include <stdlib.h>
#include <Arduino.h>
#include <stdio.h>

char time_format[] = "%02d:%02d:%02d:%02d";
char time_data[sizeof (time_format) / sizeof(char)];

struct time_numbers {
  unsigned int miliseconds: 10;
  unsigned int seconds: 6;
  unsigned int minutes: 6;
  unsigned int hours: 7;
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
    *t = (struct timer){(struct time_numbers){0,0,0,0}, 0, 1, 0};
    ++t;
  }
  return t - n; 
}

void timer_update (struct timer *t) {
  t->changed = 1;
  struct time_numbers *temp = &(t->data);
  ++temp->miliseconds;
  if (temp->miliseconds == 1000) {
    temp->miliseconds = 0;
    ++(temp->seconds);
    if (temp->seconds == 60) {
      temp->seconds = 0;
      ++(temp->minutes);
      if (temp->minutes == 60) {
        temp->minutes = 0;
        ++(temp->hours);
        if (temp->hours == 90) {
          timer->mode = 2;
        }
      }
    }
  }
}

void timer_tick(struct timer *t) {
  if (t->mode == 1) {
    timer_update(t);
  }else if (t->mode == 3) {
    t->data = (struct time_numbers) {0,0,0,0};
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
    sprintf (time_data, time_format, data->hours, data->minutes, data->seconds, data->miliseconds / 10);
    return time_data;
  }
  else
    return NULL;
}
