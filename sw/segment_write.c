#include "xgpio.h"
#include "xparameters.h"
#include "IP_Centrale_DCC.h"

void write_digit(XGpio *segment, u32 number) {
    // Bits: [g f e d c b a]
    static const u32 segment_map[10] = {
    		0xC0, // 0
			0xF9, // 1
			0xA4, // 2
			0xB0, // 3
			0x99, // 4
			0x92, // 5
			0x82, // 6
			0xF8, // 7
			0x80, // 8
			0x90  // 9
    };

    // only write if number is between 0 and 9
    if (number <= 9) {
        XGpio_DiscreteWrite(segment, 1, segment_map[number]);
    } else {
        XGpio_DiscreteWrite(segment, 1, 0xFF);
    }
}

void select_digit(XGpio *segment, u32 digit_index) {
    // Digit index: 0 is the rightmost, 3 is the leftmost
    // Anodes are bits 8, 9, 10, 11
    // Logic: Active-Low (0 = ON, 1 = OFF)

    u32 anode_pattern;

    switch(digit_index) {
        case 0: anode_pattern = 0xE; // 1110 (Digit 0 ON) - Binary: ...1110 00000000
                break;
        case 1: anode_pattern = 0xD; // 1101 (Digit 1 ON) - Binary: ...1101 00000000
                break;
        case 2: anode_pattern = 0xB; // 1011 (Digit 2 ON) - Binary: ...1011 00000000
                break;
        case 3: anode_pattern = 0x7; // 0111 (Digit 3 ON) - Binary: ...0111 00000000
                break;
        default: anode_pattern = 0xF; // All OFF
                break;
    }



    // Combine segment data with new anode selection
    XGpio_DiscreteWrite(segment, 2,  anode_pattern);
}
void write_number(XGpio *segment, u32 total_number,u32 del) {

    if (total_number > 9999) total_number = 9999;


    u32 digits[4];
    digits[0] = total_number % 10;
    digits[1] = (total_number / 10) % 10;
    digits[2] = (total_number / 100) % 10;
    digits[3] = (total_number / 1000) % 10;

    for (int i = 0; i < 4; i++) {
        select_digit(segment, i);
        write_digit(segment, digits[i]);
        for(volatile int delay = 0; delay < del; delay++);
        write_digit(segment, 10);
    }
}
void write_letter_num(XGpio *segment, u32 letter_idx, u32 number, u32 delay_timer) {
    // Array: 0-9, 10:OFF, 11:A, 12:b, 13:C, 14:d, 15:E, 16:F, 17:P
    static const u32 display_map[] = {
        0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
        0xFF, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,0X0C
    };
    if (number > 999) number = 999;

    u32 patterns[4];
    patterns[0] = display_map[number % 10];
    patterns[1] = display_map[(number / 10) % 10];
    patterns[2] = display_map[(number / 100) % 10];
    patterns[3] = display_map[letter_idx] & 0x7F;
    for (int i = 0; i < 4; i++) {
        select_digit(segment, i);
        XGpio_DiscreteWrite(segment, 1, patterns[i]);
        for (volatile int d = 0; d < delay_timer; d++);
        XGpio_DiscreteWrite(segment, 1, display_map[10]);
    }
}
