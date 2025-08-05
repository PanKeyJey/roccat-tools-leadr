#ifndef __ROCCAT_leadr_PROFILE_BUTTONS_H__
#define __ROCCAT_leadr_PROFILE_BUTTONS_H__

/*
 * This file is part of roccat-tools.
 *
 * roccat-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * roccat-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with roccat-tools. If not, see <http://www.gnu.org/licenses/>.
 */

#include "roccat_device.h"
#include "roccat_button.h"

G_BEGIN_DECLS

typedef struct _leadrProfileButtons leadrProfileButtons;

enum {
	leadr_PHYSICAL_BUTTON_NUM = 16,
	leadr_PROFILE_BUTTON_NUM = 32,
};

typedef enum {
	leadr_BUTTON_INDEX_LEFT = 0,
	leadr_BUTTON_INDEX_RIGHT,
	leadr_BUTTON_INDEX_MIDDLE,
	leadr_BUTTON_INDEX_THUMB_BACK,
	leadr_BUTTON_INDEX_THUMB_FORWARD,
	leadr_BUTTON_INDEX_THUMB_PEDAL,
	leadr_BUTTON_INDEX_THUMB_PADDLE_UP,
	leadr_BUTTON_INDEX_THUMB_PADDLE_DOWN,
	leadr_BUTTON_INDEX_LEFT_BACK,
	leadr_BUTTON_INDEX_LEFT_FORWARD,
	leadr_BUTTON_INDEX_RIGHT_BACK,
	leadr_BUTTON_INDEX_RIGHT_FORWARD,
	leadr_BUTTON_INDEX_FIN_RIGHT,
	leadr_BUTTON_INDEX_FIN_LEFT,
	leadr_BUTTON_INDEX_WHEEL_UP,
	leadr_BUTTON_INDEX_WHEEL_DOWN,
	leadr_BUTTON_INDEX_SHIFT_LEFT,
	leadr_BUTTON_INDEX_SHIFT_RIGHT,
	leadr_BUTTON_INDEX_SHIFT_MIDDLE,
	leadr_BUTTON_INDEX_SHIFT_THUMB_BACK,
	leadr_BUTTON_INDEX_SHIFT_THUMB_FORWARD,
	leadr_BUTTON_INDEX_SHIFT_THUMB_PEDAL,
	leadr_BUTTON_INDEX_SHIFT_THUMB_PADDLE_UP,
	leadr_BUTTON_INDEX_SHIFT_THUMB_PADDLE_DOWN,
	leadr_BUTTON_INDEX_SHIFT_LEFT_BACK,
	leadr_BUTTON_INDEX_SHIFT_LEFT_FORWARD,
	leadr_BUTTON_INDEX_SHIFT_RIGHT_BACK,
	leadr_BUTTON_INDEX_SHIFT_RIGHT_FORWARD,
	leadr_BUTTON_INDEX_SHIFT_FIN_RIGHT,
	leadr_BUTTON_INDEX_SHIFT_FIN_LEFT,
	leadr_BUTTON_INDEX_SHIFT_WHEEL_UP,
	leadr_BUTTON_INDEX_SHIFT_WHEEL_DOWN,
} leadrButtonIndex;

typedef enum {
	leadr_BUTTON_TYPE_UNUSED = 0x00,
	leadr_BUTTON_TYPE_CLICK = 0x01,
	leadr_BUTTON_TYPE_MENU = 0x02,
	leadr_BUTTON_TYPE_UNIVERSAL_SCROLLING = 0x03,
	leadr_BUTTON_TYPE_DOUBLE_CLICK = 0x04,
	leadr_BUTTON_TYPE_SHORTCUT = 0x05,
	leadr_BUTTON_TYPE_DISABLED = 0x06,
	leadr_BUTTON_TYPE_BROWSER_FORWARD = 0x07,
	leadr_BUTTON_TYPE_BROWSER_BACKWARD = 0x08,
	leadr_BUTTON_TYPE_TILT_LEFT = 0x09,
	leadr_BUTTON_TYPE_TILT_RIGHT = 0x0a,
	leadr_BUTTON_TYPE_SCROLL_UP = 0x0d,
	leadr_BUTTON_TYPE_SCROLL_DOWN = 0x0e,
	leadr_BUTTON_TYPE_QUICKLAUNCH = 0x0f,
	leadr_BUTTON_TYPE_PROFILE_CYCLE = 0x10,
	leadr_BUTTON_TYPE_PROFILE_UP = 0x11,
	leadr_BUTTON_TYPE_PROFILE_DOWN = 0x12,
	leadr_BUTTON_TYPE_CPI_CYCLE = 0x14,
	leadr_BUTTON_TYPE_CPI_UP = 0x15,
	leadr_BUTTON_TYPE_CPI_DOWN = 0x16,
	leadr_BUTTON_TYPE_SENSITIVITY_CYCLE = 0x17,
	leadr_BUTTON_TYPE_SENSITIVITY_UP = 0x18,
	leadr_BUTTON_TYPE_SENSITIVITY_DOWN = 0x19,
	leadr_BUTTON_TYPE_WINDOWS_KEY = 0x1a,
	leadr_BUTTON_TYPE_OPEN_DRIVER = 0x1b,
	leadr_BUTTON_TYPE_OPEN_PLAYER = 0x20,
	leadr_BUTTON_TYPE_PREV_TRACK = 0x21,
	leadr_BUTTON_TYPE_NEXT_TRACK = 0x22,
	leadr_BUTTON_TYPE_PLAY_PAUSE = 0x23,
	leadr_BUTTON_TYPE_STOP = 0x24,
        leadr_BUTTON_TYPE_MUTE = 0x25,
        leadr_BUTTON_TYPE_VOLUME_UP = 0x26,
        leadr_BUTTON_TYPE_VOLUME_DOWN = 0x27,
        leadr_BUTTON_TYPE_MACRO = 0x30,
	leadr_BUTTON_TYPE_TIMER = 0x31,
	leadr_BUTTON_TYPE_TIMER_STOP = 0x32,
	leadr_BUTTON_TYPE_EASYAIM_1 = 0x33,
	leadr_BUTTON_TYPE_EASYAIM_2 = 0x34,
	leadr_BUTTON_TYPE_EASYAIM_3 = 0x35,
	leadr_BUTTON_TYPE_EASYAIM_4 = 0x36,
	leadr_BUTTON_TYPE_EASYAIM_5 = 0x37,
	leadr_BUTTON_TYPE_EASYSHIFT_SELF = 0x41, // FIXME confirm, firmware suggests it's 0x54
	leadr_BUTTON_TYPE_EASYWHEEL_SENSITIVITY = 0x42,
	leadr_BUTTON_TYPE_EASYWHEEL_PROFILE = 0x43,
	leadr_BUTTON_TYPE_EASYWHEEL_CPI = 0x44,
	leadr_BUTTON_TYPE_EASYWHEEL_VOLUME = 0x45,
	leadr_BUTTON_TYPE_EASYWHEEL_ALT_TAB = 0x46,
	leadr_BUTTON_TYPE_EASYWHEEL_AERO_FLIP_3D = 0x47,
	leadr_BUTTON_TYPE_EASYSHIFT_OTHER = 0x51,
	leadr_BUTTON_TYPE_EASYSHIFT_LOCK_OTHER = 0x52,
	leadr_BUTTON_TYPE_EASYSHIFT_ALL = 0x53,
	leadr_BUTTON_TYPE_XINPUT_1 = 0x60,
	leadr_BUTTON_TYPE_XINPUT_2 = 0x61,
	leadr_BUTTON_TYPE_XINPUT_3 = 0x62,
	leadr_BUTTON_TYPE_XINPUT_4 = 0x63,
	leadr_BUTTON_TYPE_XINPUT_5 = 0x64,
	leadr_BUTTON_TYPE_XINPUT_6 = 0x65,
	leadr_BUTTON_TYPE_XINPUT_7 = 0x66,
	leadr_BUTTON_TYPE_XINPUT_8 = 0x67,
	leadr_BUTTON_TYPE_XINPUT_9 = 0x68,
	leadr_BUTTON_TYPE_XINPUT_10 = 0x69,
	leadr_BUTTON_TYPE_XINPUT_RX_UP = 0x6a,
	leadr_BUTTON_TYPE_XINPUT_RX_DOWN = 0x6b,
	leadr_BUTTON_TYPE_XINPUT_RY_UP = 0x6c,
	leadr_BUTTON_TYPE_XINPUT_RY_DOWN = 0x6d,
	leadr_BUTTON_TYPE_XINPUT_X_UP = 0x6e,
	leadr_BUTTON_TYPE_XINPUT_X_DOWN = 0x6f,
	leadr_BUTTON_TYPE_XINPUT_Y_UP = 0x70,
	leadr_BUTTON_TYPE_XINPUT_Y_DOWN = 0x71,
	leadr_BUTTON_TYPE_XINPUT_Z_UP = 0x72,
	leadr_BUTTON_TYPE_XINPUT_Z_DOWN = 0x73,
	leadr_BUTTON_TYPE_DINPUT_1 = 0x74,
	leadr_BUTTON_TYPE_DINPUT_2 = 0x75,
	leadr_BUTTON_TYPE_DINPUT_3 = 0x76,
	leadr_BUTTON_TYPE_DINPUT_4 = 0x77,
	leadr_BUTTON_TYPE_DINPUT_5 = 0x78,
	leadr_BUTTON_TYPE_DINPUT_6 = 0x79,
	leadr_BUTTON_TYPE_DINPUT_7 = 0x7a,
	leadr_BUTTON_TYPE_DINPUT_8 = 0x7b,
	leadr_BUTTON_TYPE_DINPUT_9 = 0x7c,
	leadr_BUTTON_TYPE_DINPUT_10 = 0x7d,
	leadr_BUTTON_TYPE_DINPUT_11 = 0x7e,
	leadr_BUTTON_TYPE_DINPUT_12 = 0x7f,
	leadr_BUTTON_TYPE_DINPUT_X_UP = 0x80,
	leadr_BUTTON_TYPE_DINPUT_X_DOWN = 0x81,
	leadr_BUTTON_TYPE_DINPUT_Y_UP = 0x82,
	leadr_BUTTON_TYPE_DINPUT_Y_DOWN = 0x83,
	leadr_BUTTON_TYPE_DINPUT_Z_UP = 0x84,
	leadr_BUTTON_TYPE_DINPUT_Z_DOWN = 0x85,
	leadr_BUTTON_TYPE_HOME = 0x86,
	leadr_BUTTON_TYPE_END = 0x87,
	leadr_BUTTON_TYPE_PAGE_UP = 0x88,
	leadr_BUTTON_TYPE_PAGE_DOWN = 0x89,
	leadr_BUTTON_TYPE_L_CTRL = 0x8a,
	leadr_BUTTON_TYPE_L_ALT = 0x8b,
} leadrButtonType;

struct _leadrProfileButtons {
	guint8 report_id; /* leadr_REPORT_ID_PROFILE_BUTTONS */
	guint8 size; /* always 99 */
	guint8 profile_index; /* range 0-4 */
	RoccatButton buttons[leadr_PROFILE_BUTTON_NUM];
} __attribute__ ((packed));

void leadr_profile_buttons_finalize(leadrProfileButtons *profile_buttons, guint profile_index);
gboolean leadr_profile_buttons_write(RoccatDevice *device, guint profile_index, leadrProfileButtons *profile_buttons, GError **error);
leadrProfileButtons *leadr_profile_buttons_read(RoccatDevice *device, guint profile_index, GError **error);
gboolean leadr_profile_buttons_equal(leadrProfileButtons const *left, leadrProfileButtons const *right);

G_END_DECLS

#endif
