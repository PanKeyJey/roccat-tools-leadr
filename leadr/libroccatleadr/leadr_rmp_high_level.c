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

#include "leadr.h"
#include "roccat_helper.h"
#include "leadr_rmp.h"
#include "roccat.h"
#include "i18n-lib.h"
#include <string.h>

static gchar *leadr_profile_dir(void) {
	gchar *base = roccat_profile_dir();
	gchar *dir = g_build_path("/", base, "leadr", NULL);
	g_free(base);
	return dir;
}

static gboolean leadr_profile_dir_create_if_needed(GError **error) {
	gchar *dir;
	gboolean retval;

	dir = leadr_profile_dir();
	retval = roccat_profile_dir_create_if_needed(dir, error);
	g_free(dir);
	return retval;
}

static gchar *leadr_build_config_rmp_path(guint profile_number) {
	gchar *dir = leadr_profile_dir();
	gchar *filename = g_strdup_printf("actual%i." ROCCAT_MOUSE_PROFILE_EXTENSION, profile_number + 1);
	gchar *path = g_build_path("/", dir, filename, NULL);
	g_free(filename);
	g_free(dir);
	return path;
}

static gboolean leadr_rmp_profile_buttons_modified(leadrRmp const *rmp) {
	guint i;

	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
		if (rmp->modified_macros[i])
			return TRUE;
	}
	return FALSE;
}

gboolean leadr_rmp_get_modified(leadrRmp const *rmp) {
	if (rmp->modified_rmp)
		return TRUE;
	if (rmp->modified_settings)
		return TRUE;
	if (leadr_rmp_profile_buttons_modified(rmp))
		return TRUE;
	return FALSE;
}

static void leadr_rmp_set_modified_state(leadrRmp *rmp, gboolean state) {
	guint i;
	rmp->modified_rmp = state;
	rmp->modified_settings = state;
	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i)
		rmp->modified_macros[i] = state;
}

void leadr_rmp_set_modified(leadrRmp *rmp) {
	leadr_rmp_set_modified_state(rmp, TRUE);
}

void leadr_rmp_set_unmodified(leadrRmp *rmp) {
	leadr_rmp_set_modified_state(rmp, FALSE);
}

leadrRmp *leadr_rmp_load_actual(guint profile_index) {
	leadrRmp *rmp;
	gchar *config_path;

	config_path = leadr_build_config_rmp_path(profile_index);
	rmp = leadr_rmp_read_with_path(config_path, leadr_rmp_defaults(), NULL);
	g_free(config_path);
	if (!rmp) {
		rmp = leadr_default_rmp();
		rmp->modified_rmp = TRUE;
	}
	return rmp;
}

static void leadr_rmp_update_with_hardware_macro(leadrRmp *rmp, RoccatDevice *device, guint profile_index, guint profile_buttons_index) {
	GError *error = NULL;
	leadrMacro *hardware_macro;
	hardware_macro = leadr_macro_read(device, profile_index, profile_buttons_index, &error);
	if (error) {
		g_clear_error(&error);
	} else {
		leadr_rmp_update_with_macro(rmp, profile_buttons_index, hardware_macro);
		g_free(hardware_macro);
	}
}

void leadr_rmp_update_from_device(leadrRmp *rmp, RoccatDevice *leadr, guint profile_index) {
	leadrProfileButtons *hardware_profile_buttons, *rmp_profile_buttons;
	leadrProfileSettings *hardware_profile_settings, *rmp_profile_settings;
	GError *error = NULL;
	guint i;

	gaminggear_device_lock(GAMINGGEAR_DEVICE(leadr));

	/* hardware takes precedence */
	hardware_profile_settings = leadr_profile_settings_read(leadr, profile_index, &error);

	if (hardware_profile_settings) {
		rmp_profile_settings = leadr_rmp_to_profile_settings(rmp);
		leadr_profile_settings_finalize(rmp_profile_settings, profile_index);

		if (!leadr_profile_settings_equal(hardware_profile_settings, rmp_profile_settings)) {
			g_warning(_("Hardware data and rmp for profile %u differ"), profile_index + 1);
			leadr_rmp_update_with_profile_settings(rmp, hardware_profile_settings);
		}
		g_free(rmp_profile_settings);
		g_free(hardware_profile_settings);
	} else
		g_clear_error(&error);

	hardware_profile_buttons = leadr_profile_buttons_read(leadr, profile_index, &error);

	if (hardware_profile_buttons) {
		rmp_profile_buttons = leadr_rmp_to_profile_buttons(rmp);

		if (!leadr_profile_buttons_equal(hardware_profile_buttons, rmp_profile_buttons)) {
			g_warning(_("Hardware data and rmp for profile %u differ"), profile_index + 1);
			leadr_rmp_update_with_profile_buttons(rmp, hardware_profile_buttons);
		}

		g_free(rmp_profile_buttons);

		for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
			if (hardware_profile_buttons->buttons[i].type == leadr_BUTTON_TYPE_MACRO)
				leadr_rmp_update_with_hardware_macro(rmp, leadr, profile_index, i);
		}

		g_free(hardware_profile_buttons);
	} else
		g_clear_error(&error);

	gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
}

leadrRmp *leadr_rmp_load(RoccatDevice *leadr, guint profile_index, GError **error) {
	leadrRmp *rmp;

	g_assert(profile_index < leadr_PROFILE_NUM);

	rmp = leadr_rmp_load_actual(profile_index);

	leadr_rmp_update_from_device(rmp, leadr, profile_index);

	return rmp;
}

gboolean leadr_rmp_save_actual(leadrRmp *rmp, guint profile_index, GError **error) {
	gchar *config_path;
	gboolean retval;

	leadr_profile_dir_create_if_needed(error);
	if (*error)
		return FALSE;

	config_path = leadr_build_config_rmp_path(profile_index);
	retval = leadr_rmp_write_with_path(config_path, rmp, error);
	g_free(config_path);

	return retval;
}

static gboolean leadr_rmp_save_profile_settings(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error) {
	leadrProfileSettings *profile_settings;
	gboolean retval;

	profile_settings = leadr_rmp_to_profile_settings(rmp);
	retval = leadr_profile_settings_write(leadr, profile_index, profile_settings, error);
	g_free(profile_settings);

	return retval;
}

static gboolean leadr_rmp_save_profile_buttons(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error) {
	leadrProfileButtons *profile_buttons;
	gboolean retval;

	profile_buttons = leadr_rmp_to_profile_buttons(rmp);
	retval = leadr_profile_buttons_write(leadr, profile_index, profile_buttons, error);
	g_free(profile_buttons);

	return retval;
}

static gboolean leadr_rmp_save_macro(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, guint key_index, GError **error) {
	leadrRmpMacroKeyInfo *macro_key_info;
	leadrMacro *macro;
	guint retval = TRUE;

	macro_key_info = leadr_rmp_get_macro_key_info(rmp, key_index);
	macro = leadr_rmp_macro_key_info_to_macro(macro_key_info);
	g_free(macro_key_info);

	if (macro) {
		retval = leadr_macro_write(leadr, profile_index, key_index, macro, error);
		g_free(macro);
	}

	return retval;
}

static gboolean leadr_rmp_save_actual_when_needed(leadrRmp *rmp, guint profile_index, GError **error) {
	gboolean retval = TRUE;

	if (leadr_rmp_get_modified(rmp)) {
		retval = leadr_rmp_save_actual(rmp, profile_index, error);
		rmp->modified_rmp = FALSE;
	}

	return retval;
}

static gboolean leadr_rmp_save_profile_settings_when_needed(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error) {
	gboolean retval = TRUE;

	if (rmp->modified_settings) {
		retval = leadr_rmp_save_profile_settings(leadr, rmp, profile_index, error);
		rmp->modified_settings = FALSE;
	}

	return retval;
}

static gboolean leadr_rmp_save_macro_when_needed(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, guint key_index, GError **error) {
	gboolean retval = TRUE;

	if (rmp->modified_macros[key_index]) {
		retval = leadr_rmp_save_macro(leadr, rmp, profile_index, key_index, error);
		rmp->modified_macros[key_index] = FALSE;
	}

	return retval;
}

static gboolean leadr_rmp_save_profile_buttons_when_needed(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error) {
	gboolean retval = TRUE;
	guint i;

	if (leadr_rmp_profile_buttons_modified(rmp)) {
		retval = leadr_rmp_save_profile_buttons(leadr, rmp, profile_index, error);
		if (!retval)
			return FALSE;

		for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
			if (!leadr_rmp_save_macro_when_needed(leadr, rmp, profile_index, i, error))
				return FALSE;
		}
	}
	return retval;
}

gboolean leadr_rmp_save(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error) {
	gboolean retval = FALSE;

	g_assert(profile_index < leadr_PROFILE_NUM);

	if (!leadr_rmp_save_actual_when_needed(rmp, profile_index, error)) return FALSE;

	gaminggear_device_lock(GAMINGGEAR_DEVICE(leadr));

	if (!leadr_rmp_save_profile_buttons_when_needed(leadr, rmp, profile_index, error)) goto error;
	if (!leadr_rmp_save_profile_settings_when_needed(leadr, rmp, profile_index, error)) goto error;

	retval = TRUE;
error:
	gaminggear_device_unlock(GAMINGGEAR_DEVICE(leadr));
	return retval;
}

leadrRmp *leadr_rmp_load_save_after_reset(RoccatDevice *leadr, guint profile_index, GError **error) {
	leadrRmp *rmp;

	rmp = leadr_default_rmp();

	/* Hardware profiles differ in light-color and -effects */
	leadr_rmp_update_from_device(rmp, leadr, profile_index);
	leadr_rmp_set_unmodified(rmp);

	leadr_rmp_save_actual(rmp, profile_index, error);
	return rmp;
}
