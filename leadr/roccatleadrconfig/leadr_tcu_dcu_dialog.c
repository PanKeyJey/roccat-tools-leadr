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

#include "leadr_tcu_dcu_dialog.h"
#include "leadr_tcu_frame.h"
#include "leadr_dcu_frame.h"
#include "leadr_tcu_calibration_assistant.h"
#include "leadr_control_unit.h"
#include "roccat_warning_dialog.h"
#include <gaminggear/gaminggear_timeout_dialog.h>
#include "i18n.h"

#define leadr_TCU_DCU_DIALOG_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_TCU_DCU_DIALOG_TYPE, leadrSensorDcuDialogPrivate))
#define leadr_TCU_DCU_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_TCU_DCU_DIALOG_TYPE, leadrSensorDcuDialogClass))
#define IS_leadr_TCU_DCU_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_TCU_DCU_DIALOG_TYPE))

typedef struct _leadrSensorDcuDialogClass leadrSensorDcuDialogClass;
typedef struct _leadrSensorDcuDialogPrivate leadrSensorDcuDialogPrivate;

struct _leadrSensorDcuDialog {
	GtkDialog parent;
	leadrSensorDcuDialogPrivate *priv;
};

struct _leadrSensorDcuDialogClass {
	GtkDialogClass parent_class;
};

struct _leadrSensorDcuDialogPrivate {
	leadrSensorFrame *tcu;
	leadrDcuFrame *dcu;
	RoccatDevice *device;
};

G_DEFINE_TYPE(leadrSensorDcuDialog, leadr_tcu_dcu_dialog, GTK_TYPE_DIALOG);

static void calibration_cb(leadrTcuCalibrationAssistant *assistant, gpointer user_data) {
	leadrSensorFrame *frame = leadr_TCU_FRAME(user_data);
	guint tcu_value;

	tcu_value = leadr_tcu_calibration_assistant_get_tcu_value(assistant);
	leadr_tcu_frame_set_value_blocked(frame, tcu_value);

	gtk_widget_destroy(GTK_WIDGET(assistant));
}

static void tcu_changed_cb(leadrSensorFrame *frame, gpointer user_data) {
	leadrSensorDcuDialog *tcu_dcu_dialog = leadr_TCU_DCU_DIALOG(user_data);
	leadrSensorDcuDialogPrivate *priv = tcu_dcu_dialog->priv;
	GError *local_error = NULL;
	GtkWidget *assistant;
	guint tcu_value;
	guint dcu_value;

	tcu_value = leadr_tcu_frame_get_value(priv->tcu);
	dcu_value = leadr_dcu_frame_get_value(priv->dcu);

	if (tcu_value == leadr_TRACKING_CONTROL_UNIT_ON) {
		assistant = leadr_tcu_calibration_assistant_new(GTK_WINDOW(tcu_dcu_dialog), priv->device, dcu_value);
		g_signal_connect(G_OBJECT(assistant), "completed", G_CALLBACK(calibration_cb), frame);
		gtk_widget_show_all(GTK_WIDGET(assistant));
	} else {
		leadr_tracking_control_unit_off(priv->device, dcu_value, &local_error);
		roccat_handle_error_dialog(GTK_WINDOW(tcu_dcu_dialog), _("Could not access sensor data"), &local_error);
		leadr_tcu_frame_set_value_blocked(priv->tcu, leadr_TRACKING_CONTROL_UNIT_OFF);
	}
}

static void dcu_changed_cb(leadrDcuFrame *frame, gpointer user_data) {
	leadrSensorDcuDialog *tcu_dcu_dialog = leadr_TCU_DCU_DIALOG(user_data);
	leadrSensorDcuDialogPrivate *priv = tcu_dcu_dialog->priv;
	guint new_value;
	GError *local_error = NULL;
	guint old_value;
	gboolean result;

	new_value = leadr_dcu_frame_get_value(priv->dcu);

	old_value = leadr_distance_control_unit_get(priv->device, &local_error);
	if (!roccat_handle_error_dialog(GTK_WINDOW(tcu_dcu_dialog), _("Could not access sensor data"), &local_error))
		return;

	if (new_value == old_value)
		return;

	leadr_distance_control_unit_try(priv->device, new_value, &local_error);
	if (!roccat_handle_error_dialog(GTK_WINDOW(tcu_dcu_dialog), _("Could not access sensor data"), &local_error))
		return;

	result = gaminggear_timeout_dialog(GTK_WINDOW(tcu_dcu_dialog),
			_("Keep settings?"),
			_("Do you want to keep this settings?"), 20);

	if (result) {
		leadr_distance_control_unit_accept(priv->device, new_value, &local_error);
		leadr_dcu_frame_set_value_blocked(priv->dcu, new_value);
	} else {
		leadr_distance_control_unit_cancel(priv->device, old_value, &local_error);
		leadr_dcu_frame_set_value_blocked(priv->dcu, old_value);
	}
	roccat_handle_error_dialog(GTK_WINDOW(tcu_dcu_dialog), _("Could not access sensor data"), &local_error);
}

static void init_frames(leadrSensorDcuDialog *tcu_dcu_dialog) {
	leadrSensorDcuDialogPrivate *priv = tcu_dcu_dialog->priv;
	GError *local_error = NULL;
	leadrControlUnit *sensor;

	sensor = leadr_control_unit_read(priv->device, &local_error);
	if (!roccat_handle_error_dialog(GTK_WINDOW(tcu_dcu_dialog), _("Could not access sensor data"), &local_error))
		return;
	leadr_dcu_frame_set_value_blocked(priv->dcu, sensor->dcu);
	leadr_tcu_frame_set_value_blocked(priv->tcu, sensor->tcu);
}

GtkWidget *leadr_tcu_dcu_dialog_new(GtkWindow *parent, RoccatDevice *device) {
	leadrSensorDcuDialog *tcu_dcu_dialog;
	leadrSensorDcuDialogPrivate *priv;

	tcu_dcu_dialog = leadr_TCU_DCU_DIALOG(g_object_new(leadr_TCU_DCU_DIALOG_TYPE,
			"transient-for", parent,
			NULL));

	priv = tcu_dcu_dialog->priv;
	priv->device = device;

	init_frames(tcu_dcu_dialog);

	return GTK_WIDGET(tcu_dcu_dialog);
}

static void leadr_tcu_dcu_dialog_init(leadrSensorDcuDialog *tcu_dcu_dialog) {
	leadrSensorDcuDialogPrivate *priv = leadr_TCU_DCU_DIALOG_GET_PRIVATE(tcu_dcu_dialog);
	GtkVBox *content_area;

	tcu_dcu_dialog->priv = priv;

	gtk_dialog_add_buttons(GTK_DIALOG(tcu_dcu_dialog),
			GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_window_set_modal(GTK_WINDOW(tcu_dcu_dialog), TRUE);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(tcu_dcu_dialog), TRUE);
	gtk_window_set_title(GTK_WINDOW(tcu_dcu_dialog), _("TCU/DCU"));

	content_area = GTK_VBOX(gtk_dialog_get_content_area(GTK_DIALOG(tcu_dcu_dialog)));

	priv->tcu = leadr_TCU_FRAME(leadr_tcu_frame_new());
	priv->dcu = leadr_DCU_FRAME(leadr_dcu_frame_new());

	g_signal_connect(G_OBJECT(priv->tcu), "changed", G_CALLBACK(tcu_changed_cb), tcu_dcu_dialog);
	g_signal_connect(G_OBJECT(priv->dcu), "changed", G_CALLBACK(dcu_changed_cb), tcu_dcu_dialog);

	gtk_box_pack_start(GTK_BOX(content_area), GTK_WIDGET(priv->tcu), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(content_area), GTK_WIDGET(priv->dcu), FALSE, FALSE, 0);

	gtk_widget_show_all(GTK_WIDGET(content_area));
}

static void leadr_tcu_dcu_dialog_class_init(leadrSensorDcuDialogClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrSensorDcuDialogPrivate));
}
