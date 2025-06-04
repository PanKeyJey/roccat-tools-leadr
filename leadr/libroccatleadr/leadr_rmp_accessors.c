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

#include "leadr_rmp.h"
#include "leadr_profile_buttons.h"
#include "leadr_profile_settings.h"
#include "roccat_helper.h"
#include "g_roccat_helper.h"
#include "i18n-lib.h"
#include <string.h>

static gchar const * const leadr_rmp_group_name = "Setting";

static gchar const * const leadr_rmp_xy_sync_name = "XYSynchronous";
static gchar const * const leadr_rmp_sensitivity_x_name = "Sensitivity_X";
static gchar const * const leadr_rmp_sensitivity_y_name = "Sensitivity_Y";
static gchar const * const leadr_rmp_sensitivity_name = "Sensitivity";
static gchar const * const leadr_rmp_profile_name_name = "ProFileName";
static gchar const * const leadr_rmp_cpi_x_name = "Cpi_X";
static gchar const * const leadr_rmp_polling_rate_name = "PollingRate";
static gchar const * const leadr_rmp_light_color_flow_name = "LightColorFlow";
static gchar const * const leadr_rmp_light_effect_type_name = "LightEffectType";
static gchar const * const leadr_rmp_light_effect_speed_name = "LightEffectSpeed";
static gchar const * const leadr_rmp_light_chose_type_name = "LtChoseType";
static gchar const * const leadr_rmp_use_color_for_all_name = "UseColorForAll";
static gchar const * const leadr_rmp_talkfx_name = "HDDKeyboard";

static gchar *leadr_rmp_create_macro_key_info_name(guint index) {
	g_assert(index < leadr_PROFILE_BUTTON_NUM);
	return g_strdup_printf("MacroKeyInfo%u", index);
}

static gchar *leadr_rmp_create_light_info_name(guint index) {
	g_assert(index < leadr_LIGHTS_NUM);
	return g_strdup_printf("LightInfo%u", index);
}

static gchar *leadr_rmp_create_custom_light_info_name(guint index) {
	g_assert(index < leadr_LIGHTS_NUM);
	return g_strdup_printf("CustomLightInfo%u", index);
}

static gchar *leadr_rmp_create_cpi_name(guint bit) {
	g_assert(bit < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM);
	return g_strdup_printf("dpi%u", bit);
}

static gchar *leadr_rmp_create_cpi_level_x_name(guint bit) {
	g_assert(bit < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM);
	return g_strdup_printf("CpiLevel_X%u", bit);
}

static gchar *leadr_rmp_create_game_file_name(guint index) {
	return g_strdup_printf("GameFile%u", index);
}

static void leadr_rmp_macro_key_info_finalize(leadrRmpMacroKeyInfo *rmp_macro_key_info, guint index) {
	rmp_macro_key_info->button_index = index;
	leadr_rmp_macro_key_info_set_checksum(rmp_macro_key_info);
}

static void leadr_rmp_light_info_finalize(leadrRmpLightInfo *rmp_light_info) {
	leadr_rmp_light_info_set_checksum(rmp_light_info);
}

static guint leadr_rmp_get_value(leadrRmp *rmp, gchar const *key) {
	GError *error = NULL;
	guint result = g_key_file_get_integer(rmp->key_file, leadr_rmp_group_name, key, &error);
	if (error) {
		g_clear_error(&error);
		result = g_key_file_get_integer(rmp->default_key_file, leadr_rmp_group_name, key, &error);
	}
	return result;
}

static void leadr_rmp_set_value_settings(leadrRmp *rmp, gchar const *key, guint value) {
	if (leadr_rmp_get_value(rmp, key) != value) {
		g_key_file_set_integer(rmp->key_file, leadr_rmp_group_name, key, value);
		rmp->modified_settings = TRUE;
	}
}

static leadrRmpMacroKeyInfo *macro_key_info_without_default(GKeyFile *key_file, guint index) {
	gchar *key;
	GError *local_error = NULL;
	leadrRmpMacroKeyInfo *rmp_macro_key_info;

	key = leadr_rmp_create_macro_key_info_name(index);
	rmp_macro_key_info = roccat_key_file_get_binary(key_file, leadr_rmp_group_name, key, sizeof(leadrRmpMacroKeyInfo), &local_error);
	g_free(key);
	return rmp_macro_key_info;
}

leadrRmpMacroKeyInfo *leadr_rmp_get_macro_key_info(leadrRmp *rmp, guint index) {
	leadrRmpMacroKeyInfo *rmp_macro_key_info;

	rmp_macro_key_info = macro_key_info_without_default(rmp->key_file, index);
	if (rmp_macro_key_info)
		return rmp_macro_key_info;

	rmp_macro_key_info = macro_key_info_without_default(rmp->default_key_file, index);
	return rmp_macro_key_info;
}

void leadr_rmp_set_macro_key_info(leadrRmp *rmp, guint index, leadrRmpMacroKeyInfo *rmp_macro_key_info) {
	gchar *key;
	leadrRmpMacroKeyInfo *actual_key_info;

	actual_key_info = leadr_rmp_get_macro_key_info(rmp, index);
	if (!leadr_rmp_macro_key_info_equal(rmp_macro_key_info, actual_key_info)) {
		key = leadr_rmp_create_macro_key_info_name(index);
		leadr_rmp_macro_key_info_finalize(rmp_macro_key_info, index);
		roccat_key_file_set_binary(rmp->key_file, leadr_rmp_group_name, key, (gconstpointer)rmp_macro_key_info, sizeof(leadrRmpMacroKeyInfo));
		g_free(key);
		rmp->modified_macros[index] = TRUE;
	}
	leadr_rmp_macro_key_info_free(actual_key_info);
}

gchar *leadr_rmp_get_profile_name(leadrRmp *rmp) {
	GError *error = NULL;
	gchar *result;

	result = g_key_file_get_string(rmp->key_file, leadr_rmp_group_name, leadr_rmp_profile_name_name, &error);
	if (error) {
		g_clear_error(&error);
		result = g_key_file_get_string(rmp->default_key_file, leadr_rmp_group_name, leadr_rmp_profile_name_name, &error);
		if (error)
			g_error(_("Could not get default value for key '%s': %s"), leadr_rmp_profile_name_name, error->message);
	}
	return result;
}

void leadr_rmp_set_profile_name(leadrRmp *rmp, gchar const *string) {
	gchar *actual_string;
	actual_string = leadr_rmp_get_profile_name(rmp);
	if (strcmp(string, actual_string)) {
		g_key_file_set_string(rmp->key_file, leadr_rmp_group_name, leadr_rmp_profile_name_name, string);
		rmp->modified_rmp = TRUE;
	}
	g_free(actual_string);
}

void leadr_rmp_set_sensitivity_x(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_sensitivity_x_name, value);
}

guint leadr_rmp_get_sensitivity_x(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_sensitivity_x_name);
}

void leadr_rmp_set_sensitivity_y(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_sensitivity_y_name, value);
}

guint leadr_rmp_get_sensitivity_y(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_sensitivity_y_name);
}

void leadr_rmp_set_sensitivity(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_sensitivity_name, value);
}

guint leadr_rmp_get_sensitivity(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_sensitivity_name);
}

void leadr_rmp_set_xy_synchronous(leadrRmp *rmp, guint value) {
	leadr_rmp_set_value_settings(rmp, leadr_rmp_xy_sync_name, value);
}

guint leadr_rmp_get_xy_synchronous(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_xy_sync_name);
}

guint leadr_rmp_get_cpi(leadrRmp *rmp, guint bit) {
	gchar *key = NULL;
	guint result;
	key = leadr_rmp_create_cpi_name(bit);
	result = leadr_rmp_get_value(rmp, key);
	g_free(key);
	return result;
}

void leadr_rmp_set_cpi(leadrRmp *rmp, guint bit, guint value) {
	gchar *key = NULL;
	if (leadr_rmp_get_cpi(rmp, bit) != value) {
		key = leadr_rmp_create_cpi_name(bit);
		g_key_file_set_integer(rmp->key_file, leadr_rmp_group_name, key, value);
		g_free(key);
		rmp->modified_settings = TRUE;
	}
}

void leadr_rmp_set_cpi_all(leadrRmp *rmp, guint value) {
	guint i;
	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i)
		leadr_rmp_set_cpi(rmp, i, roccat_get_bit8(value, i));
}

guint8 leadr_rmp_get_cpi_all(leadrRmp *rmp) {
	guint i;
	guint8 result = 0;
	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i) {
		roccat_set_bit8(&result, i, leadr_rmp_get_cpi(rmp, i));
	}
	return result;
}

static leadrRmpLightInfo *leadr_rmp_get_light_info_value(leadrRmp *rmp, gchar const *key) {
	leadrRmpLightInfo *rmp_light_info;
	GError *error = NULL;

	rmp_light_info = roccat_key_file_get_binary(rmp->key_file, leadr_rmp_group_name, key, sizeof(leadrRmpLightInfo), &error);
	if (error) {
		g_clear_error(&error);
		rmp_light_info = roccat_key_file_get_binary(rmp->default_key_file, leadr_rmp_group_name, key, sizeof(leadrRmpLightInfo), &error);
		if (error)
			g_error(_("Could not get default value for key '%s': %s"), key, error->message);
	}

	return rmp_light_info;
}

leadrRmpLightInfo *leadr_rmp_get_custom_light_info(leadrRmp *rmp, guint index) {
	leadrRmpLightInfo *rmp_light_info;
	gchar *key = NULL;

	key = leadr_rmp_create_custom_light_info_name(index);
	rmp_light_info = leadr_rmp_get_light_info_value(rmp, key);
	g_free(key);
	return rmp_light_info;
}

leadrRmpLightInfo *leadr_rmp_get_rmp_light_info(leadrRmp *rmp, guint index) {
	leadrRmpLightInfo *rmp_light_info;
	gchar *key = NULL;

	key = leadr_rmp_create_light_info_name(index);
	rmp_light_info = leadr_rmp_get_light_info_value(rmp, key);
	g_free(key);
	return rmp_light_info;
}

static void leadr_rmp_set_light_info_value(leadrRmp *rmp, gchar const *key, leadrRmpLightInfo *rmp_light_info) {
	leadrRmpLightInfo *actual_light_info;

	actual_light_info = leadr_rmp_get_light_info_value(rmp, key);
	if (!leadr_rmp_light_info_equal(rmp_light_info, actual_light_info)) {
		leadr_rmp_light_info_finalize(rmp_light_info);
		roccat_key_file_set_binary(rmp->key_file, leadr_rmp_group_name, key, (gconstpointer)rmp_light_info, sizeof(leadrRmpLightInfo));
		rmp->modified_settings = TRUE;
	}

	g_free(actual_light_info);
}

void leadr_rmp_set_custom_light_info(leadrRmp *rmp, guint index, leadrRmpLightInfo *rmp_light_info) {
	gchar *key = NULL;

	key = leadr_rmp_create_custom_light_info_name(index);
	leadr_rmp_set_light_info_value(rmp, key, rmp_light_info);
	g_free(key);
}

void leadr_rmp_set_rmp_light_info(leadrRmp *rmp, guint index, leadrRmpLightInfo *rmp_light_info) {
	gchar *key = NULL;

	key = leadr_rmp_create_light_info_name(index);
	leadr_rmp_set_light_info_value(rmp, key, rmp_light_info);
	g_free(key);
}

void leadr_rmp_set_polling_rate(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_polling_rate_name, value);
}

guint leadr_rmp_get_polling_rate(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_polling_rate_name);
}

void leadr_rmp_set_light_effect_type(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_light_effect_type_name, value);
}

guint leadr_rmp_get_light_effect_type(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_light_effect_type_name);
}

void leadr_rmp_set_light_effect_speed(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_light_effect_speed_name, value);
}

guint leadr_rmp_get_light_effect_speed(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_light_effect_speed_name);
}

void leadr_rmp_set_light_color_flow(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_light_color_flow_name, value);
}

guint leadr_rmp_get_light_color_flow(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_light_color_flow_name);
}

void leadr_rmp_set_light_chose_type(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_light_chose_type_name, value);
}

guint leadr_rmp_get_light_chose_type(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_light_chose_type_name);
}

guint leadr_rmp_get_cpi_level_x(leadrRmp *rmp, guint index) {
	gchar *key = NULL;
	guint result;

	key = leadr_rmp_create_cpi_level_x_name(index);
	result = leadr_rmp_get_value(rmp, key);
	g_free(key);
	return result;
}

void leadr_rmp_set_cpi_level_x(leadrRmp *rmp, guint index, guint value) {
	gchar *key = NULL;
	if (leadr_rmp_get_cpi_level_x(rmp, index) != value) {
		key = leadr_rmp_create_cpi_level_x_name(index);
		g_key_file_set_integer(rmp->key_file, leadr_rmp_group_name, key, value);
		g_free(key);
		rmp->modified_settings = TRUE;
	}
}

void leadr_rmp_set_cpi_x(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_settings(rmp, leadr_rmp_cpi_x_name, value);
}

guint leadr_rmp_get_cpi_x(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_cpi_x_name);
}

gchar *leadr_rmp_get_game_file_name(leadrRmp *rmp, guint index) {
	GError *error = NULL;
	gchar *result;
	gchar *key;

	key = leadr_rmp_create_game_file_name(index);
	result = g_key_file_get_string(rmp->key_file, leadr_rmp_group_name, key, &error);
	if (error) {
		g_clear_error(&error);
		result = g_key_file_get_string(rmp->default_key_file, leadr_rmp_group_name, key, &error);
		if (error)
			g_error(_("Could not get default value for key '%s': %s"), key, error->message);
	}
	g_free(key);
	return result;
}

void leadr_rmp_set_game_file_name(leadrRmp *rmp, guint index, gchar const *string) {
	gchar *key;
	gchar *actual_string;

	key = leadr_rmp_create_game_file_name(index);
	actual_string = leadr_rmp_get_game_file_name(rmp, index);
	if (strcmp(string, actual_string)) {
		g_key_file_set_string(rmp->key_file, leadr_rmp_group_name, key, string);
		rmp->modified_rmp = TRUE;
	}
	g_free(actual_string);
	g_free(key);
}

static void leadr_rmp_set_value_rmp(leadrRmp *rmp, gchar const *key, guint value) {
	if (leadr_rmp_get_value(rmp, key) != value) {
		g_key_file_set_integer(rmp->key_file, leadr_rmp_group_name, key, value);
		rmp->modified_rmp = TRUE;
	}
}

void leadr_rmp_set_use_color_for_all(leadrRmp *rmp, guint value) {
 	leadr_rmp_set_value_rmp(rmp, leadr_rmp_use_color_for_all_name, value);
}

guint leadr_rmp_get_use_color_for_all(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_use_color_for_all_name);
}

void leadr_rmp_set_talkfx(leadrRmp *rmp, guint value) {
	leadr_rmp_set_value_settings(rmp, leadr_rmp_talkfx_name, value);
}

guint leadr_rmp_get_talkfx(leadrRmp *rmp) {
	return leadr_rmp_get_value(rmp, leadr_rmp_talkfx_name);
}
