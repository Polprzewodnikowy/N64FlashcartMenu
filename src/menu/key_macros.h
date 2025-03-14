
#ifndef __N64_CONTROLLER_KEYS_MACRO_H
#define __N64_CONTROLLER_KEYS_MACRO_H

#include <stdio.h>
#include "utils/utils.h"


#define KEYBOARD_ESC 0x0A08
#define KEYBOARD_F1  0x0B01
#define KEYBOARD_F2  0x0A01
#define KEYBOARD_F3  0x0B08
#define KEYBOARD_F4  0x0A07
#define KEYBOARD_F5  0x0B07
#define KEYBOARD_F6  0x0A02
#define KEYBOARD_F7  0x0B02
#define KEYBOARD_F8  0x0A03
#define KEYBOARD_F9  0x0B03
#define KEYBOARD_F10 0x0A04
#define KEYBOARD_F11 0x0203
#define KEYBOARD_F12 0x0B06

#define KEYBOARD_NUM_LOCK 0x0A05
#define KEYBOARD_NUM_LOCK_1 0x0B05
#define KEYBOARD_NUM_LOCK_2 0x0208 
#define KEYBOARD_NUM_LOCK_3 0x0207
#define KEYBOARD_ESC_UNDER 0x0D05
#define KEYBOARD_1 0x0C05 
#define KEYBOARD_2 0x0505
#define KEYBOARD_3 0x0605
#define KEYBOARD_4 0x0705
#define KEYBOARD_5 0x0805
#define KEYBOARD_6 0x0905
#define KEYBOARD_7 0x0906
#define KEYBOARD_8 0x0806
#define KEYBOARD_9 0x0706 
#define KEYBOARD_0 0x0606 
#define KEYBOARD_EQUALS 0x0506 
#define KEYBOARD_HAT 0x0C06 
#define KEYBOARD_BACKSPACE 0x0D06

#define KEYBOARD_TAB 0x0D01
#define KEYBOARD_Q   0x0C01
#define KEYBOARD_W   0x0501
#define KEYBOARD_E   0x0601
#define KEYBOARD_R   0x0701
#define KEYBOARD_T   0x0801
#define KEYBOARD_Y   0x0901
#define KEYBOARD_U   0x0904
#define KEYBOARD_I   0x0804
#define KEYBOARD_O   0x0704
#define KEYBOARD_P   0x0604
#define KEYBOARD_AT  0x0504
#define KEYBOARD_SQUARE_OPEN  0x0C04
#define KEYBOARD_SQUARE_CLOSE 0x0406

#define KEYBOARD_CAPS_LOCK 0x0F05
#define KEYBOARD_A 0x0D07
#define KEYBOARD_S 0x0C07
#define KEYBOARD_D 0x0507
#define KEYBOARD_F 0x0607
#define KEYBOARD_G 0x0707
#define KEYBOARD_H 0x0807
#define KEYBOARD_J 0x0907
#define KEYBOARD_K 0x0903
#define KEYBOARD_L 0x0803
#define KEYBOARD_DOT_COMA 0x0703
#define KEYBOARD_DOUBLE_COMAS 0x0603
#define KEYBOARD_ENTER 0x0D04

#define KEYBOARD_SHIFT_LEFT 0x0E01
#define KEYBOARD_Z 0x0D08
#define KEYBOARD_X 0x0C08
#define KEYBOARD_C 0x0508
#define KEYBOARD_V 0x0608
#define KEYBOARD_B 0x0708
#define KEYBOARD_N 0x0808
#define KEYBOARD_M 0x0908
#define KEYBOARD_ARROW_OPEN 0x0902
#define KEYBOARD_ARROW_CLOSE 0x0802
#define KEYBOARD_QUESTION 0x0702
#define KEYBOARD_MINUS 0x1004
#define KEYBOARD_UP 0x0204
#define KEYBOARD_SHIFT_RIGHT 0x0E06

#define KEYBOARD_CRTL 0x1107
#define KEYBOARD_OPT 0x0F07
#define KEYBOARD_TWO_LINES 0x1105
#define KEYBOARD_ALT 0x1008
#define KEYBOARD_ALT_1 0x1002
#define KEYBOARD_SPACE 0x0602
#define KEYBOARD_SPACE_1 0x0E02
#define KEYBOARD_SPACE_2 0x1006
#define KEYBOARD_HOME 0x9999    //FIXME !
#define KEYBOARD_END 0x0206
#define KEYBOARD_LEFT 0x0205
#define KEYBOARD_DOWN 0x0305
#define KEYBOARD_RIGHT 0x0405

char * identify_key(uint16_t _value)
{
    return " ";
    /*
    switch( _value)
    {
        //Line 1
        case KEYBOARD_ESC: return "ESC";
        case KEYBOARD_F1: return "F1";
        case KEYBOARD_F2: return "F2";
        case KEYBOARD_F3: return "F3";
        case KEYBOARD_F4: return "F4";
        case KEYBOARD_F5: return "F5";
        case KEYBOARD_F6: return "F6";
        case KEYBOARD_F7: return "F7";
        case KEYBOARD_F8: return "F8";
        case KEYBOARD_F9: return "F9";
        case KEYBOARD_F10: return "F10";
        case KEYBOARD_F11: return "F11";
        case KEYBOARD_F12: return "F12";

        //Line 2
        case KEYBOARD_NUM_LOCK_1: return  "Num_Lock_1";
        case KEYBOARD_NUM_LOCK_2: return "Num_Lock_2";
        case KEYBOARD_NUM_LOCK_3: return "Num_Lock_3";
        case KEYBOARD_ESC_UNDER: return "ESC_Under";
        case KEYBOARD_1: return gl_shift_pressed == true ? "!" : "1";
        case KEYBOARD_2: return gl_shift_pressed == true ? "\"": "2";
        case KEYBOARD_3: return gl_shift_pressed == true ? "#" : "3";
        case KEYBOARD_4: return gl_shift_pressed == true ? "$" : "4";
        case KEYBOARD_5: return gl_shift_pressed == true ? "%" : "5";
        case KEYBOARD_6: return gl_shift_pressed == true ? "&" : "6";
        case KEYBOARD_7: return gl_shift_pressed == true ? "'" : "7";
        case KEYBOARD_8: return gl_shift_pressed == true ? "(" : "8";
        case KEYBOARD_9: return gl_shift_pressed == true ? ")" : "9";
        case KEYBOARD_0: return gl_shift_pressed == true ? "*" : "0";
        case KEYBOARD_EQUALS: return "=";
        case KEYBOARD_HAT: return "^";
        case KEYBOARD_BACKSPACE: return "BACKSPACE";

        //Line 3
        case KEYBOARD_TAB : return "\t";
        case KEYBOARD_Q: return gl_caps_lock == true ? "Q" : "q";
        case KEYBOARD_W: return gl_caps_lock == true ? "W" : "w";
        case KEYBOARD_E: return gl_caps_lock == true ? "E" : "e";
        case KEYBOARD_R: return gl_caps_lock == true ? "R" : "r";
        case KEYBOARD_T: return gl_caps_lock == true ? "T" : "t";
        case KEYBOARD_Y: return gl_caps_lock == true ? "Y" : "y";
        case KEYBOARD_U: return gl_caps_lock == true ? "U" : "u";
        case KEYBOARD_I: return gl_caps_lock == true ? "I" : "i";
        case KEYBOARD_O: return gl_caps_lock == true ? "O" : "o";
        case KEYBOARD_P: return gl_caps_lock == true ? "P" : "p";
        case KEYBOARD_AT: return "@";
        case KEYBOARD_SQUARE_OPEN: return "[";
        case KEYBOARD_SQUARE_CLOSE: return "]";

        //Line 4

        case KEYBOARD_A: return gl_caps_lock == true ? "A" : "a";
        case KEYBOARD_S: return gl_caps_lock == true ? "S" : "s";
        case KEYBOARD_D: return gl_caps_lock == true ? "D" : "d";
        case KEYBOARD_F: return gl_caps_lock == true ? "F" : "f";
        case KEYBOARD_G: return gl_caps_lock == true ? "G" : "g";
        case KEYBOARD_H: return gl_caps_lock == true ? "H" : "h";
        case KEYBOARD_J: return gl_caps_lock == true ? "J" : "j";
        case KEYBOARD_K: return gl_caps_lock == true ? "K" : "k";
        case KEYBOARD_L: return gl_caps_lock == true ? "L" : "l";
        case KEYBOARD_DOT_COMA: return ";";
        case KEYBOARD_DOUBLE_COMAS: return ":";
        case KEYBOARD_ENTER: return "\n";

        //Line 5
        case KEYBOARD_Z : return gl_caps_lock == true ? "Z" : "z";
        case KEYBOARD_X : return gl_caps_lock == true ? "X" : "x";
        case KEYBOARD_C : return gl_caps_lock == true ? "C" : "c";
        case KEYBOARD_V : return gl_caps_lock == true ? "V" : "v";
        case KEYBOARD_B : return gl_caps_lock == true ? "B" : "b";
        case KEYBOARD_N : return gl_caps_lock == true ? "N" : "n";
        case KEYBOARD_M : return gl_caps_lock == true ? "M" : "m";
        case KEYBOARD_ARROW_OPEN : return "<";
        case KEYBOARD_ARROW_CLOSE : return ">";
        case KEYBOARD_QUESTION : return "?";
        case KEYBOARD_MINUS : return "-";
        case KEYBOARD_UP : return "UP";

        //Line 6
        case KEYBOARD_TWO_LINES : return gl_shift_pressed ? "€" : "|"; 
        case KEYBOARD_ALT_1 : return "ALT_1";
        case KEYBOARD_SPACE : return " ";
        case KEYBOARD_SPACE_1 : return "SPACE_1";
        case KEYBOARD_SPACE_2 : return "SPACE_2";
        case KEYBOARD_HOME : return "HOME";
        case KEYBOARD_END : return "END";
        case KEYBOARD_LEFT : return "LEFT";
        case KEYBOARD_DOWN : return "DOWN";
        case KEYBOARD_RIGHT : return "RIGHT";

        default:     return "";
    }
    */

}

#endif