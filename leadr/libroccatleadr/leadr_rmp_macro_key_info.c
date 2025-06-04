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

#include "leadr_rmp_macro_key_info.h"
#include "leadr_device.h"
#include "roccat_helper.h"
#include "roccat.h"
#include "i18n-lib.h"
#include <string.h>

guint8 leadr_rmp_macro_key_info_calc_checksum(leadrRmpMacroKeyInfo const *rmp_macro_key_info) {
	return ROCCAT_BYTESUM_PARTIALLY(rmp_macro_key_info, leadrRmpMacroKeyInfo, button_index, checksum);
}

void leadr_rmp_macro_key_info_set_checksum(leadrRmpMacroKeyInfo *rmp_macro_key_info) {
	rmp_macro_key_info->checksum = leadr_rmp_macro_key_info_calc_checksum(rmp_macro_key_info);
}

static void leadr_rmp_macro_key_info_set_macroset_name(leadrRmpMacroKeyInfo *macro_key_info, gchar const *new_name) {
	g_strlcpy((gchar *)(macro_key_info->macroset_name), new_name, leadr_RMP_MACRO_KEY_INFO_MACROSET_NAME_LENGTH);
}

static void leadr_rmp_macro_key_info_set_macro_name(leadrRmpMacroKeyInfo *macro_key_info, gchar const *new_name) {
	g_strlcpy((gchar *)(macro_key_info->macro_name), new_name, leadr_RMP_MACRO_KEY_INFO_MACRO_NAME_LENGTH);
}

static void leadr_rmp_macro_key_info_set_filename(leadrRmpMacroKeyInfo *macro_key_info, gchar const *new_name) {
	g_strlcpy((gchar *)(macro_key_info->filename), new_name, leadr_RMP_MACRO_KEY_INFO_FILENAME_LENGTH);
}

static void leadr_rmp_macro_key_info_set_timer_name(leadrRmpMacroKeyInfo *macro_key_info, gchar const *new_name) {
	g_strlcpy((gchar *)(macro_key_info->timer_name), new_name, leadr_RMP_MACRO_KEY_INFO_TIMER_NAME_LENGTH);
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new(void) {
	return (leadrRmpMacroKeyInfo *)g_malloc0(sizeof(leadrRmpMacroKeyInfo));
}

void leadr_rmp_macro_key_info_free(leadrRmpMacroKeyInfo *macro_key_info) {
	g_free(macro_key_info);
}

leadrRmpMacroKeyInfo *gaminggear_macro_to_leadr_rmp_macro_key_info(GaminggearMacro const *gaminggear_macro, GError **error) {
	leadrRmpMacroKeyInfo *result;
	guint i, count;

	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

	count = gaminggear_macro_keystrokes_get_count(&gaminggear_macro->keystrokes);
	if (count > leadr_RMP_MACRO_KEY_INFO_KEYSTROKES_NUM) {
		g_set_error(error, ROCCAT_ERROR, ROCCAT_ERROR_MACRO_TOO_LONG,
				_("Macro contains %u actions while device only supports %u actions"), count, leadr_RMP_MACRO_KEY_INFO_KEYSTROKES_NUM);
		return NULL;
	}

	result = leadr_rmp_macro_key_info_new();
	result->type = leadr_BUTTON_TYPE_MACRO;
	leadr_rmp_macro_key_info_set_macroset_name(result, gaminggear_macro->macroset);
	leadr_rmp_macro_key_info_set_macro_name(result, gaminggear_macro->macro);
	leadr_rmp_macro_key_info_set_count(result, count);
	result->loop = gaminggear_macro->keystrokes.loop;

	for (i = 0; i < count; ++i) {
		gaminggear_macro_keystroke_to_roccat_keystroke(&gaminggear_macro->keystrokes.keystrokes[i], &result->keystrokes[i]);
	}

	return result;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new_quicklaunch(gchar const *filename) {
	leadrRmpMacroKeyInfo *result = leadr_rmp_macro_key_info_new();
	result->type = leadr_BUTTON_TYPE_QUICKLAUNCH;
	leadr_rmp_macro_key_info_set_filename(result, filename);
	return result;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new_special(leadrButtonType type) {
	leadrRmpMacroKeyInfo *result = leadr_rmp_macro_key_info_new();
	result->type = type;
	return result;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new_timer(gchar const *name, guint seconds) {
	leadrRmpMacroKeyInfo *result = leadr_rmp_macro_key_info_new();
	result->type = leadr_BUTTON_TYPE_TIMER;
	leadr_rmp_macro_key_info_set_timer_name(result, name);
	leadr_rmp_macro_key_info_set_timer_length(result, seconds);
	return result;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new_talk(leadrButtonType type, guint talk_device) {
	leadrRmpMacroKeyInfo *result = leadr_rmp_macro_key_info_new();
	result->type = type;
	leadr_rmp_macro_key_info_set_talk_device(result, talk_device);
	return result;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_new_shortcut(guint hid_usage_id,
		guint8 modifier) {
	leadrRmpMacroKeyInfo *result = leadr_rmp_macro_key_info_new();

	result->type = leadr_BUTTON_TYPE_SHORTCUT;
	result->keystrokes[0].key = hid_usage_id;
	leadr_rmp_macro_key_info_set_count(result, 1);
	result->keystrokes[0].action = modifier;

	return result;
}

gboolean leadr_rmp_macro_key_info_equal(leadrRmpMacroKeyInfo const *left, leadrRmpMacroKeyInfo const *right) {
	gboolean equal;
	equal = ROCCAT_MEMCMP_PARTIALLY(left, right, leadrRmpMacroKeyInfo, type, checksum);
	return equal ? FALSE : TRUE;
}

leadrRmpMacroKeyInfo *leadr_rmp_macro_key_info_dup(leadrRmpMacroKeyInfo const *source) {
	return g_memdup((gconstpointer)source, sizeof(leadrRmpMacroKeyInfo));
}

leadrMacro *leadr_rmp_macro_key_info_to_macro(leadrRmpMacroKeyInfo const *macro_key_info) {
	leadrMacro *macro;
	guint i;

	if (macro_key_info->type != leadr_BUTTON_TYPE_MACRO)
		return NULL;

	macro = g_malloc0(sizeof(leadrMacro));

	macro->button_index = macro_key_info->button_index;
	macro->loop = leadr_rmp_macro_key_info_get_loop(macro_key_info);
	leadr_macro_set_macroset_name(macro, (gchar const *)macro_key_info->macroset_name);
	leadr_macro_set_macro_name(macro, (gchar const *)macro_key_info->macro_name);
	leadr_macro_set_count(macro, leadr_rmp_macro_key_info_get_count(macro_key_info));

	for (i = 0; i < leadr_macro_get_count(macro); ++i)
		macro->keystrokes[i] = macro_key_info->keystrokes[i];

	return macro;
}

leadrRmpMacroKeyInfo *leadr_macro_to_rmp_macro_key_info(leadrMacro const *macro) {
	leadrRmpMacroKeyInfo *key_info;
	key_info = leadr_rmp_macro_key_info_new();
	guint i, count;

	count = leadr_macro_get_count(macro);
	count = MIN(count, leadr_RMP_MACRO_KEY_INFO_KEYSTROKES_NUM);
	count = MIN(count, leadr_MACRO_KEYSTROKES_NUM);

	key_info->button_index = macro->button_index;
	key_info->type = leadr_BUTTON_TYPE_MACRO;
	leadr_rmp_macro_key_info_set_macroset_name(key_info, (gchar const *)macro->macroset_name);
	leadr_rmp_macro_key_info_set_macro_name(key_info, (gchar const *)macro->macro_name);
	leadr_rmp_macro_key_info_set_loop(key_info, macro->loop);
	leadr_rmp_macro_key_info_set_count(key_info, count);
	for (i = 0; i < count; ++i)
		key_info->keystrokes[i] = macro->keystrokes[i];

	return key_info;
}
