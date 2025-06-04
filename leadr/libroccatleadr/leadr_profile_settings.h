#ifndef __ROCCAT_leadr_PROFILE_SETTINGS_H__
#define __ROCCAT_leadr_PROFILE_SETTINGS_H__

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
#include "roccat_helper.h"
#include "leadr.h"

G_BEGIN_DECLS

enum {
	leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM = 5,
};

typedef struct _leadrProfileSettings leadrProfileSettings;
typedef struct _leadrLight leadrLight;

struct _leadrLight {
	guint8 index;
	guint8 red;
	guint8 green;
	guint8 blue;
	guint8 unused;
} __attribute__ ((packed));

struct _leadrProfileSettings {
	guint8 report_id; /* leadr_REPORT_ID_PROFILE_SETTINGS */
	guint8 size; /* always 30 */
	guint8 profile_index; /* range 0-4 */
	guint8 advanced_sensitivity; /* RoccatSensitivityAdvanced */
	guint8 sensitivity_x; /* RoccatSensitivity */
	guint8 sensitivity_y; /* RoccatSensitivity */
	guint8 cpi_levels_enabled;
	guint8 cpi_levels[leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM];
	guint8 cpi_active; /* range 0-4 */
	guint8 talkfx_polling_rate; /* leadrProfileSettingsTalkfx + RoccatPollingRate */
	guint8 lights_enabled;
	guint8 color_flow;
	guint8 light_effect;
	guint8 effect_speed;
	leadrLight lights[leadr_LIGHTS_NUM];
	guint16 checksum;
} __attribute__ ((packed));

static inline guint8 leadr_profile_settings_get_polling_rate(leadrProfileSettings const *profile_settings) {
	return roccat_get_nibble8(profile_settings->talkfx_polling_rate, ROCCAT_NIBBLE_LOW);
}

static inline guint8 leadr_profile_settings_get_talkfx(leadrProfileSettings const *profile_settings) {
	return roccat_get_nibble8(profile_settings->talkfx_polling_rate, ROCCAT_NIBBLE_HIGH);
}

static inline void leadr_profile_settings_set_polling_rate(leadrProfileSettings *profile_settings, guint8 new_value) {
	roccat_set_nibble8(&profile_settings->talkfx_polling_rate, ROCCAT_NIBBLE_LOW, new_value);
}

static inline void leadr_profile_settings_set_talkfx(leadrProfileSettings *profile_settings, guint8 new_value) {
	roccat_set_nibble8(&profile_settings->talkfx_polling_rate, ROCCAT_NIBBLE_HIGH, new_value);
}

typedef enum {
	leadr_PROFILE_SETTINGS_TALKFX_ON = 0,
	leadr_PROFILE_SETTINGS_TALKFX_OFF = 1,
} leadrProfileSettingsTalkfx;

typedef enum {
	leadr_PROFILE_SETTINGS_LIGHTS_ENABLED_BIT_WHEEL = 0,
	leadr_PROFILE_SETTINGS_LIGHTS_ENABLED_BIT_BOTTOM = 1,
	leadr_PROFILE_SETTINGS_LIGHTS_ENABLED_BIT_CUSTOM_COLOR = 4,
} leadrProfileSettingsLightsEnabled;

typedef enum {
	leadr_PROFILE_SETTINGS_COLOR_FLOW_OFF = 0,
	leadr_PROFILE_SETTINGS_COLOR_FLOW_SIMULTANEOUSLY = 1,
	leadr_PROFILE_SETTINGS_COLOR_FLOW_UP = 2,
	leadr_PROFILE_SETTINGS_COLOR_FLOW_DOWN = 3,
} leadrProfileSettingsColorFlow;

typedef enum {
	leadr_PROFILE_SETTINGS_LIGHT_EFFECT_ALL_OFF = 0,
	leadr_PROFILE_SETTINGS_LIGHT_EFFECT_FULLY_LIGHTED = 1,
	leadr_PROFILE_SETTINGS_LIGHT_EFFECT_BLINKING = 2,
	leadr_PROFILE_SETTINGS_LIGHT_EFFECT_BREATHING = 3,
	leadr_PROFILE_SETTINGS_LIGHT_EFFECT_HEARTBEAT = 4,
} leadrProfileSettingsLightEffect;

typedef enum {
	leadr_PROFILE_SETTINGS_EFFECT_SPEED_MIN = 1,
	leadr_PROFILE_SETTINGS_EFFECT_SPEED_MAX = 3,
} leadrProfileSettingsEffectSpeed;

void leadr_profile_settings_finalize(leadrProfileSettings *profile_settings, guint profile_index);
gboolean leadr_profile_settings_write(RoccatDevice *device, guint profile_index, leadrProfileSettings *profile_settings, GError **error);
leadrProfileSettings *leadr_profile_settings_read(RoccatDevice *device, guint profile_index, GError **error);
gboolean leadr_profile_settings_equal(leadrProfileSettings const *left, leadrProfileSettings const *right);

G_END_DECLS

#endif
