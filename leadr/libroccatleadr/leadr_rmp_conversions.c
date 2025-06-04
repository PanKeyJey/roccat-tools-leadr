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

#include "leadr_device.h"
#include "leadr_macro.h"
#include "leadr_rmp.h"
#include "roccat.h"
#include "roccat_helper.h"
#include "i18n-lib.h"

guint leadr_rmp_cpi_to_bin(guint rmp_value) {
	return rmp_value << 2;
}

guint leadr_bin_cpi_to_rmp(guint bin_value) {
	return bin_value >> 2;
}

leadrProfileSettings *leadr_rmp_to_profile_settings(leadrRmp *rmp) {
	leadrProfileSettings *settings;
	guint i;
	leadrRmpLightInfo *rmp_light_info;
	guint custom_color;

	settings = g_malloc0(sizeof(leadrProfileSettings));
	settings->report_id = leadr_REPORT_ID_PROFILE_SETTINGS;
	settings->size = sizeof(leadrProfileSettings);

	settings->advanced_sensitivity = leadr_rmp_get_xy_synchronous(rmp);
	if (settings->advanced_sensitivity == ROCCAT_SENSITIVITY_ADVANCED_ON) {
		settings->sensitivity_x = leadr_rmp_get_sensitivity_x(rmp);
		settings->sensitivity_y = leadr_rmp_get_sensitivity_y(rmp);
	} else {
		settings->sensitivity_x = leadr_rmp_get_sensitivity(rmp);
		settings->sensitivity_y = settings->sensitivity_x;
	}

	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i)
		settings->cpi_levels[i] = leadr_rmp_cpi_to_bin(leadr_rmp_get_cpi_level_x(rmp, i));
	settings->cpi_levels_enabled = leadr_rmp_get_cpi_all(rmp);
	settings->cpi_active = leadr_rmp_get_cpi_x(rmp);

	leadr_profile_settings_set_polling_rate(settings, leadr_rmp_get_polling_rate(rmp));
	leadr_profile_settings_set_talkfx(settings, leadr_rmp_get_talkfx(rmp));

	custom_color = leadr_rmp_get_light_chose_type(rmp);
	roccat_set_bit8(&settings->lights_enabled, leadr_PROFILE_SETTINGS_LIGHTS_ENABLED_BIT_CUSTOM_COLOR,
			custom_color == leadr_RMP_LIGHT_CHOSE_TYPE_CUSTOM);
	for (i = 0; i < leadr_LIGHTS_NUM; ++i) {
		if (custom_color == leadr_RMP_LIGHT_CHOSE_TYPE_CUSTOM)
			rmp_light_info = leadr_rmp_get_custom_light_info(rmp, i);
		else
			rmp_light_info = leadr_rmp_get_rmp_light_info(rmp, i);
		leadr_rmp_light_info_to_light(rmp_light_info, &settings->lights[i]);
		roccat_set_bit8(&settings->lights_enabled, i, rmp_light_info->state == leadr_RMP_LIGHT_INFO_STATE_ON);
		g_free(rmp_light_info);
	}

	settings->color_flow = leadr_rmp_get_light_color_flow(rmp);
	settings->light_effect = leadr_rmp_get_light_effect_type(rmp);
	settings->effect_speed = leadr_rmp_get_light_effect_speed(rmp);

	return settings;
}

void leadr_rmp_update_with_profile_settings(leadrRmp *rmp, leadrProfileSettings const *profile_settings) {
	leadrRmpLightInfo rmp_light_info;
	guint i;
	guint custom_color;
	guint light_state;

	leadr_rmp_set_xy_synchronous(rmp, profile_settings->advanced_sensitivity);
	if (profile_settings->advanced_sensitivity == ROCCAT_SENSITIVITY_ADVANCED_ON) {
		leadr_rmp_set_sensitivity_x(rmp, profile_settings->sensitivity_x);
		leadr_rmp_set_sensitivity_y(rmp, profile_settings->sensitivity_y);
	} else {
		leadr_rmp_set_sensitivity(rmp, profile_settings->sensitivity_x);
	}

	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i)
		leadr_rmp_set_cpi_level_x(rmp, i, leadr_bin_cpi_to_rmp(profile_settings->cpi_levels[i]));
	leadr_rmp_set_cpi_all(rmp, profile_settings->cpi_levels_enabled);
	leadr_rmp_set_cpi_x(rmp, profile_settings->cpi_active);

	if (roccat_get_bit8(profile_settings->lights_enabled, leadr_PROFILE_SETTINGS_LIGHTS_ENABLED_BIT_CUSTOM_COLOR))
		custom_color = leadr_RMP_LIGHT_CHOSE_TYPE_CUSTOM;
	else
		custom_color = leadr_RMP_LIGHT_CHOSE_TYPE_PALETTE;
	leadr_rmp_set_light_chose_type(rmp, custom_color);
	for (i = 0; i < leadr_LIGHTS_NUM; ++i) {
		light_state = roccat_get_bit8(profile_settings->lights_enabled, i) ? leadr_RMP_LIGHT_INFO_STATE_ON : leadr_RMP_LIGHT_INFO_STATE_OFF;
		leadr_light_to_rmp_light_info(&profile_settings->lights[i], &rmp_light_info, light_state);
		if (custom_color == leadr_RMP_LIGHT_CHOSE_TYPE_CUSTOM)
			leadr_rmp_set_custom_light_info(rmp, i, &rmp_light_info);
		else
			leadr_rmp_set_rmp_light_info(rmp, i, &rmp_light_info);
	}

	leadr_rmp_set_light_effect_type(rmp, profile_settings->light_effect);
	leadr_rmp_set_light_effect_speed(rmp, profile_settings->effect_speed);
	leadr_rmp_set_light_color_flow(rmp, profile_settings->color_flow);

	leadr_rmp_set_polling_rate(rmp, leadr_profile_settings_get_polling_rate(profile_settings));
	leadr_rmp_set_talkfx(rmp, leadr_profile_settings_get_talkfx(profile_settings));
}

static void leadr_profile_buttons_set_button(leadrRmp *rmp, guint index,
		leadrProfileButtons *profile_buttons) {
	leadrRmpMacroKeyInfo *macro_key_info;
	RoccatButton *button;

	macro_key_info = leadr_rmp_get_macro_key_info(rmp, index);
	button = &profile_buttons->buttons[index];

	button->type = macro_key_info->type;
	switch (button->type) {
	case leadr_BUTTON_TYPE_SHORTCUT:
		button->key = macro_key_info->keystrokes[0].key;
		button->modifier = macro_key_info->keystrokes[0].action;
		break;
	default:
		button->modifier = 0;
		button->key = 0;
	}

	g_free(macro_key_info);
}

leadrProfileButtons *leadr_rmp_to_profile_buttons(leadrRmp *rmp) {
	leadrProfileButtons *buttons;
	guint i;

	buttons = g_malloc0(sizeof(leadrProfileButtons));
	buttons->report_id = leadr_REPORT_ID_PROFILE_BUTTONS;
	buttons->size = sizeof(leadrProfileButtons);
	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
		leadr_profile_buttons_set_button(rmp, i, buttons);
	}
	return buttons;
}

static void leadr_rmp_update_with_profile_button(leadrRmp *rmp,
		RoccatButton const *button,
		guint rmp_index) {
	leadrRmpMacroKeyInfo *macro_key_info;
	leadrRmpMacroKeyInfo *actual_key_info;

	switch (button->type) {
	case leadr_BUTTON_TYPE_MACRO: /* doing nothing, macro is updated extra */
		macro_key_info = NULL;
		break;
	case leadr_BUTTON_TYPE_QUICKLAUNCH:
	case leadr_BUTTON_TYPE_TIMER:
	case leadr_BUTTON_TYPE_EASYSHIFT_OTHER:
	case leadr_BUTTON_TYPE_EASYSHIFT_LOCK_OTHER:
		actual_key_info = leadr_rmp_get_macro_key_info(rmp, rmp_index);
		if (button->type != actual_key_info->type) {
			g_warning(_("Profile button %i differs from rmp with missing data, so rmp takes precedence"), rmp_index);
			rmp->modified_macros[rmp_index] = TRUE; /* Hardware should be updated */
		}
		leadr_rmp_macro_key_info_free(actual_key_info);
		macro_key_info = NULL;
		break;
	case leadr_BUTTON_TYPE_SHORTCUT:
		macro_key_info = leadr_rmp_macro_key_info_new_shortcut(button->key, button->modifier);
		break;
	default:
		macro_key_info = leadr_rmp_macro_key_info_new();
		macro_key_info->type = button->type;
		break;
	}

	if (macro_key_info)
		leadr_rmp_set_macro_key_info(rmp, rmp_index, macro_key_info);
}

void leadr_rmp_update_with_profile_buttons(leadrRmp *rmp, leadrProfileButtons const *profile_buttons) {
	guint i;

	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i)
		leadr_rmp_update_with_profile_button(rmp, &profile_buttons->buttons[i], i);
}

void leadr_rmp_update_with_macro(leadrRmp *rmp, guint key_index, leadrMacro const *macro) {
	leadrRmpMacroKeyInfo *key_info;
	if (!macro)
		return;
	key_info = leadr_macro_to_rmp_macro_key_info(macro);
	leadr_rmp_set_macro_key_info(rmp, key_index, key_info);
	g_free(key_info);
}
