#include "features/achordion.h"
#include "keymap_us_international_linux.h"
#include "sendstring_us_international.h"

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) {
    return false;
  }

  return true;
}

void housekeeping_task_user(void) {
  achordion_task();
}

// Short aliases for home row mods and other tap-hold keys.
#define HRM_F LSFT_T(KC_F)
#define HRM_D LALT_T(KC_D)
#define HRM_S LCTL_T(KC_S)
#define HRM_A LGUI_T(KC_A)

#define HRM_J RSFT_T(KC_J)
#define HRM_K RALT_T(KC_K)
#define HRM_L RCTL_T(KC_L)
#define HRM_SCLN RGUI_T(KC_SCLN)

enum unicode_names {
  U_AW_LOWER,
  U_AW_UPPER,
  U_AO_LOWER,
  U_AO_UPPER,
  U_OW_LOWER,
  U_OW_UPPER,
};

const uint32_t unicode_map[] PROGMEM = {
    [U_AO_LOWER] = 0x00e5,  // å
    [U_AO_UPPER] = 0x00c5,  // Å
    [U_AW_LOWER] = 0x00e4,  // ä
    [U_AW_UPPER] = 0x00c4,  // Ä
    [U_OW_LOWER] = 0x00f6,  // ö
    [U_OW_UPPER] = 0x00d6,  // Ö
};

#define U_AO UP(U_AO_LOWER, U_AO_UPPER)
#define U_AW UP(U_AW_LOWER, U_AW_UPPER)
#define U_OW UP(U_OW_LOWER, U_OW_UPPER)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       KC_MINUS,
        KC_LCTL, HRM_A,   HRM_S,  HRM_D,   HRM_F,    KC_G,                               KC_H,   HRM_J,    HRM_K,   HRM_L,   HRM_SCLN,   KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                               KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_EQL,
                                            TT(1), TT(1), KC_SPC,           KC_ENT,  TT(2),  TT(2)
    ),
    [1] = LAYOUT_split_3x6_3(
        KC_TRNS,  S(KC_1),  S(KC_2),  S(KC_3),  S(KC_4),     S(KC_5),                S(KC_6),     S(KC_7),     S(KC_8),  S(KC_9),  S(KC_0),  S(KC_GRV),
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_LBRC,  S(KC_LBRC),  KC_BSLS,                S(KC_BSLS),  S(KC_RBRC),  KC_RBRC,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_TRNS,                KC_TRNS,     KC_TRNS,     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                                            KC_TRNS, KC_TRNS ,  KC_TRNS,           KC_DEL,  KC_ESC,  KC_TRNS
    ),
    [2] = LAYOUT_split_3x6_3(
        KC_TRNS,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_GRV,
        KC_TRNS,  U_AW,     U_AO,     U_OW,     KC_TRNS,  KC_TRNS,                KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                                              KC_TRNS, KC_ESC, KC_BSPC,         KC_TRNS, KC_TRNS ,  KC_TRNS
    )
};

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
      return TAPPING_TERM;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HRM_J:
    case HRM_K:
    case HRM_L:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

// Callback for Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
bool get_chordal_hold(
        uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
        uint16_t other_keycode, keyrecord_t* other_record) {
  return get_chordal_hold_default(tap_hold_record, other_record);
}


///////////////////////////////////////////////////////////////////////////////
// Achordion (https://getreuer.info/posts/keyboards/achordion)
///////////////////////////////////////////////////////////////////////////////
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  return achordion_opposite_hands(tap_hold_record, other_record);
}

///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////
// const uint16_t escape_combo[] = {KC_E, KC_R, COMBO_END};
// const uint16_t bspc_r_combo[] = {KC_U, KC_I, COMBO_END};
// const uint16_t del_combo[]    = {KC_I, KC_O, COMBO_END};

// combo_t key_combos[] = {
//     COMBO(escape_combo, KC_ESC),
//     COMBO(bspc_r_combo, KC_BSPC),
//     COMBO(del_combo, KC_DEL),
// };

