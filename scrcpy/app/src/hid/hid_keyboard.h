#ifndef SC_HID_KEYBOARD_H
#define SC_HID_KEYBOARD_H

#include "common.h"

#include <stdbool.h>

#include "hid/hid_event.h"
#include "input_events.h"

// See "SDL2/SDL_scancode.h".
// Maybe SDL_Keycode is used by most people, but SDL_Scancode is taken from USB
// HID protocol.
// 0x65 is Application, typically AT-101 Keyboard ends here.
#define SC_HID_KEYBOARD_KEYS 0x66

extern const uint8_t SC_HID_KEYBOARD_REPORT_DESC[];
extern const size_t SC_HID_KEYBOARD_REPORT_DESC_LEN;

/**
 * HID keyboard events are sequence-based, every time keyboard state changes
 * it sends an array of currently pressed keys, the host is responsible for
 * compare events and determine which key becomes pressed and which key becomes
 * released. In order to convert SDL_KeyboardEvent to HID events, we first use
 * an array of keys to save each keys' state. And when a SDL_KeyboardEvent was
 * emitted, we updated our state, and then we use a loop to generate HID
 * events. The sequence of array elements is unimportant and when too much keys
 * pressed at the same time (more than report count), we should generate
 * phantom state. Don't forget that modifiers should be updated too, even for
 * phantom state.
 */
struct sc_hid_keyboard {
    bool keys[SC_HID_KEYBOARD_KEYS];
};

void
sc_hid_keyboard_init(struct sc_hid_keyboard *hid);

bool
sc_hid_keyboard_event_from_key(struct sc_hid_keyboard *hid,
                               struct sc_hid_event *hid_event,
                               const struct sc_key_event *event);

bool
sc_hid_keyboard_event_from_mods(struct sc_hid_event *event,
                                uint16_t mods_state);

#endif
