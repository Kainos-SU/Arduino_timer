typedef struct BUTTON;
struct BUTTON * button_init (int pin, bool pull);
void tick_button (struct BUTTON * button);
bool isShortPush (struct BUTTON * but);
bool isLongPush (struct BUTTON * but);
