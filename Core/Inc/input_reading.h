#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#define BUTTON_SCAN_MS   10    /* chu ky doc nut (chong doi) */

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */
