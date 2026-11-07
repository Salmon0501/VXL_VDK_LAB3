#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

extern const int MAX_LED;
extern int index_led;
extern int led_buffer[4];

void display7SEG(int num);
void update7SEG(int index);
void update_led_buffer(int mode, int time_road1, int time_road2, int edit_val);
void led_single_2Hz(int mode);
void clear_led_traffic(void);
void led_traffic(int state);

#endif /* INC_LED_DISPLAY_H_ */
