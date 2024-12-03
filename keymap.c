// aodh66 Keyboard Layout

#include QMK_KEYBOARD_H
#include "env.h"
#include "features/achordion.h"

// * -----------------------------
// * -- Home row mods (Recurva) --
// * -----------------------------
// Left-hand
#define HOME_S LGUI_T(KC_S)
#define HOME_N LALT_T(KC_N)
#define HOME_T LSFT_T(KC_T)
#define HOME_C LCTL_T(KC_C)

#define HOME_Z LGUI_T(KC_Z)
#define HOME_X LALT_T(KC_X)
#define HOME_K LSFT_T(KC_K)
#define HOME_G LCTL_T(KC_G)

// Right-hand
#define HOME_H RCTL_T(KC_H)
#define HOME_E RSFT_T(KC_E)
#define HOME_A LALT_T(KC_A)
#define HOME_I RGUI_T(KC_I)

#define HOME_L RCTL_T(KC_L)
#define HOME_CN RSFT_T(KC_SCLN)
#define HOME_QT LALT_T(KC_QUOT)
#define HOME_CM RGUI_T(KC_COMM)

// Layers
#define UNDO LCTL(KC_Z)
#define COPY LCTL(KC_C)
#define CUT LCTL(KC_X)
#define PASTE LCTL(KC_V)
#define FIND LCTL(KC_F)

#define NXT_TAB LCTL(KC_TAB)

#define ALT_F4 LALT(KC_F4)
#define OS_SFT OSM(MOD_LSFT)
#define CTL_BSPC LCTL(KC_BSPC)

// * ------------------
// * -- Tapping Term --
// * ------------------
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    // Increase the tapping term a little for slower ring and pinky fingers.
    case HOME_S:
    case HOME_N:
    case HOME_A:
    case HOME_I:
    case HOME_Z:
    case HOME_X:
    case HOME_QT:
    case HOME_CM:
      return TAPPING_TERM + 15;

    default:
      return TAPPING_TERM;
  }
}

// * ------------------------
// * -- Macro Declarations --
// * ------------------------
enum custom_keycodes {
    SS_QU = SAFE_RANGE,
    BRACES,
    RESIZE,
    EMAIL,
    COMMENT,
};

// * ---------------
// * -- Accordion --
// * ---------------
bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LCTL:
        case MOD_RCTL:
            return true; // Eagerly apply Shift and Ctrl mods.

        default:
            return false;
    }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return 800; // Use a timeout of 800 ms.
}

// Typing Streak
uint16_t achordion_streak_chord_timeout(
    uint16_t tap_hold_keycode, uint16_t next_keycode) {
  if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
    return 0;  // Disable streak detection on layer-tap keys.
  }

  // Otherwise, tap_hold_keycode is a mod-tap key.
  uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
  if ((mod & MOD_LSFT) != 0) {
    return 100;  // A shorter streak timeout for Shift mod-tap keys.
  } else {
    return 240;  // A longer timeout otherwise.
  }
}

bool achordion_streak_continue(uint16_t keycode) {
  // If mods other than shift or AltGr are held, don't continue the streak.
  if (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) return false;
  // This function doesn't get called for holds, so convert to tap keycodes.
  if (IS_QK_MOD_TAP(keycode)) {
    keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
  }
  if (IS_QK_LAYER_TAP(keycode)) {
    keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
  }
  // Regular letters and punctuation continue the streak.
  if (keycode >= KC_A && keycode <= KC_Z) return true;
  switch (keycode) {
    case KC_DOT:
    case KC_COMMA:
    case KC_QUOTE:
    case KC_SPACE:
      return true;
  }
  return false;  // All other keys end the streak.
}

    // // ! Might be in wrong place
    // const uint8_t mods         = get_mods();
    // const uint8_t oneshot_mods = get_oneshot_mods();

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    // ? Achordion
    if (!process_achordion(keycode, record)) {
        return false;
    }

        // ! Might be in wrong place
    const uint8_t mods         = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();

    switch (keycode) {
            // * Macros
        case SS_QU:
            if (record->event.pressed) {
                SEND_STRING("qu");
            }
            break;

            case RESIZE:
            if (record->event.pressed) {
                set_mods(MOD_MASK_CSAG);
                register_code(KC_X);
            } else {
                clear_mods();
                unregister_code(KC_X);
            }
            // Do not let QMK process the keycode further
            return false;

        case EMAIL:
            if (record->event.pressed) {
                SEND_STRING(EMAIL_STRING);
            }
            break;

        case COMMENT:
            if (record->event.pressed) {
                clear_oneshot_mods(); // Temporarily disable mods.
                register_code(KC_LCTL);// Hold Ctrl.
                tap_code(KC_SLSH);   // Hit slash to comment out text.
                unregister_code(KC_LCTL); // Stops holding Ctrl.
            }
            break;

        case BRACES: // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
                clear_oneshot_mods(); // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    SEND_STRING("{}");
                } else if ((mods | oneshot_mods) & MOD_MASK_ALT) {
                    SEND_STRING("<>");
                } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                    SEND_STRING("[]");
                } else {
                    SEND_STRING("()");
                }
                tap_code(KC_LEFT);   // Move cursor between braces.
                register_mods(mods); // Restore mods.
            }
            return false;
    }

    return true;
};

void matrix_scan_user(void) {
    achordion_task();
}

// * -----------------------
// * -- Layer Definitions --
// * -----------------------
#define _ALPHA 0
#define _AKL 5
#define _NAV 1
#define _SYM 2
#define _NUM 3
#define _FN 4

// * ------------
// * -- Layout --
// * ------------
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Base (Recurva Colstag)
    [_ALPHA] = LAYOUT_split_3x5_2(
        KC_F, KC_R, KC_D, KC_P, KC_V,                               SS_QU, KC_M, KC_U, KC_O, KC_Y, 
        KC_S, KC_N, KC_T, KC_C, KC_B,                               KC_DOT, KC_H,  KC_E,  KC_A,  KC_I,
        HOME_Z, HOME_X, HOME_K, HOME_G, KC_W,                       KC_J,   HOME_L,  HOME_CN, HOME_QT, HOME_CM,
        MO(_NAV), KC_SPC,                                           OS_SFT, MO(_SYM)),

    // Nav/Ext
    [_NAV] = LAYOUT_split_3x5_2(
        KC_ESC, KC_CAPS, KC_MEH, KC_HYPR, ALT_F4,                           KC_PGUP, KC_HOME, BRACES, KC_END, KC_DEL, 
        KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, RESIZE,                         KC_PGDN, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, 
        UNDO, CUT, COPY, PASTE, KC_DEL,                                     KC_BSPC, NXT_TAB, KC_TAB, KC_DEL, COMMENT, 
        KC_TRNS, KC_NO,                                                     CW_TOGG, MO(_NUM)),

    // Symbol
    [_SYM] = LAYOUT_split_3x5_2(
        KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                           KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_PIPE, 
        KC_GRV, KC_LT, KC_LCBR, KC_LPRN, KC_LBRC,                           KC_EQL, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, 
        KC_TILD, KC_GT, KC_RCBR, KC_RPRN, KC_RBRC,                          KC_PLUS, KC_MINS, KC_QUES, KC_BSLS, KC_SLSH,
        MO(_NUM), KC_ENT,                                                      KC_NO, KC_TRNS),

    // Numpad
    [_NUM] = LAYOUT_split_3x5_2(
        MO(_FN), KC_NO, QK_BOOT, KC_NO, KC_VOLU,                                  KC_MINS, KC_7, KC_8, KC_9, KC_ASTR, 
        KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_VOLD,                          KC_DOT, KC_4, KC_5, KC_6, KC_0, 
        EMAIL, KC_NO, KC_BSPC, KC_DEL, KC_NO,                               KC_PLUS, KC_1, KC_2, KC_3, KC_SLSH, 
        KC_NO, KC_TRNS,                                                     KC_BSPC, KC_DEL),

    // Function
    [_FN] = LAYOUT_split_3x5_2(
        KC_NO, KC_NO, QK_BOOT, KC_NO, KC_VOLU,                              KC_NO, KC_F7, KC_F8, KC_F9, KC_NO, 
        KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_VOLD,                        KC_F12, KC_F4, KC_F5, KC_F6, KC_F10, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_MPLY,                          KC_F11, KC_F1, KC_F2, KC_F3, KC_NO, 
        KC_TRNS, KC_NO,                                                     KC_NO, KC_TRNS),
};

// * -------------------------------------------
// * -- Combos (Currently set up for Recurva) --
// * -------------------------------------------
// Key Definitions
// Left Hand
const uint16_t PROGMEM combo1[] = {SS_QU, KC_U, COMBO_END}; // Q+U -> Q
const uint16_t PROGMEM combo17[] = {SS_QU, KC_M, COMBO_END}; // Q+M -> Q

const uint16_t PROGMEM combo2[] = {HOME_K, HOME_G, COMBO_END}; // K+G -> ESC
const uint16_t PROGMEM combo3[] = {HOME_X, HOME_K, COMBO_END}; // X+K -> Tab

// const uint16_t PROGMEM combo4[] = {KC_F, HOME_S, COMBO_END}; // F+S -> Grave `
// const uint16_t PROGMEM combo5[] = {KC_R, HOME_N, COMBO_END}; // R+N -> Less than <
// const uint16_t PROGMEM combo6[] = {KC_D, HOME_T, COMBO_END}; // D+T -> Left Curly Brace {
// const uint16_t PROGMEM combo7[] = {KC_P, HOME_C, COMBO_END}; // P+C -> Left Parenthesis (
// const uint16_t PROGMEM combo8[] = {KC_V, KC_B, COMBO_END};   // V+B -> Left Brace [

// Right Hand
const uint16_t PROGMEM combo9[] = {HOME_CN, HOME_QT, COMBO_END}; // /+' -> Enter
const uint16_t PROGMEM combo10[] = {HOME_G, HOME_L, COMBO_END};     // G+L -> Caps Word
const uint16_t PROGMEM combo11[] = {HOME_L, HOME_CM, COMBO_END};    // L+/ -> Delete Bot

// const uint16_t PROGMEM combo12[] = {SS_QU, KC_DOT, COMBO_END}; // Q+. -> Right Brace ]
// const uint16_t PROGMEM combo13[] = {KC_M, HOME_H, COMBO_END};  // M+H -> Right Parenthesis )
// const uint16_t PROGMEM combo14[] = {KC_U, HOME_E, COMBO_END};  // U+E -> Right Curly Brace }
// const uint16_t PROGMEM combo15[] = {KC_O, HOME_A, COMBO_END};  // O+A -> Greater than >
// const uint16_t PROGMEM combo16[] = {KC_Y, HOME_I, COMBO_END};  // Y+I -> Semicolon ;

// Outcome Definitions
combo_t key_combos[COMBO_COUNT] = {
    // Left Hand
    COMBO(combo1, KC_Q), // Q+U -> Q
    COMBO(combo17, KC_Q), // Q+M -> Q

    COMBO(combo2, KC_ESC), // K+G -> ESC
    COMBO(combo3, KC_TAB), // X+K -> Tab

    // COMBO(combo4, KC_GRV),  // F+S -> Grave `
    // COMBO(combo5, KC_LT),   // R+N -> Less than <
    // COMBO(combo6, KC_LCBR), // D+T -> Left Curly Brace {
    // COMBO(combo7, KC_LPRN), // P+C -> Left Parenthesis (
    // COMBO(combo8, KC_LBRC), // V+B -> Left Brace [

    // Right Hand
    COMBO(combo9, KC_ENT),  // /+' -> Enter
    COMBO(combo10, CW_TOGG), // G+L -> Caps Word
    COMBO(combo11, KC_DEL),  // L+/ -> Delete Bot

    // COMBO(combo12, KC_RBRC), // Q+. -> Right Brace ]
    // COMBO(combo13, KC_RPRN), // M+H -> Right Parenthesis )
    // COMBO(combo14, KC_RCBR), // U+E -> Right Curly Brace }
    // COMBO(combo15, KC_GT),   // O+A -> Greater than >
    // COMBO(combo16, KC_SCLN), // Y+I -> Semicolon ;
};
