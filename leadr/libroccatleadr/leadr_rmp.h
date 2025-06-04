#ifndef __ROCCAT_leadr_RMP_H__
#define __ROCCAT_leadr_RMP_H__

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
#include "leadr_rmp_light_info.h"

G_BEGIN_DECLS

enum {
	leadr_GAMEFILE_NUM = 3,
};

typedef struct _leadrRmp leadrRmp;

struct _leadrRmp {
	gboolean modified_rmp; /* means things only in rmp like profile name */
	gboolean modified_settings;
	gboolean modified_macros[leadr_PROFILE_BUTTON_NUM];
	GKeyFile *key_file;
	GKeyFile *default_key_file;
};

void leadr_rmp_free(leadrRmp *rmp);
leadrRmp *leadr_rmp_dup(leadrRmp const *src);
leadrRmp *leadr_rmp_read_with_path(gchar const *path, GKeyFile *default_key_file, GError **error);
gboolean leadr_rmp_write_with_path(gchar const *path, leadrRmp *rmp, GError **error);
GKeyFile *leadr_rmp_defaults(void);
leadrRmp *leadr_default_rmp(void);

leadrRmpMacroKeyInfo *leadr_rmp_get_macro_key_info(leadrRmp *rmp, guint index);
void leadr_rmp_set_macro_key_info(leadrRmp *rmp, guint index, leadrRmpMacroKeyInfo *rmp_macro_key_info);
gchar *leadr_rmp_get_profile_name(leadrRmp *rmp);
void leadr_rmp_set_profile_name(leadrRmp *rmp, gchar const *string);
guint leadr_rmp_get_xy_synchronous(leadrRmp *rmp);
void leadr_rmp_set_xy_synchronous(leadrRmp *rmp, guint value);
guint leadr_rmp_get_sensitivity_x(leadrRmp *rmp);
void leadr_rmp_set_sensitivity_x(leadrRmp *rmp, guint value);
guint leadr_rmp_get_sensitivity_y(leadrRmp *rmp);
void leadr_rmp_set_sensitivity_y(leadrRmp *rmp, guint value);
guint leadr_rmp_get_sensitivity(leadrRmp *rmp);
void leadr_rmp_set_sensitivity(leadrRmp *rmp, guint value);
guint leadr_rmp_get_cpi(leadrRmp *rmp, guint bit);
void leadr_rmp_set_cpi(leadrRmp *rmp, guint bit, guint value);
guint8 leadr_rmp_get_cpi_all(leadrRmp *rmp);
void leadr_rmp_set_cpi_all(leadrRmp *rmp, guint value);
guint leadr_rmp_get_light_chose_type(leadrRmp *rmp);
void leadr_rmp_set_light_chose_type(leadrRmp *rmp, guint value);
leadrRmpLightInfo *leadr_rmp_get_custom_light_info(leadrRmp *rmp, guint index);
void leadr_rmp_set_custom_light_info(leadrRmp *rmp, guint index, leadrRmpLightInfo *rmp_light_info);
leadrRmpLightInfo *leadr_rmp_get_rmp_light_info(leadrRmp *rmp, guint index);
void leadr_rmp_set_rmp_light_info(leadrRmp *rmp, guint index, leadrRmpLightInfo *rmp_light_info);
guint leadr_rmp_get_polling_rate(leadrRmp *rmp);
void leadr_rmp_set_polling_rate(leadrRmp *rmp, guint value);
guint leadr_rmp_get_light_effect_type(leadrRmp *rmp);
void leadr_rmp_set_light_effect_type(leadrRmp *rmp, guint value);
guint leadr_rmp_get_light_effect_speed(leadrRmp *rmp);
void leadr_rmp_set_light_effect_speed(leadrRmp *rmp, guint value);
//guint leadr_rmp_get_light_effect_mode(leadrRmp *rmp);
//void leadr_rmp_set_light_effect_mode(leadrRmp *rmp, guint value);
guint leadr_rmp_get_light_color_flow(leadrRmp *rmp);
void leadr_rmp_set_light_color_flow(leadrRmp *rmp, guint value);
guint leadr_rmp_get_cpi_level_x(leadrRmp *rmp, guint index);
void leadr_rmp_set_cpi_level_x(leadrRmp *rmp, guint index, guint value);
//guint leadr_rmp_get_cpi_level_y(leadrRmp *rmp, guint index);
//void leadr_rmp_set_cpi_level_y(leadrRmp *rmp, guint index, guint value);
guint leadr_rmp_get_cpi_x(leadrRmp *rmp);
void leadr_rmp_set_cpi_x(leadrRmp *rmp, guint value);
//guint leadr_rmp_get_cpi_y(leadrRmp *rmp);
//void leadr_rmp_set_cpi_y(leadrRmp *rmp, guint value);
gchar *leadr_rmp_get_game_file_name(leadrRmp *rmp, guint index);
void leadr_rmp_set_game_file_name(leadrRmp *rmp, guint index, gchar const *string);
//guint leadr_rmp_get_light_switch(leadrRmp *rmp);
//void leadr_rmp_set_light_switch(leadrRmp *rmp, guint value);
//guint leadr_rmp_get_light_type(leadrRmp *rmp);
//void leadr_rmp_set_light_type(leadrRmp *rmp, guint value);
//guint leadr_rmp_get_color_flag(leadrRmp *rmp);
//void leadr_rmp_set_color_flag(leadrRmp *rmp, guint value);
//guint leadr_rmp_get_color_options(leadrRmp *rmp);
//void leadr_rmp_set_color_options(leadrRmp *rmp, guint value);
void leadr_rmp_set_use_color_for_all(leadrRmp *rmp, guint value);
guint leadr_rmp_get_use_color_for_all(leadrRmp *rmp);
void leadr_rmp_set_talkfx(leadrRmp *rmp, guint value);
guint leadr_rmp_get_talkfx(leadrRmp *rmp);

guint leadr_rmp_cpi_to_bin(guint rmp_value);
guint leadr_bin_cpi_to_rmp(guint bin_value);

leadrProfileSettings *leadr_rmp_to_profile_settings(leadrRmp *rmp);
leadrProfileButtons *leadr_rmp_to_profile_buttons(leadrRmp *rmp);

void leadr_rmp_update_with_profile_settings(leadrRmp *rmp, leadrProfileSettings const *profile_settings);
void leadr_rmp_update_with_profile_buttons(leadrRmp *rmp, leadrProfileButtons const *profile_buttons);
void leadr_rmp_update_with_macro(leadrRmp *rmp, guint key_index, leadrMacro const *macro);
void leadr_rmp_update_from_device(leadrRmp *rmp, RoccatDevice *leadr, guint profile_index);

gboolean leadr_rmp_get_modified(leadrRmp const *rmp);
void leadr_rmp_set_modified(leadrRmp *rmp);
void leadr_rmp_set_unmodified(leadrRmp *rmp);

leadrRmp *leadr_rmp_load_actual(guint profile_index);
leadrRmp *leadr_rmp_load(RoccatDevice *leadr, guint profile_index, GError **error);
gboolean leadr_rmp_save_actual(leadrRmp *rmp, guint profile_index, GError **error);
gboolean leadr_rmp_save(RoccatDevice *leadr, leadrRmp *rmp, guint profile_index, GError **error);
leadrRmp *leadr_rmp_load_save_after_reset(RoccatDevice *leadr, guint profile_index, GError **error);

G_END_DECLS

#endif
