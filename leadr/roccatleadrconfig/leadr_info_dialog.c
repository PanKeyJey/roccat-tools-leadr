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

#include "leadr_info_dialog.h"
#include "roccat_warning_dialog.h"
#include "leadr_info.h"
#include "roccat_firmware.h"
#include "i18n.h"

#define leadr_INFO_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_INFO_DIALOG_TYPE, leadrInfoDialogClass))
#define IS_leadr_INFO_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_INFO_DIALOG_TYPE))
#define leadr_INFO_DIALOG_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_INFO_DIALOG_TYPE, leadrInfoDialogPrivate))

typedef struct _leadrInfoDialogClass leadrInfoDialogClass;

struct _leadrInfoDialogClass {
	RoccatInfoDialogClass parent_class;
};

struct _leadrInfoDialogPrivate {
	GtkLabel *firmware_label;
	GtkLabel *dfu_label;
};

G_DEFINE_TYPE(leadrInfoDialog, leadr_info_dialog, ROCCAT_INFO_DIALOG_TYPE);

static void set_firmware(leadrInfoDialog *info_dialog, RoccatDevice *device) {
	leadrInfoDialogPrivate *priv = info_dialog->priv;
	GError *local_error = NULL;
	leadrInfo *info;
	gchar *firmware;
	gchar *dfu;

	info = leadr_info_read(device, &local_error);
	if (!roccat_handle_error_dialog(GTK_WINDOW(info_dialog), _("Could not read informations"), &local_error)) {
		gtk_label_set_text(priv->firmware_label, NULL);
		return;
	}

	firmware = roccat_firmware_version_to_string(info->firmware_version);
	gtk_label_set_text(priv->firmware_label, firmware);
	g_free(firmware);

	dfu = roccat_firmware_version_to_string(info->dfu_version);
	gtk_label_set_text(priv->dfu_label, dfu);
	g_free(dfu);

	g_free(info);
}

GtkWidget *leadr_info_dialog_new(GtkWindow *parent, RoccatDevice *device) {
	leadrInfoDialog *info_dialog;

	info_dialog = leadr_INFO_DIALOG(g_object_new(leadr_INFO_DIALOG_TYPE,
			"transient-for", parent,
			NULL));

	set_firmware(info_dialog, device);

	return GTK_WIDGET(info_dialog);
}

static void leadr_info_dialog_init(leadrInfoDialog *info_dialog) {
	leadrInfoDialogPrivate *priv = leadr_INFO_DIALOG_GET_PRIVATE(info_dialog);
	info_dialog->priv = priv;

	priv->firmware_label = GTK_LABEL(gtk_label_new(NULL));
	priv->dfu_label = GTK_LABEL(gtk_label_new(NULL));

	roccat_info_dialog_add_line(ROCCAT_INFO_DIALOG(info_dialog), _("Firmware version"), GTK_WIDGET(priv->firmware_label));
	roccat_info_dialog_add_line(ROCCAT_INFO_DIALOG(info_dialog), _("DFU version"), GTK_WIDGET(priv->dfu_label));
}

static void leadr_info_dialog_class_init(leadrInfoDialogClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrInfoDialogPrivate));
}
