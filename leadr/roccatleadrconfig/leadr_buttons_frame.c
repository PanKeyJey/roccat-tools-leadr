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

#include "leadr_profile_buttons.h"
#include "leadr_buttons_frame.h"
#include "leadr_key_combo_box.h"
#include "i18n.h"

#define leadr_BUTTONS_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_BUTTONS_FRAME_TYPE, leadrButtonsFrameClass))
#define IS_leadr_BUTTONS_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_BUTTONS_FRAME_TYPE))
#define leadr_BUTTONS_FRAME_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_BUTTONS_FRAME_TYPE, leadrButtonsFramePrivate))

typedef struct _leadrButtonsFrameClass leadrButtonsFrameClass;
typedef struct _leadrButtonsFramePrivate leadrButtonsFramePrivate;

struct _leadrButtonsFrame {
	GtkFrame parent;
	leadrButtonsFramePrivate *priv;
};

struct _leadrButtonsFrameClass {
	GtkFrameClass parent_class;
};

struct _leadrButtonsFramePrivate {
	leadrKeyComboBox *buttons[leadr_PROFILE_BUTTON_NUM];
	guint button_rmp_index[leadr_PROFILE_BUTTON_NUM];
};

G_DEFINE_TYPE(leadrButtonsFrame, leadr_buttons_frame, GTK_TYPE_FRAME);

typedef struct {
	gchar const * const name;
	leadrButtonIndex standard_index;
	guint standard_mask;
	leadrButtonIndex easyshift_index;
	guint easyshift_mask;
} ButtonList;

static ButtonList button_list[leadr_PHYSICAL_BUTTON_NUM] = {
	{N_("Left"), leadr_BUTTON_INDEX_LEFT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
		leadr_BUTTON_INDEX_SHIFT_LEFT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
	{N_("Right"), leadr_BUTTON_INDEX_RIGHT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
		leadr_BUTTON_INDEX_SHIFT_RIGHT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
	{N_("Middle"), leadr_BUTTON_INDEX_MIDDLE, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
		leadr_BUTTON_INDEX_SHIFT_MIDDLE, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Thumb Backward"), leadr_BUTTON_INDEX_THUMB_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_THUMB_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Thumb Forward"), leadr_BUTTON_INDEX_THUMB_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_THUMB_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Thumb Pedal"), leadr_BUTTON_INDEX_THUMB_PEDAL, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_THUMB_PEDAL, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("X-Celerator Up"), leadr_BUTTON_INDEX_THUMB_PADDLE_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_THUMB_PADDLE_UP, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG},
       {N_("X-Celerator Down"), leadr_BUTTON_INDEX_THUMB_PADDLE_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_THUMB_PADDLE_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG},
       {N_("Left Backward"), leadr_BUTTON_INDEX_LEFT_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_LEFT_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Left Forward"), leadr_BUTTON_INDEX_LEFT_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_LEFT_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Right Backward"), leadr_BUTTON_INDEX_RIGHT_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_RIGHT_BACK, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Right Forward"), leadr_BUTTON_INDEX_RIGHT_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_RIGHT_FORWARD, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Fin Right"), leadr_BUTTON_INDEX_FIN_RIGHT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_FIN_RIGHT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Fin Left"), leadr_BUTTON_INDEX_FIN_LEFT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_FIN_LEFT, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Wheel Up"), leadr_BUTTON_INDEX_WHEEL_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_WHEEL_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
       {N_("Wheel Down"), leadr_BUTTON_INDEX_WHEEL_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG,
               leadr_BUTTON_INDEX_SHIFT_WHEEL_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT},
};

GtkWidget *leadr_buttons_frame_new(void) {
	leadrButtonsFrame *frame;

	frame = leadr_BUTTONS_FRAME(g_object_new(leadr_BUTTONS_FRAME_TYPE,
			"label", _("Buttons"),
			NULL));

	return GTK_WIDGET(frame);
}

static void leadr_buttons_frame_init(leadrButtonsFrame *frame) {
	leadrButtonsFramePrivate *priv = leadr_BUTTONS_FRAME_GET_PRIVATE(frame);
	GtkWidget *standard_frame;
	GtkWidget *standard_table;
	GtkWidget *easyshift_frame;
	GtkWidget *easyshift_table;
	GtkWidget *hbox;
	guint i;

	frame->priv = priv;

	hbox = gtk_hbox_new(FALSE, 0);
	standard_frame = gtk_frame_new(_("Standard"));
	easyshift_frame = gtk_frame_new(_("Easyshift"));
	standard_table = gtk_table_new(leadr_PHYSICAL_BUTTON_NUM, 1, FALSE);
	easyshift_table = gtk_table_new(leadr_PHYSICAL_BUTTON_NUM, 2, FALSE);

	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_container_add(GTK_CONTAINER(standard_frame), standard_table);
	gtk_container_add(GTK_CONTAINER(easyshift_frame), easyshift_table);

	gtk_box_pack_start(GTK_BOX(hbox), standard_frame, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), easyshift_frame, TRUE, TRUE, 0);

	for (i = 0; i < leadr_PHYSICAL_BUTTON_NUM; ++i) {
		priv->button_rmp_index[i] = button_list[i].standard_index;
		priv->buttons[i] = leadr_KEY_COMBO_BOX(leadr_key_combo_box_new(button_list[i].standard_mask));
		gtk_table_attach(GTK_TABLE(standard_table), gtk_label_new(_N(button_list[i].name)), 0, 1, i, i + 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
		gtk_table_attach(GTK_TABLE(standard_table), GTK_WIDGET(priv->buttons[i]), 1, 2, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);

		priv->button_rmp_index[i + leadr_PHYSICAL_BUTTON_NUM] = button_list[i].easyshift_index;
		priv->buttons[i + leadr_PHYSICAL_BUTTON_NUM] = leadr_KEY_COMBO_BOX(leadr_key_combo_box_new(button_list[i].easyshift_mask));
		gtk_table_attach(GTK_TABLE(easyshift_table), GTK_WIDGET(priv->buttons[i + leadr_PHYSICAL_BUTTON_NUM]), 0, 1, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);
	}
}

static void leadr_buttons_frame_class_init(leadrButtonsFrameClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrButtonsFramePrivate));
}

void leadr_buttons_frame_set_from_rmp(leadrButtonsFrame *buttons_frame, leadrRmp *rmp) {
	leadrButtonsFramePrivate *priv = buttons_frame->priv;
	guint i;
	leadrRmpMacroKeyInfo *key_info;

	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
		key_info = leadr_rmp_get_macro_key_info(rmp, priv->button_rmp_index[i]);
		leadr_key_combo_box_set_value_blocked(priv->buttons[i], key_info);
	}
}

void leadr_buttons_frame_update_rmp(leadrButtonsFrame *buttons_frame, leadrRmp *rmp) {
	leadrButtonsFramePrivate *priv = buttons_frame->priv;
	guint i;
	leadrRmpMacroKeyInfo *key_info;

	for (i = 0; i < leadr_PROFILE_BUTTON_NUM; ++i) {
		key_info = leadr_key_combo_box_get_value(priv->buttons[i]);
		leadr_rmp_set_macro_key_info(rmp, priv->button_rmp_index[i], key_info);
	}
}
