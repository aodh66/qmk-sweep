#include QMK_KEYBOARD_H
#include "features/achordion.h"


//* Home row mods (Use when you've switched to Recurva)
// // Left-hand home row mods
// #define HOME_A LGUI_T(KC_A)
// #define HOME_R LALT_T(KC_R)
// #define HOME_S LSFT_T(KC_S)
// #define HOME_T LCTL_T(KC_T)

// // Right-hand home row mods
// #define HOME_N RCTL_T(KC_N)
// #define HOME_E RSFT_T(KC_E)
// #define HOME_I LALT_T(KC_I)
// #define HOME_O RGUI_T(KC_O)


//* Macro Declarations
enum custom_keycodes {
	SS_POUND = SAFE_RANGE,
	EMAIL,
	SS_QU,
};

// * Tap Dance declarations
enum {
    TD_SPC_ENT,
	TD_CTRLBKSP_BKSP,
	TD_DOLLAR_POUND,
	TD_TAP_DOLLAR_POUND,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// Accordion
	if (!process_achordion(keycode, record)) { return false; }

	// Tap Dance
    tap_dance_action_t *action;

    switch (keycode) {
	// Tap Dance
        case TD(TD_DOLLAR_POUND):  // list all tap dance keycodes with tap-hold configurations
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    
	// Macros
        case SS_POUND:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_RALT("4")));
            }
            break;

		case EMAIL:
        if (record->event.pressed) {
            SEND_STRING("aodhan66@gmail.com");
        } 
        break;

		case SS_QU:
        if (record->event.pressed) {
            SEND_STRING("qu");
        } 
        break;

	}
    return true;
};

// Accordion
void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LCTL:
    case MOD_RCTL:
      return true;  // Eagerly apply Shift and Ctrl mods.

    default:
      return false;
  }
}

// Tap Dance
void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

// ! Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Space, twice for Backspace
    [TD_SPC_ENT] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
	// Tap once for Control+Backspace, twice for Backspace
    [TD_CTRLBKSP_BKSP] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_BSPC), KC_BSPC),
// Tap hold for UK pound sign on US dollar sign hold
    [TD_DOLLAR_POUND] = ACTION_TAP_DANCE_TAP_HOLD(KC_DLR, SS_POUND),
	// Tap once for Dollar, twice for Pound sign
    [TD_TAP_DOLLAR_POUND] = ACTION_TAP_DANCE_DOUBLE(KC_DLR, SS_POUND),

};



// ! ======================================================================================
		// HOME_A, HOME_R, HOME_S, HOME_T, KC_G, KC_M, HOME_N, HOME_E, HOME_I, HOME_O, 
			   								//    MO(1), TD(TD_SPC_ENT), 			TD(TD_CTRLBKSP_BKSP), MO(2)),
// ! ======================================================================================


//* Layout
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	//Base (Colemak-DH)
	[0] = LAYOUT_split_3x5_2(
		SS_QU		, KC_W		  , KC_F		, KC_P		  , KC_B, 			KC_J, KC_L		  , KC_U   		, KC_Y  		, KC_QUOT	  , 
		LGUI_T(KC_A), LALT_T(KC_R), LSFT_T(KC_S), LCTL_T(KC_T), KC_G, 			KC_M, RCTL_T(KC_N), RSFT_T(KC_E), LALT_T(KC_I)  , RGUI_T(KC_O), 
		KC_Z		, KC_X		  , KC_C		, KC_D		  , KC_V, 			KC_K, KC_H		  , KC_COMM		, KC_DOT		, KC_SLSH	  , 
			   								   	MO(2), LT(4, KC_SPC), 			LCTL(KC_BSPC), MO(3)),

	//Base (Recurva)
	[1] = LAYOUT_split_3x5_2(
		KC_F		, KC_R		  , KC_D		, KC_P		  , KC_V, 			SS_QU , KC_M		, KC_U   	  , KC_O  		, KC_Y		  , 
		LGUI_T(KC_S), LALT_T(KC_N), LSFT_T(KC_T), LCTL_T(KC_C), KC_B, 			KC_DOT, RCTL_T(KC_H), RSFT_T(KC_E), LALT_T(KC_A), RGUI_T(KC_I), 
		KC_Z		, KC_X		  , KC_K		, KC_G		  , KC_W, 			KC_J  , KC_L		, KC_SLSH	  , KC_QUOT		, KC_COMM	  , 
			   								   	MO(2), LT(4, KC_SPC), 			LCTL(KC_BSPC), MO(3)),
	
	//Nav/Ext
	[2] = LAYOUT_split_3x5_2(
		KC_ESC    , KC_TAB     , CW_TOGG   , KC_CAPS   , KC_NO   , 			KC_PGUP , KC_HOME, KC_UP  , KC_END , KC_NO , 
		KC_LGUI   , KC_LALT   , KC_LSFT   , KC_LCTL   , KC_NO   , 			KC_PGDN , KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,
		LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_F), 			QK_REP  , KC_TAB , KC_NO  , KC_NO  , KC_NO , 
												   	KC_TRNS, KC_NO, 			KC_BSPC, MO(5)),
	
	//Symbol
	[3] = LAYOUT_split_3x5_2(
		KC_EXLM, KC_AT, KC_HASH, KC_DLR , KC_PERC,			 KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_SCLN, 
		KC_GRV , KC_LT, KC_LCBR, KC_LPRN, KC_LBRC,			 KC_COLN, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, 
		KC_TILD, KC_GT, KC_RCBR, KC_RPRN, KC_RBRC, 		 	 KC_PIPE, KC_MINS, KC_EQL , KC_PLUS, KC_BSLS, 
									MO(5), KC_ENT, 		 	 KC_NO, KC_TRNS),
	
	//Numpad
	[4] = LAYOUT_split_3x5_2(
		KC_NO  , KC_NO  , KC_BSPC, KC_DEL , KC_NO, 			KC_NO, KC_7, KC_8, KC_9, KC_NO, 
		KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, 			KC_NO, KC_4, KC_5, KC_6, KC_0 , 
		KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO,			KC_NO, KC_1, KC_2, KC_3, KC_NO,
								 KC_TRNS, KC_TRNS, 			KC_NO, KC_TRNS),
	
	//Function
	[5] = LAYOUT_split_3x5_2(
		TO(0)  , KC_NO  , QK_BOOT, KC_NO  , TG(1), 			KC_NO , KC_F7, KC_F8, KC_F9, KC_NO , 
		KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, 			KC_F12, KC_F4, KC_F5, KC_F6, KC_F10, 
		KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO,			KC_F11, KC_F1, KC_F2, KC_F3, KC_NO , 
							   	   KC_TRNS, KC_NO, 			KC_NO, KC_TRNS),
};

//* Combos
const uint16_t PROGMEM combo1[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo2[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo3[] = {KC_D, KC_V, KC_K, KC_H, COMBO_END};
const uint16_t PROGMEM combo19[] = {SS_QU, KC_U, COMBO_END};
// const uint16_t PROGMEM combo3[] = {KC_Q, KC_A, KC_Z, COMBO_END};
// const uint16_t PROGMEM combo4[] = {KC_Q, KC_A, COMBO_END};
// const uint16_t PROGMEM combo5[] = {KC_W, KC_R, COMBO_END};
// const uint16_t PROGMEM combo6[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM combo7[] = {KC_P, LCTL_T(KC_T), COMBO_END};
// const uint16_t PROGMEM combo8[] = {KC_B, KC_G, COMBO_END};
const uint16_t PROGMEM combo17[] = {KC_DLR, KC_LPRN, COMBO_END};
const uint16_t PROGMEM combo18[] = {KC_H, KC_COMM, COMBO_END};

const uint16_t PROGMEM combo9[] = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM combo10[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo16[] = {KC_J, KC_L, COMBO_END};
// const uint16_t PROGMEM combo11[] = {KC_J, KC_M, COMBO_END};
// const uint16_t PROGMEM combo12[] = {KC_L, KC_N, COMBO_END};
// const uint16_t PROGMEM combo13[] = {KC_U, KC_E, COMBO_END};
// const uint16_t PROGMEM combo14[] = {KC_Y, KC_I, COMBO_END};
// const uint16_t PROGMEM combo15[] = {KC_QUOT, KC_O, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo1, KC_ESC),
    COMBO(combo2, KC_TAB), // keycodes with modifiers are possible too!
	COMBO(combo3, QK_BOOT),
	// COMBO(combo4, KC_EXLM),
	// COMBO(combo5, KC_AT),
	// COMBO(combo6, KC_HASH),
	COMBO(combo7, KC_DLR),
	// COMBO(combo8, KC_PERC),
	COMBO(combo17, SS_POUND),
	COMBO(combo18, KC_BSPC),
	COMBO(combo19, KC_Q),

	COMBO(combo9, KC_BSPC),
	COMBO(combo10, KC_ENT),
	COMBO(combo16, CW_TOGG),
	// COMBO(combo11, KC_CIRC),
	// COMBO(combo12, KC_AMPR),
	// COMBO(combo13, KC_ASTR),
	// COMBO(combo14, KC_UNDS),
	// COMBO(combo15, KC_SCLN),
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)



