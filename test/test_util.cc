#include "test_util.hh"

std::string KeyToString(KeyboardKey key)
{
    switch(key) {
        case KEY_ZERO: return "KEY_ZERO";
        case KEY_ONE: return "KEY_ONE";
        case KEY_TWO: return "KEY_TWO";
        case KEY_THREE: return "KEY_THREE";
        case KEY_FOUR: return "KEY_FOUR";
        case KEY_FIVE: return "KEY_FIVE";
        case KEY_SIX: return "KEY_SIX";
        case KEY_SEVEN: return "KEY_SEVEN";
        case KEY_EIGHT: return "KEY_EIGHT";
        case KEY_NINE: return "KEY_NINE";
        case KEY_A: return "KEY_A";
        case KEY_B: return "KEY_B";
        case KEY_C: return "KEY_C";
        case KEY_D: return "KEY_D";
        case KEY_E: return "KEY_E";
        case KEY_F: return "KEY_F";
        case KEY_G: return "KEY_G";
        case KEY_H: return "KEY_H";
        case KEY_I: return "KEY_I";
        case KEY_J: return "KEY_J";
        case KEY_K: return "KEY_K";
        case KEY_L: return "KEY_L";
        case KEY_M: return "KEY_M";
        case KEY_N: return "KEY_N";
        case KEY_O: return "KEY_O";
        case KEY_P: return "KEY_P";
        case KEY_Q: return "KEY_Q";
        case KEY_R: return "KEY_R";
        case KEY_S: return "KEY_S";
        case KEY_T: return "KEY_T";
        case KEY_U: return "KEY_U";
        case KEY_V: return "KEY_V";
        case KEY_W: return "KEY_W";
        case KEY_X: return "KEY_X";
        case KEY_Y: return "KEY_Y";
        case KEY_Z: return "KEY_Z";
        case KEY_SPACE: return "KEY_SPACE";
        case KEY_ESCAPE: return "KEY_ESCAPE";
        case KEY_ENTER: return "KEY_ENTER";
        case KEY_TAB: return "KEY_TAB";
        case KEY_BACKSPACE: return "KEY_BACKSPACE";
        case KEY_COMMA: return "KEY_COMMA";
        case KEY_PERIOD: return "KEY_PERIOD";
        case KEY_DELETE: return "KEY_DELETE";
        case KEY_MINUS: return "KEY_MINUS";
        case KEY_EQUAL: return "KEY_EQUAL";
        case KEY_RIGHT: return "KEY_RIGHT";
        case KEY_LEFT: return "KEY_LEFT";
        case KEY_DOWN: return "KEY_DOWN";
        case KEY_UP: return "KEY_UP";
        default: return "UNKNOWN_KEY";
    }
}
