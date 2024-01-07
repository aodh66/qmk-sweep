#include QMK_KEYBOARD_H
#include "features/achordion.h"

//* Home row mods (Recurva)
// Left-hand home row mods
#define HOME_S LGUI_T(KC_S)
#define HOME_N LALT_T(KC_N)
#define HOME_T LSFT_T(KC_T)
#define HOME_C LCTL_T(KC_C)

// Right-hand home row mods
#define HOME_H RCTL_T(KC_H)
#define HOME_E RSFT_T(KC_E)
#define HOME_A LALT_T(KC_A)
#define HOME_I RGUI_T(KC_I)

//* Macro Declarations
enum custom_keycodes {
    // SS_POUND = SAFE_RANGE, //! requires US International, which breaks grave
    // EMAIL = SAFE_RANGE,
    SS_QU = SAFE_RANGE,
};

// ! CURRENTLY UNUSED
// ! ======================================================================================
// // * Tap Dance declarations
// enum {
//     TD_SPC_ENT,
// 	TD_CTRLBKSP_BKSP,
// 	TD_DOLLAR_POUND,
// 	TD_TAP_DOLLAR_POUND,
// };

// typedef struct {
//     uint16_t tap;
//     uint16_t hold;
//     uint16_t held;
// } tap_dance_tap_hold_t;
// ! ======================================================================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Accordion
    if (!process_achordion(keycode, record)) {
        return false;
    }

    // ! CURRENTLY UNUSED
    // ! ======================================================================================
    // // Tap Dance
    // tap_dance_action_t *action;
    // ! ======================================================================================

    switch (keycode) {
    // ! CURRENTLY UNUSED
    // ! ======================================================================================
            // // Tap Dance
            //     case TD(TD_DOLLAR_POUND):  // list all tap dance keycodes with tap-hold configurations
            //         action = &tap_dance_actions[TD_INDEX(keycode)];
            //         if (!record->event.pressed && action->state.count && !action->state.finished) {
            //             tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
            //             tap_code16(tap_hold->tap);
            //         }

            // Macros
            // case SS_POUND:
            //     if (record->event.pressed) {
            //         SEND_STRING(SS_LSFT(SS_RALT("4"))); //! requires US International, which breaks grave
            //     }
            //     break;

            // case EMAIL:
            // if (record->event.pressed) {
            //     SEND_STRING("aodhan66@gmail.com");
            // }
            // break;
            // ! ======================================================================================

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
            return true; // Eagerly apply Shift and Ctrl mods.

        default:
            return false;
    }
}

// ! CURRENTLY UNUSED
// ! ======================================================================================
// // Tap Dance
// void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
//     tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

//     if (state->pressed) {
//         if (state->count == 1
// #ifndef PERMISSIVE_HOLD
//             && !state->interrupted
// #endif
//         ) {
//             register_code16(tap_hold->hold);
//             tap_hold->held = tap_hold->hold;
//         } else {
//             register_code16(tap_hold->tap);
//             tap_hold->held = tap_hold->tap;
//         }
//     }
// }

// void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
//     tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

//     if (tap_hold->held) {
//         unregister_code16(tap_hold->held);
//         tap_hold->held = 0;
//     }
// }

// #define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \ { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

// // ! Tap Dance definitions
// tap_dance_action_t tap_dance_actions[] = {
//     // Tap once for Space, twice for Backspace
//     [TD_SPC_ENT] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
// 	// Tap once for Control+Backspace, twice for Backspace
//     [TD_CTRLBKSP_BKSP] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_BSPC), KC_BSPC),
// // Tap hold for UK pound sign on US dollar sign hold
//     [TD_DOLLAR_POUND] = ACTION_TAP_DANCE_TAP_HOLD(KC_DLR, SS_POUND),
// 	// Tap once for Dollar, twice for Pound sign
//     [TD_TAP_DOLLAR_POUND] = ACTION_TAP_DANCE_DOUBLE(KC_DLR, SS_POUND),

// };
// ! ======================================================================================

//* Layout
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	//Base (Recurva)
	[0] = LAYOUT_split_3x5_2(
		KC_F		, KC_R		  , KC_D		, KC_P		  , KC_V	  	 , 			SS_QU  		 , KC_M		   , KC_U   	 , KC_O  	   , KC_Y		 , 
		HOME_S      , HOME_N      , HOME_T      , HOME_C      , KC_B	  	 , 			KC_DOT 		 , HOME_H      , HOME_E      , HOME_A      , HOME_I      , 
		KC_Z		, KC_X		  , KC_K		, KC_G		  , KC_W	  	 , 			KC_J   		 , KC_L		   , KC_SLSH	 , KC_QUOT	   , KC_COMM	 , 
			   								   	  MO(1)		  , LT(3, KC_SPC), 			LCTL(KC_BSPC), MO(2))	   ,

	//Nav/Ext
	[1] = LAYOUT_split_3x5_2(
		KC_ESC    	, KC_TAB	  , CW_TOGG     , KC_CAPS     , KC_NO     	 , 			KC_PGUP		 , KC_HOME	   , KC_UP  	 , KC_END 	   , KC_NO 	   	 , 
		KC_LGUI   	, KC_LALT  	  , KC_LSFT     , KC_LCTL     , KC_NO     	 , 			KC_PGDN		 , KC_LEFT	   , KC_DOWN	 , KC_RGHT	   , KC_DEL	   	 ,
		LCTL(KC_Z)	, LCTL(KC_X)  , LCTL(KC_C)  , LCTL(KC_V)  , LCTL(KC_F)	 , 			QK_REP		 , KC_TAB 	   , KC_NO  	 , KC_NO  	   , KC_NO 	   	 , 
												  KC_TRNS	  , KC_NO	 	 , 			KC_BSPC		 , MO(4))	   ,
	
	//Symbol
	[2] = LAYOUT_split_3x5_2(
		KC_EXLM		, KC_AT		  , KC_HASH		, KC_DLR 	  , KC_PERC	  	 ,			KC_CIRC		 , KC_AMPR	   , KC_ASTR	 , KC_UNDS	   , KC_PIPE	 , 
		KC_GRV 		, KC_LT		  , KC_LCBR		, KC_LPRN	  , KC_LBRC	  	 ,			KC_COLN		 , KC_RCTL	   , KC_RSFT	 , KC_RALT	   , KC_RGUI	 , 
		KC_TILD		, KC_GT		  , KC_RCBR		, KC_RPRN	  , KC_RBRC	  	 , 		 	KC_SCLN		 , KC_MINS	   , KC_EQL 	 , KC_PLUS	   , KC_BSLS	 , 
												  MO(4)		  , KC_ENT		 , 		 	KC_NO		 , KC_TRNS)	   ,
	
	//Numpad
	[3] = LAYOUT_split_3x5_2(
		KC_NO  		, KC_NO  	  , KC_BSPC		, KC_DEL 	  , KC_NO	  	 , 			KC_NO  		 , KC_7		   , KC_8		 , KC_9		   , KC_NO	   	 , 
		KC_LGUI		, KC_LALT	  , KC_LSFT		, KC_LCTL	  , KC_NO	  	 , 			KC_NO  		 , KC_4		   , KC_5		 , KC_6		   , KC_0 	   	 , 
		KC_NO  		, KC_NO  	  , KC_NO  		, KC_NO  	  , KC_NO	  	 ,			KC_NO  		 , KC_1		   , KC_2		 , KC_3		   , KC_NO	   	 ,
								 				  KC_NO       , KC_TRNS	  	 , 			KC_NO  		 , KC_NO)	   ,
	
	//Function
	[4] = LAYOUT_split_3x5_2(
		KC_NO  		, KC_NO  	  , QK_BOOT		, KC_NO  	  , KC_VOLU	  	 , 			KC_NO  		 , KC_F7	   , KC_F8	   	 , KC_F9	   , KC_NO 	   	 , 
		KC_LGUI		, KC_LALT	  , KC_LSFT		, KC_LCTL	  , KC_VOLD	  	 , 			KC_F12 		 , KC_F4  	   , KC_F5	   	 , KC_F6	   , KC_F10	   	 , 
		KC_NO  		, KC_NO  	  , KC_NO  		, KC_NO  	  , KC_NO	  	 ,			KC_F11 		 , KC_F1	   , KC_F2	   	 , KC_F3	   , KC_NO 	   	 , 
							   	   				  KC_TRNS	  , KC_NO		 , 			KC_NO		 , KC_TRNS)	   ,
};

//* Combos (Currently set up for Colemak-DH)
// Left Hand
const uint16_t PROGMEM combo1[] = {SS_QU, KC_U, COMBO_END}; // Q+U -> Q

const uint16_t PROGMEM combo2[]  = {KC_R, KC_D, COMBO_END}; // R+D -> ESC
const uint16_t PROGMEM combo3[]  = {KC_X, KC_K, COMBO_END}; // X+K -> Tab

const uint16_t PROGMEM combo4[] = {KC_F, HOME_S, COMBO_END}; // F+S -> Grave `
const uint16_t PROGMEM combo5[] = {KC_R, HOME_N, COMBO_END}; // R+N -> Less than <
const uint16_t PROGMEM combo6[] = {KC_D, HOME_T, COMBO_END}; // D+T -> Left Curly Brace {
const uint16_t PROGMEM combo7[] = {KC_P, HOME_C, COMBO_END}; // P+C -> Left Parenthesis (
const uint16_t PROGMEM combo8[] = {KC_V, KC_B, COMBO_END}; // V+B -> Left Brace [


// Right Hand
const uint16_t PROGMEM combo9[]  = {KC_G, LCTL(KC_BSPC), COMBO_END}; // Ctrl+Bkspc Key -> Backspace
const uint16_t PROGMEM combo10[] = {KC_SLSH, KC_QUOT, COMBO_END}; // /+' -> Enter
const uint16_t PROGMEM combo11[] = {KC_B, KC_DOT, COMBO_END}; // B+. -> Caps Word

const uint16_t PROGMEM combo12[] = {KC_L, KC_SLSH, COMBO_END}; // L+/ -> Backspace Bot
const uint16_t PROGMEM combo13[] = {KC_QUOT, KC_COMM, COMBO_END}; // '+, -> Delete Bot

const uint16_t PROGMEM combo14[] = {SS_QU, KC_DOT, COMBO_END}; // Q+. -> Right Brace ]
const uint16_t PROGMEM combo15[] = {KC_M, HOME_H, COMBO_END}; // M+H -> Right Parenthesis )
const uint16_t PROGMEM combo16[] = {KC_U, HOME_E, COMBO_END}; // U+E -> Right Curly Brace }
const uint16_t PROGMEM combo17[] = {KC_O, HOME_A, COMBO_END}; // O+A -> Greater than >
const uint16_t PROGMEM combo18[] = {KC_Y, HOME_I, COMBO_END}; // Y+I -> Semicolon ;

// ! CURRENTLY UNUSED
// ! ======================================================================================
// const uint16_t PROGMEM combo17[] = {KC_DLR, KC_LPRN, COMBO_END}; //! requires US International, which breaks grave
// ! ======================================================================================

combo_t key_combos[COMBO_COUNT] = {
    // Left Hand
    COMBO(combo1, KC_Q), // Q+U -> Q

    COMBO(combo2, KC_ESC), // R+D -> ESC
    COMBO(combo3, KC_TAB), // X+K -> Tab

    COMBO(combo4, KC_GRV), // F+S -> Grave `
    COMBO(combo5, KC_LT), // R+N -> Less than <
    COMBO(combo6, KC_LCBR), // D+T -> Left Curly Brace {
    COMBO(combo7, KC_LPRN), // P+C -> Left Parenthesis (
    COMBO(combo8, KC_LBRC), // V+B -> Left Brace [


    // Right Hand
    COMBO(combo9, KC_BSPC), // Ctrl+Bkspc Key -> Backspace
	COMBO(combo10, KC_ENT), // /+' -> Enter
	COMBO(combo11, CW_TOGG), // B+. -> Caps Word

    COMBO(combo12, KC_BSPC), // L+/ -> Backspace Bot
	COMBO(combo13, KC_DEL), // '+, -> Delete Bot

    COMBO(combo14, KC_RBRC), // Q+. -> Right Brace ]
    COMBO(combo15, KC_RPRN), // M+H -> Right Parenthesis )
    COMBO(combo16, KC_RCBR), // U+E -> Right Curly Brace }
    COMBO(combo17, KC_GT), // O+A -> Greater than >
    COMBO(combo18, KC_SCLN), // Y+I -> Semicolon ;

    // ! CURRENTLY UNUSED
    // ! ======================================================================================
    // COMBO(combo17, SS_POUND), //! requires US International, which breaks grave
    // ! ======================================================================================
};
