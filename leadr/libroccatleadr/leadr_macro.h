#ifndef __ROCCAT_leadr_MACRO_H__
#define __ROCCAT_leadr_MACRO_H__

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

#include <gaminggear/macro.h>
#include "roccat_device.h"
#include "roccat_keystroke.h"
#include "leadr.h"

G_BEGIN_DECLS

enum {
	leadr_MACRO_MACROSET_NAME_LENGTH = 24,
	leadr_MACRO_MACRO_NAME_LENGTH = 24,
	leadr_MACRO_KEYSTROKES_NUM = 480,
};

typedef struct _leadrMacro leadrMacro;

/* This structure is transferred to hardware in 2 parts */
struct _leadrMacro {
	guint8 profile_index;
	guint8 button_index;
	guint8 loop;
	guint8 unused1[24];
	guint8 macroset_name[leadr_MACRO_MACROSET_NAME_LENGTH];
	guint8 macro_name[leadr_MACRO_MACRO_NAME_LENGTH];
	guint16 count;
	RoccatKeystroke keystrokes[leadr_MACRO_KEYSTROKES_NUM];
} __attribute__ ((packed));

static inline guint16 leadr_macro_get_count(leadrMacro const *macro) {
	return GUINT16_FROM_LE(macro->count);
}

static inline void leadr_macro_set_count(leadrMacro *macro, guint16 new_value) {
	macro->count = GUINT16_TO_LE(new_value);
}

void leadr_macro_set_macroset_name(leadrMacro *macro, gchar const *new_name);
void leadr_macro_set_macro_name(leadrMacro *macro, gchar const *new_name);
gboolean leadr_macro_write(RoccatDevice *leadr, guint profile_index, guint button_index, leadrMacro *macro, GError **error);
leadrMacro *leadr_macro_read(RoccatDevice *leadr, guint profile_index, guint button_index, GError **error);
gboolean gaminggear_macro_to_leadr_macro(GaminggearMacro const *gaminggear_macro, leadrMacro *leadr_macro, GError **error);
GaminggearMacro *leadr_macro_to_gaminggear_macro(leadrMacro const *leadr_macro);
gboolean leadr_macro_equal(leadrMacro const *left, leadrMacro const *right);
void leadr_macro_copy(leadrMacro *destination, leadrMacro const *source);

G_END_DECLS

#endif
