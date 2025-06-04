#ifndef __ROCCAT_leadr_KEY_COMBO_BOX_H__
#define __ROCCAT_leadr_KEY_COMBO_BOX_H__

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
#include "roccat_key_combo_box.h"

G_BEGIN_DECLS

#define leadr_KEY_COMBO_BOX_TYPE (leadr_key_combo_box_get_type())
#define leadr_KEY_COMBO_BOX(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_KEY_COMBO_BOX_TYPE, leadrKeyComboBox))
#define IS_leadr_KEY_COMBO_BOX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_KEY_COMBO_BOX_TYPE))

typedef struct _leadrKeyComboBox leadrKeyComboBox;

typedef enum {
	leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG = ROCCAT_KEY_COMBO_BOX_GROUP_LAST << 0, /* are not supported by analog buttons */
} leadrKeyComboBoxGroup;

GType leadr_key_combo_box_get_type(void);
GtkWidget *leadr_key_combo_box_new(guint mask);
void leadr_key_combo_box_set_value_blocked(leadrKeyComboBox *key_combo_box, leadrRmpMacroKeyInfo const *key_info);
leadrRmpMacroKeyInfo *leadr_key_combo_box_get_value(leadrKeyComboBox *key_combo_box);

G_END_DECLS

#endif
