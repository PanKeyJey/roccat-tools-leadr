/*
 * This file is part of roccat-tools.
 *
 * roccat-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Macro Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * roccat-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Macro Public License for more details.
 *
 * You should have received a copy of the GNU Macro Public License
 * along with roccat-tools. If not, see <http://www.gnu.org/licenses/>.
 */

#include "leadr_macro.h"
#include "leadr_device.h"
#include "roccat_helper.h"
#include "roccat.h"
#include "i18n-lib.h"
#include <string.h>

typedef struct _leadrMacro1 leadrMacro1;
typedef struct _leadrMacro2 leadrMacro2;

enum {
	leadr_MACRO_1_DATA_SIZE = 1024,
	leadr_MACRO_2_DATA_SIZE = sizeof(leadrMacro) - leadr_MACRO_1_DATA_SIZE,
	leadr_MACRO_2_UNUSED_SIZE = 1024 - leadr_MACRO_2_DATA_SIZE,
};

struct _leadrMacro1 {
	guint8 report_id;
	guint8 one;
	guint8 data[leadr_MACRO_1_DATA_SIZE];
} __attribute__ ((packed));

struct _leadrMacro2 {
	guint8 report_id;
	guint8 two;
	guint8 data[leadr_MACRO_2_DATA_SIZE];
	guint8 unused[leadr_MACRO_2_UNUSED_SIZE];
} __attribute__ ((packed));

void leadr_macro_set_macroset_name(leadrMacro *macro, gchar const *new_name) {
	g_strlcpy((gchar *)(macro->macroset_name), new_name, leadr_MACRO_MACROSET_NAME_LENGTH);
}

void leadr_macro_set_macro_name(leadrMacro *macro, gchar const *new_name) {
	g_strlcpy((gchar *)(macro->macro_name), new_name, leadr_MACRO_MACRO_NAME_LENGTH);
}

static void leadr_macro_finalize(leadrMacro *macro, guint profile_index, guint button_index) {
	macro->profile_index = profile_index;
	macro->button_index = button_index;
}

gboolean leadr_macro_write(RoccatDevice *leadr, guint profile_index, guint button_index, leadrMacro *macro, GError **error) {
	leadrMacro1 macro1;
	leadrMacro2 macro2;
	gboolean result;

	g_assert(profile_index < leadr_PROFILE_NUM);

	leadr_macro_finalize(macro, profile_index, button_index);

	macro1.report_id = leadr_REPORT_ID_MACRO;
	macro1.one = 1;
	memcpy(macro1.data, macro, leadr_MACRO_1_DATA_SIZE);

	macro2.report_id = leadr_REPORT_ID_MACRO;
	macro2.two = 2;
	memcpy(macro2.data, (guint8 *)macro + leadr_MACRO_1_DATA_SIZE, leadr_MACRO_2_DATA_SIZE);
	memset(macro2.unused, 0, leadr_MACRO_2_UNUSED_SIZE);

	result = leadr_device_write(leadr, (gchar const *)&macro1, sizeof(leadrMacro1), error);
	if (!result)
		return FALSE;

	return leadr_device_write(leadr, (gchar const *)&macro2, sizeof(leadrMacro2), error);
}

leadrMacro *leadr_macro_read(RoccatDevice *leadr, guint profile_index, guint button_index, GError **error) {
	leadrMacro1 *macro1;
	leadrMacro2 *macro2;
	leadrMacro *macro;

	g_assert(profile_index < leadr_PROFILE_NUM);

	gaminggear_device_lock(GAMINGGEAR_DEVICE(leadr));

	if (!leadr_select(leadr, profile_index, leadr_CONTROL_DATA_INDEX_MACRO_1, button_index, error)) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
		return NULL;
	}

	macro1 = (leadrMacro1 *)leadr_device_read(leadr, leadr_REPORT_ID_MACRO, sizeof(leadrMacro1), error);
	if (!macro1) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
		return NULL;
	}

	if (!leadr_select(leadr, profile_index, leadr_CONTROL_DATA_INDEX_MACRO_2, button_index, error)) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
		g_free(macro1);
		return NULL;
	}

	macro2 = (leadrMacro2 *)leadr_device_read(leadr, leadr_REPORT_ID_MACRO, sizeof(leadrMacro2), error);
	if (!macro2) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
		g_free(macro1);
		return NULL;
	}

	gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));

	macro = (leadrMacro *)g_malloc(sizeof(leadrMacro));
	memcpy(macro, macro1->data, leadr_MACRO_1_DATA_SIZE);
	memcpy((guint8 *)macro + leadr_MACRO_1_DATA_SIZE, macro2->data, leadr_MACRO_2_DATA_SIZE);

	g_free(macro1);
	g_free(macro2);

	return macro;
}

gboolean gaminggear_macro_to_leadr_macro(GaminggearMacro const *gaminggear_macro, leadrMacro *leadr_macro, GError **error) {
	guint i, count;

	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

	count = gaminggear_macro_keystrokes_get_count(&gaminggear_macro->keystrokes);
	if (count > leadr_MACRO_KEYSTROKES_NUM) {
		g_set_error(error, ROCCAT_ERROR, ROCCAT_ERROR_MACRO_TOO_LONG,
				_("Macro contains %u actions while device only supports %u actions"), count, leadr_MACRO_KEYSTROKES_NUM);
		return FALSE;
	}

	memset(leadr_macro, 0, sizeof(leadrMacro));
	leadr_macro_set_count(leadr_macro, count);
	leadr_macro->loop = gaminggear_macro->keystrokes.loop;
	leadr_macro_set_macroset_name(leadr_macro, gaminggear_macro->macroset);
	leadr_macro_set_macro_name(leadr_macro, gaminggear_macro->macro);

	for (i = 0; i < count; ++i)
		gaminggear_macro_keystroke_to_roccat_keystroke(&gaminggear_macro->keystrokes.keystrokes[i], &leadr_macro->keystrokes[i]);

	return TRUE;
}

GaminggearMacro *leadr_macro_to_gaminggear_macro(leadrMacro const *leadr_macro) {
	GaminggearMacro *gaminggear_macro;
	GaminggearMacroKeystroke keystroke;
	guint i, count;

	gaminggear_macro = gaminggear_macro_new((gchar const *)leadr_macro->macroset_name, (gchar const *)leadr_macro->macro_name, NULL);

	gaminggear_macro->keystrokes.loop = leadr_macro->loop;

	count = leadr_macro_get_count(leadr_macro);
	count = MIN(count, GAMINGGEAR_MACRO_KEYSTROKES_NUM);
	count = MIN(count, leadr_MACRO_KEYSTROKES_NUM);

	for (i = 0; i < count; ++i) {
		roccat_keystroke_to_gaminggear_macro_keystroke(&leadr_macro->keystrokes[i], &keystroke);
		gaminggear_macro_keystrokes_add(&gaminggear_macro->keystrokes, &keystroke);
	}

	return gaminggear_macro;
}

gboolean leadr_macro_equal(leadrMacro const *left, leadrMacro const *right) {
	gboolean equal;
	equal = ROCCAT_MEMCMP_PARTIALLY_TO_END(left, right, leadrMacro, loop);
	return equal ? FALSE : TRUE;
}

void leadr_macro_copy(leadrMacro *destination, leadrMacro const *source) {
	memcpy(destination, source, sizeof(leadrMacro));
}
