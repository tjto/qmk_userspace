#include QMK_KEYBOARD_H
#include "features/achordion.h"

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) {
    return false;
  }
  // Your macros ...

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


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,      KC_I,    KC_O,    KC_P,     KC_BSPC,
        KC_LCTL, HRM_A,   HRM_S,  HRM_D,   HRM_F,    KC_G,                               KC_H,   HRM_J,     HRM_K,   HRM_L, HRM_SCLN,    KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                               KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_RSFT,
                                            KC_LGUI, KC_BSPC, KC_SPC,           KC_SPC,  KC_ENT,  KC_RALT
    )
};

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case HRM_F:
    case HRM_J:
      return TAPPING_TERM - 45;
    default:
      return TAPPING_TERM;
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HRM_H:
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
  // Also allow same-hand holds when the other key is in the rows outside the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboards are split.
  const uint8_t row = other_record->event.key.row % (MATRIX_ROWS / 2);
  if (!(1 <= row && row <= 3)) { return true; }


  return achordion_opposite_hands(tap_hold_record, other_record);
}
