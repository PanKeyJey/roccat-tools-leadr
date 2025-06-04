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

#include <gaminggear/gaminggear_macro_dialog.h>
#include <gaminggear/key_translations.h>
#include <gaminggear/gdk_key_translations.h>
#include "leadr_key_combo_box.h"
#include "leadr_profile_buttons.h"
#include "roccat_talk_target_dialog.h"
#include "roccat_shortcut_dialog.h"
#include "roccat_timer_dialog.h"
#include "roccat_warning_dialog.h"
#include "roccat_helper.h"
#include "roccat_talk.h"
#include "gtk_roccat_helper.h"
#include "i18n.h"

#define leadr_KEY_COMBO_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_KEY_COMBO_BOX_TYPE, leadrKeyComboBoxClass))
#define IS_leadr_KEY_COMBO_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_KEY_COMBO_BOX_TYPE))
#define leadr_KEY_COMBO_BOX_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), leadr_KEY_COMBO_BOX_TYPE, leadrKeyComboBoxClass))
#define leadr_KEY_COMBO_BOX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_KEY_COMBO_BOX_TYPE, leadrKeyComboBoxPrivate))

typedef struct _leadrKeyComboBoxClass leadrKeyComboBoxClass;
typedef struct _leadrKeyComboBoxPrivate leadrKeyComboBoxPrivate;

struct _leadrKeyComboBox {
	RoccatKeyComboBox parent;
	leadrKeyComboBoxPrivate *priv;
};

struct _leadrKeyComboBoxClass {
	RoccatKeyComboBoxClass parent_class;
	GtkTreeModel *model;
};

struct _leadrKeyComboBoxPrivate {
	leadrRmpMacroKeyInfo *key_info;
	gulong changed_handler_id;
};

G_DEFINE_TYPE(leadrKeyComboBox, leadr_key_combo_box, ROCCAT_KEY_COMBO_BOX_TYPE);

static void set_text(leadrKeyComboBox *key_combo_box) {
	leadrKeyComboBoxPrivate *priv = key_combo_box->priv;
	gchar *text;
	gchar *temp;

	switch (priv->key_info->type) {
	case leadr_BUTTON_TYPE_SHORTCUT:
		temp = roccat_shortcut_dialog_create_name(priv->key_info->keystrokes[0].action, priv->key_info->keystrokes[0].key);
		text = g_strdup_printf(_("Shortcut: %s"), temp);
		g_free(temp);
		break;
	case leadr_BUTTON_TYPE_MACRO:
		text = g_strdup_printf(_("Macro: %s/%s"), (gchar const *)priv->key_info->macroset_name, (gchar const *)priv->key_info->macro_name);
		break;
	case leadr_BUTTON_TYPE_QUICKLAUNCH:
		temp = g_path_get_basename((gchar const *)priv->key_info->filename);
		text = g_strdup_printf(_("Quicklaunch: %s"), temp);
		g_free(temp);
		break;
	case leadr_BUTTON_TYPE_TIMER:
		text = g_strdup_printf(_("Timer: %s"), (gchar const *)priv->key_info->timer_name);
		break;
	case leadr_BUTTON_TYPE_EASYSHIFT_OTHER:
		text = g_strdup_printf(_("Easyshift: %s"), roccat_talk_device_get_text_static(leadr_rmp_macro_key_info_get_talk_device(priv->key_info)));
		break;
	case leadr_BUTTON_TYPE_EASYSHIFT_LOCK_OTHER:
		text = g_strdup_printf(_("Easyshift lock: %s"), roccat_talk_device_get_text_static(leadr_rmp_macro_key_info_get_talk_device(priv->key_info)));
		break;
	default:
		text = roccat_key_combo_box_model_get_title_for_type(gtk_combo_box_get_model(GTK_COMBO_BOX(key_combo_box)), priv->key_info->type);
		break;
	}

	g_signal_handler_block(G_OBJECT(key_combo_box), priv->changed_handler_id);
	roccat_key_combo_box_set_text(ROCCAT_KEY_COMBO_BOX(key_combo_box), text);
	g_signal_handler_unblock(G_OBJECT(key_combo_box), priv->changed_handler_id);

	g_free(text);
}

static leadrRmpMacroKeyInfo *button_quicklaunch(GtkWindow *parent) {
	GtkWidget *dialog;
	gchar *filename;
	leadrRmpMacroKeyInfo *key_info = NULL;

	dialog = gtk_file_chooser_dialog_new(_("Select executable"),
			parent,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL
	);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		key_info = leadr_rmp_macro_key_info_new_quicklaunch(filename);
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
	return key_info;
}

static leadrRmpMacroKeyInfo *button_macro(GtkWindow *parent) {
	GaminggearMacro *gaminggear_macro;
	leadrRmpMacroKeyInfo *key_info = NULL;
	GError *local_error = NULL;

	gaminggear_macro = gaminggear_macro_dialog(parent);
	if (!gaminggear_macro)
		return NULL;

	key_info = gaminggear_macro_to_leadr_rmp_macro_key_info(gaminggear_macro, &local_error);
	gaminggear_macro_free(gaminggear_macro);
	if (!key_info)
		roccat_handle_error_dialog(parent, _("Could not convert macro"), &local_error);

	return key_info;
}

static leadrRmpMacroKeyInfo *button_shortcut(GtkWindow *parent) {
	RoccatShortcutDialog *dialog;
	leadrRmpMacroKeyInfo *key_info = NULL;

	dialog = ROCCAT_SHORTCUT_DIALOG(roccat_shortcut_dialog_new(parent));
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		key_info = leadr_rmp_macro_key_info_new_shortcut(
				roccat_shortcut_dialog_get_hid(dialog),
				roccat_button_build_shortcut_modifier(
						roccat_shortcut_dialog_get_ctrl(dialog),
						roccat_shortcut_dialog_get_shift(dialog),
						roccat_shortcut_dialog_get_alt(dialog),
						roccat_shortcut_dialog_get_win(dialog)));
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
	return key_info;
}

static leadrRmpMacroKeyInfo *button_timer(GtkWindow *parent) {
	leadrRmpMacroKeyInfo *key_info = NULL;
	RoccatTimer *timer;

	timer = roccat_timer_dialog(parent);

	if (timer) {
		key_info = leadr_rmp_macro_key_info_new_timer((gchar const *)timer->name, roccat_timer_get_seconds(timer));
		roccat_timer_free(timer);
	}

	return key_info;
}

static leadrRmpMacroKeyInfo *button_talk_other(leadrKeyComboBox *key_combo_box, gint key_type) {
	leadrKeyComboBoxPrivate *priv = key_combo_box->priv;
	RoccatTalkTargetDialog *talk_target_dialog;
	guint target;
	leadrRmpMacroKeyInfo *new_key_info = NULL;

	talk_target_dialog = ROCCAT_TALK_TARGET_DIALOG(roccat_talk_target_dialog_new(
			gtk_roccat_widget_get_gtk_window(GTK_WIDGET(key_combo_box)), ROCCAT_TALK_DEVICE_MOUSE));

	roccat_talk_target_dialog_set_value(talk_target_dialog, leadr_rmp_macro_key_info_get_talk_device(priv->key_info));

	if (gtk_dialog_run(GTK_DIALOG(talk_target_dialog)) == GTK_RESPONSE_ACCEPT) {
		target = roccat_talk_target_dialog_get_value(talk_target_dialog);
		new_key_info = leadr_rmp_macro_key_info_new_talk(key_type, target);
	}

	gtk_widget_destroy(GTK_WIDGET(talk_target_dialog));

	return new_key_info;
}

static leadrRmpMacroKeyInfo *get_macro_key_info(leadrKeyComboBox *key_combo_box, gint type) {
	GtkWindow *parent = gtk_roccat_widget_get_gtk_window(GTK_WIDGET(key_combo_box));
	leadrRmpMacroKeyInfo *key_info;

	switch (type) {
	case leadr_BUTTON_TYPE_MACRO:
		key_info = button_macro(parent);
		break;
	case leadr_BUTTON_TYPE_SHORTCUT:
		key_info = button_shortcut(parent);
		break;
	case leadr_BUTTON_TYPE_QUICKLAUNCH:
		key_info = button_quicklaunch(parent);
		break;
	case leadr_BUTTON_TYPE_TIMER:
		key_info = button_timer(parent);
		break;
	case leadr_BUTTON_TYPE_EASYSHIFT_OTHER:
	case leadr_BUTTON_TYPE_EASYSHIFT_LOCK_OTHER:
		key_info = button_talk_other(key_combo_box, type);
		break;
	case leadr_BUTTON_TYPE_EASYSHIFT_ALL:
		key_info = leadr_rmp_macro_key_info_new_talk(type, ROCCAT_TALK_DEVICE_ALL);
		break;
	default:
		key_info = leadr_rmp_macro_key_info_new_special(type);
	}

	return key_info;
}

static void button_changed_cb(GtkComboBox *combo, gpointer user_data) {
	leadrKeyComboBox *key_combo_box = leadr_KEY_COMBO_BOX(combo);
	leadrKeyComboBoxPrivate *priv = key_combo_box->priv;
	gint type_data;
	leadrRmpMacroKeyInfo *new_key_info;

	type_data = roccat_key_combo_box_get_active_type(ROCCAT_KEY_COMBO_BOX(combo));
	if (type_data == ROCCAT_KEY_COMBO_BOX_TYPE_NONE)
		return;

	new_key_info = get_macro_key_info(key_combo_box, type_data);
	if (new_key_info) {
		leadr_rmp_macro_key_info_free(priv->key_info);
		priv->key_info = new_key_info;
	}
	set_text(key_combo_box);
}

GtkWidget *leadr_key_combo_box_new(guint mask) {
	leadrKeyComboBox *key_combo_box;
	leadrKeyComboBoxClass *key_combo_box_class;
	leadrKeyComboBoxPrivate *priv;

	key_combo_box = leadr_KEY_COMBO_BOX(g_object_new(leadr_KEY_COMBO_BOX_TYPE,
			"exclude-mask", mask,
			NULL));

	priv = key_combo_box->priv;

	key_combo_box_class = leadr_KEY_COMBO_BOX_GET_CLASS(key_combo_box);
	roccat_key_combo_box_set_model(ROCCAT_KEY_COMBO_BOX(key_combo_box), key_combo_box_class->model);

	priv->changed_handler_id = g_signal_connect(G_OBJECT(key_combo_box), "changed", G_CALLBACK(button_changed_cb), NULL);

	return GTK_WIDGET(key_combo_box);
}

static void leadr_key_combo_box_init(leadrKeyComboBox *key_combo_box) {
	leadrKeyComboBoxPrivate *priv = leadr_KEY_COMBO_BOX_GET_PRIVATE(key_combo_box);
	key_combo_box->priv = priv;
}

static GtkTreeModel *button_info_model_new(void) {
	GtkTreeStore *store;
	GtkTreeIter toplevel, sublevel;

	store = roccat_key_combo_box_store_new();

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Buttons"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_NONE | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 1 (Click)"), leadr_BUTTON_TYPE_CLICK, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 2 (Universal scrolling)"), leadr_BUTTON_TYPE_UNIVERSAL_SCROLLING, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 3 (Menu)"), leadr_BUTTON_TYPE_MENU, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 4 (Scroll up)"), leadr_BUTTON_TYPE_SCROLL_UP, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 5 (Scroll down)"), leadr_BUTTON_TYPE_SCROLL_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 6 (Tilt left)"), leadr_BUTTON_TYPE_TILT_LEFT, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 7 (Tilt right)"), leadr_BUTTON_TYPE_TILT_RIGHT, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 8 (Browser backward)"), leadr_BUTTON_TYPE_BROWSER_BACKWARD, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Button 9 (Browser forward)"), leadr_BUTTON_TYPE_BROWSER_FORWARD, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Double click"), leadr_BUTTON_TYPE_DOUBLE_CLICK, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("CPI"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("CPI cycle"), leadr_BUTTON_TYPE_CPI_CYCLE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("CPI up"), leadr_BUTTON_TYPE_CPI_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("CPI down"), leadr_BUTTON_TYPE_CPI_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Multimedia"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Open player"), leadr_BUTTON_TYPE_OPEN_PLAYER, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Previous track"), leadr_BUTTON_TYPE_PREV_TRACK, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Next track"), leadr_BUTTON_TYPE_NEXT_TRACK, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Play/Pause"), leadr_BUTTON_TYPE_PLAY_PAUSE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Stop"), leadr_BUTTON_TYPE_STOP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Mute"), leadr_BUTTON_TYPE_MUTE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Volume up"), leadr_BUTTON_TYPE_VOLUME_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Volume down"), leadr_BUTTON_TYPE_VOLUME_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Profile"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Profile cycle"), leadr_BUTTON_TYPE_PROFILE_CYCLE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Profile up"), leadr_BUTTON_TYPE_PROFILE_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Profile down"), leadr_BUTTON_TYPE_PROFILE_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Sensitivity"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Sensitivity cycle"), leadr_BUTTON_TYPE_SENSITIVITY_CYCLE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Sensitivity up"), leadr_BUTTON_TYPE_SENSITIVITY_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Sensitivity down"), leadr_BUTTON_TYPE_SENSITIVITY_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Easywheel"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel Sensitivity"), leadr_BUTTON_TYPE_EASYWHEEL_SENSITIVITY, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel Profile"), leadr_BUTTON_TYPE_EASYWHEEL_PROFILE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel CPI"), leadr_BUTTON_TYPE_EASYWHEEL_CPI, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel Volume"), leadr_BUTTON_TYPE_EASYWHEEL_VOLUME, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel Alt Tab"), leadr_BUTTON_TYPE_EASYWHEEL_ALT_TAB, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easywheel Aero Flip 3D"), leadr_BUTTON_TYPE_EASYWHEEL_AERO_FLIP_3D, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Easyaim"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easyaim Setting 1"), leadr_BUTTON_TYPE_EASYAIM_1, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easyaim Setting 2"), leadr_BUTTON_TYPE_EASYAIM_2, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easyaim Setting 3"), leadr_BUTTON_TYPE_EASYAIM_3, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easyaim Setting 4"), leadr_BUTTON_TYPE_EASYAIM_4, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Easyaim Setting 5"), leadr_BUTTON_TYPE_EASYAIM_5, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Easyshift"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, &sublevel, &toplevel, _("Self"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &sublevel, _("Self Easyshift"), leadr_BUTTON_TYPE_EASYSHIFT_SELF, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, &sublevel, &toplevel, _("Other"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &sublevel, _("Other Easyshift"), leadr_BUTTON_TYPE_EASYSHIFT_OTHER, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &sublevel, _("Other Easyshift lock"), leadr_BUTTON_TYPE_EASYSHIFT_LOCK_OTHER, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, &sublevel, &toplevel, _("Both"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &sublevel, _("Both Easyshift"), leadr_BUTTON_TYPE_EASYSHIFT_ALL, ROCCAT_KEY_COMBO_BOX_GROUP_EASYSHIFT | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Timer"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Timer start"), leadr_BUTTON_TYPE_TIMER, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Timer stop"), leadr_BUTTON_TYPE_TIMER_STOP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("XInput"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 1"), leadr_BUTTON_TYPE_XINPUT_1, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 2"), leadr_BUTTON_TYPE_XINPUT_2, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 3"), leadr_BUTTON_TYPE_XINPUT_3, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 4"), leadr_BUTTON_TYPE_XINPUT_4, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 5"), leadr_BUTTON_TYPE_XINPUT_5, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 6"), leadr_BUTTON_TYPE_XINPUT_6, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 7"), leadr_BUTTON_TYPE_XINPUT_7, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 8"), leadr_BUTTON_TYPE_XINPUT_8, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 9"), leadr_BUTTON_TYPE_XINPUT_9, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput 10"), leadr_BUTTON_TYPE_XINPUT_10, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Rx up"), leadr_BUTTON_TYPE_XINPUT_RX_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Rx down"), leadr_BUTTON_TYPE_XINPUT_RX_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Ry up"), leadr_BUTTON_TYPE_XINPUT_RY_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Ry down"), leadr_BUTTON_TYPE_XINPUT_RY_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput X up"), leadr_BUTTON_TYPE_XINPUT_X_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput X down"), leadr_BUTTON_TYPE_XINPUT_X_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Y up"), leadr_BUTTON_TYPE_XINPUT_Y_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Y down"), leadr_BUTTON_TYPE_XINPUT_Y_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Z up"), leadr_BUTTON_TYPE_XINPUT_Z_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("XInput Z down"), leadr_BUTTON_TYPE_XINPUT_Z_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("DInput"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 1"), leadr_BUTTON_TYPE_DINPUT_1, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 2"), leadr_BUTTON_TYPE_DINPUT_2, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 3"), leadr_BUTTON_TYPE_DINPUT_3, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 4"), leadr_BUTTON_TYPE_DINPUT_4, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 5"), leadr_BUTTON_TYPE_DINPUT_5, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 6"), leadr_BUTTON_TYPE_DINPUT_6, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 7"), leadr_BUTTON_TYPE_DINPUT_7, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 8"), leadr_BUTTON_TYPE_DINPUT_8, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 9"), leadr_BUTTON_TYPE_DINPUT_9, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 10"), leadr_BUTTON_TYPE_DINPUT_10, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 11"), leadr_BUTTON_TYPE_DINPUT_11, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput 12"), leadr_BUTTON_TYPE_DINPUT_12, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput X up"), leadr_BUTTON_TYPE_DINPUT_X_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput X down"), leadr_BUTTON_TYPE_DINPUT_X_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput Y up"), leadr_BUTTON_TYPE_DINPUT_Y_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput Y down"), leadr_BUTTON_TYPE_DINPUT_Y_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput Z up"), leadr_BUTTON_TYPE_DINPUT_Z_UP, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("DInput Z down"), leadr_BUTTON_TYPE_DINPUT_Z_DOWN, ROCCAT_KEY_COMBO_BOX_GROUP_ANALOG);

	roccat_key_combo_box_store_append(store, &toplevel, NULL, _("Shortcuts"), ROCCAT_KEY_COMBO_BOX_TYPE_NONE, ROCCAT_KEY_COMBO_BOX_GROUP_NONE | leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Shortcut"), leadr_BUTTON_TYPE_SHORTCUT, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Windows key"), leadr_BUTTON_TYPE_WINDOWS_KEY, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Home"), leadr_BUTTON_TYPE_HOME, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("End"), leadr_BUTTON_TYPE_END, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Page up"), leadr_BUTTON_TYPE_PAGE_UP, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Page down"), leadr_BUTTON_TYPE_PAGE_DOWN, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Left ctrl"), leadr_BUTTON_TYPE_L_CTRL, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, &toplevel, _("Left alt"), leadr_BUTTON_TYPE_L_ALT, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, NULL, _("Macro"), leadr_BUTTON_TYPE_MACRO, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);
	roccat_key_combo_box_store_append(store, NULL, NULL, _("Quicklaunch"), leadr_BUTTON_TYPE_QUICKLAUNCH, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);
	roccat_key_combo_box_store_append(store, NULL, NULL, _("Open driver"), leadr_BUTTON_TYPE_OPEN_DRIVER, leadr_KEY_COMBO_BOX_GROUP_NOT_ANALOG);

	roccat_key_combo_box_store_append(store, NULL, NULL, _("Unused"), leadr_BUTTON_TYPE_UNUSED, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);
	roccat_key_combo_box_store_append(store, NULL, NULL, _("Disabled"), leadr_BUTTON_TYPE_DISABLED, ROCCAT_KEY_COMBO_BOX_GROUP_NONE);

	return GTK_TREE_MODEL(store);
}

static void finalize(GObject *object) {
	leadrKeyComboBoxPrivate *priv = leadr_KEY_COMBO_BOX(object)->priv;
	g_free(priv->key_info);
	G_OBJECT_CLASS(leadr_key_combo_box_parent_class)->finalize(object);
}

static void leadr_key_combo_box_class_init(leadrKeyComboBoxClass *klass) {
	GObjectClass *gobject_class = (GObjectClass*)klass;

	gobject_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(leadrKeyComboBoxPrivate));
	klass->model = button_info_model_new();
}

void leadr_key_combo_box_set_value_blocked(leadrKeyComboBox *key_combo_box, leadrRmpMacroKeyInfo const *key_info) {
	leadrKeyComboBoxPrivate *priv = key_combo_box->priv;

	leadr_rmp_macro_key_info_free(priv->key_info);
	priv->key_info = leadr_rmp_macro_key_info_dup(key_info);

	set_text(key_combo_box);
}

leadrRmpMacroKeyInfo *leadr_key_combo_box_get_value(leadrKeyComboBox *key_combo_box) {
	return leadr_rmp_macro_key_info_dup(key_combo_box->priv->key_info);
}
