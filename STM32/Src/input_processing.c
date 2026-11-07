#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "timer.h"
#include "led_display.h"

enum ButtonState {
    BUTTON_RELEASED,
    BUTTON_PRESSED,
    BUTTON_PRESSED_MORE_THAN_1_SECOND
};

enum ButtonState button1State = BUTTON_RELEASED;
enum ButtonState button2State = BUTTON_RELEASED;
enum ButtonState button3State = BUTTON_RELEASED;

int mode = 1;
int red_duration = 5;
int yellow_duration = 2;
int green_duration = 3;
int temp_red = 5;
int temp_yellow = 2;
int temp_green = 3;
int road1_time = 5;
int road2_time = 3;
int traffic_state = 0;

void fsm_for_input_processing(void) {
    switch (button1State) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                button1State = BUTTON_PRESSED;
                clear_led_traffic();
                mode++;
                if (mode > 4) mode = 1;
                switch (mode) {
                case 1:
                	temp_red = red_duration;
                	temp_yellow = yellow_duration;
                	temp_green = green_duration;
                	road1_time = red_duration;
                    road2_time = green_duration;
                    traffic_state = 0;
                    setTimer0(1000);
                    break;
                case 2:
                    //temp_duration = red_duration;
                    setTimer1(250);
                    break;
                case 3:
                    //temp_duration = yellow_duration;
                    setTimer1(250);
                    break;
                case 4:
                    //temp_duration = green_duration;
                    setTimer1(250);
                    break;
                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                button1State = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(0)) {
                    button1State = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                button1State = BUTTON_RELEASED;
            }
            break;
    }

    if (mode != 1) {
            switch (button2State) {
            case BUTTON_RELEASED:
            	if (is_button_pressed(1)) {
            		button2State = BUTTON_PRESSED;
            		if (mode == 2) {
            		   temp_red++;
            		   if (temp_red > 99) temp_red = 1;
            		} else if (mode == 3) {
            		   temp_yellow++;
            		   if (temp_yellow > 99) temp_yellow = 1;
            		} else if (mode == 4) {
            		   temp_green++;
            		   if (temp_green > 99) temp_green = 1;
            		}
                }
                break;

             case BUTTON_PRESSED:
                    if (!is_button_pressed(1)) {
                        button2State = BUTTON_RELEASED;
                    } else if (is_button_pressed_1s(1)) {
                        button2State = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                        setTimer2(200);
                    }
                    break;

             case BUTTON_PRESSED_MORE_THAN_1_SECOND:
                    if (!is_button_pressed(1)) {
                        button2State = BUTTON_RELEASED;
                    } else {
                    	if (timer2_flag == 1) {
                    	   setTimer2(200);
                    	   if (mode == 2) {
                    	      temp_red++;
                    	      if (temp_red > 99) temp_red = 1;
                    	   } else if (mode == 3) {
                    	      temp_yellow++;
                    	      if (temp_yellow > 99) temp_yellow = 1;
                    	   } else if (mode == 4) {
                    	      temp_green++;
                    	   if (temp_green > 99) temp_green = 1;
                    	   }
                    	}
                    }
                    break;
            }
        }

    if (mode != 1) {
            switch (button3State) {
                case BUTTON_RELEASED:
                    if (is_button_pressed(2)) {
                        button3State = BUTTON_PRESSED;

                        red_duration = temp_red;
                        yellow_duration = temp_yellow;
                        green_duration = temp_green;
                    }
                    break;

                case BUTTON_PRESSED:
                    if (!is_button_pressed(2)) {
                        button3State = BUTTON_RELEASED;
                    }
                    break;

                case BUTTON_PRESSED_MORE_THAN_1_SECOND:
                    if (!is_button_pressed(2)) {
                        button3State = BUTTON_RELEASED;
                    }
                    break;
            }
        }

    if (mode == 1) {
            led_traffic(traffic_state);
            update_led_buffer(1, road1_time, road2_time, 0);

            if (timer0_flag == 1) {
                setTimer0(1000);
                road1_time--;
                road2_time--;

                if (traffic_state == 0 && road2_time <= 0) {
                    traffic_state = 1;
                    road2_time = yellow_duration;
                } else if (traffic_state == 1 && road2_time <= 0) {
                    traffic_state = 2;
                    road1_time = green_duration;
                    road2_time = red_duration;
                } else if (traffic_state == 2 && road1_time <= 0) {
                    traffic_state = 3;
                    road1_time = yellow_duration;
                } else if (traffic_state == 3 && road1_time <= 0) {
                    traffic_state = 0;
                    road1_time = red_duration;
                    road2_time = green_duration;
                }
            }
        }
    else {
            if (timer1_flag == 1) {
                setTimer1(250);
                led_single_2Hz(mode);
            }
            if (mode == 2) {
                        update_led_buffer(2, 0, 0, temp_red);
                    } else if (mode == 3) {
                        update_led_buffer(3, 0, 0, temp_yellow);
                    } else if (mode == 4) {
                        update_led_buffer(4, 0, 0, temp_green);
                    }
    }
}
