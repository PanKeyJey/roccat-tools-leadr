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

#include "leadr_tcu_frame.h"
#include "leadr_control_unit.h"
#include "i18n.h"

#define leadr_TCU_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_TCU_FRAME_TYPE, leadrSensorFrameClass))
#define IS_leadr_TCU_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_TCU_FRAME_TYPE))
#define leadr_TCU_FRAME_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_TCU_FRAME_TYPE, leadrSensorFramePrivate))

typedef struct _leadrSensorFrameClass leadrSensorFrameClass;
typedef struct _leadrSensorFramePrivate leadrSensorFramePrivate;

struct _leadrSensorFrame {
	GtkFrame parent;
	leadrSensorFramePrivate *priv;
};

struct _leadrSensorFrameClass {
	GtkFrameClass parent_class;
	void (*changed)(leadrSensorFrame *frame);
};

struct _leadrSensorFramePrivate {
	GtkToggleButton *active_button;
	GtkButton *cal_button;
	gulong cal_handler;
	gulong active_handler;
};

G_DEFINE_TYPE(leadrSensorFrame, leadr_tcu_frame, GTK_TYPE_FRAME);

enum {
	CHANGED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

guint leadr_tcu_frame_get_value(leadrSensorFrame *frame) {
	return (gtk_toggle_button_get_active(frame->priv->active_button) == TRUE) ? leadr_TRACKING_CONTROL_UNIT_ON : leadr_TRACKING_CONTROL_UNIT_OFF;
}

void leadr_tcu_frame_set_value_blocked(leadrSensorFrame *frame, guint value) {
	leadrSensorFramePrivate *priv = frame->priv;
	g_signal_handler_block(G_OBJECT(priv->active_button), priv->active_handler);
	gtk_toggle_button_set_active(frame->priv->active_button, (value == leadr_TRACKING_CONTROL_UNIT_ON) ? TRUE : FALSE);
	g_signal_handler_unblock(G_OBJECT(priv->active_button), priv->active_handler);
}

GtkWidget *leadr_tcu_frame_new(void) {
	leadrSensorFrame *leadr_tcu_frame;

	leadr_tcu_frame = leadr_TCU_FRAME(g_object_new(leadr_TCU_FRAME_TYPE,
			NULL));

	return GTK_WIDGET(leadr_tcu_frame);
}

static void active_toggled_cb(GtkToggleButton *button, gpointer user_data) {
	leadrSensorFrame *frame = (leadrSensorFrame *)user_data;
	g_signal_emit((gpointer)frame, signals[CHANGED], 0);
}

static void cal_clicked_cb(GtkButton *button, gpointer user_data) {
	leadrSensorFrame *frame = (leadrSensorFrame *)user_data;
	leadr_tcu_frame_set_value_blocked(frame, leadr_TRACKING_CONTROL_UNIT_ON);
	g_signal_emit((gpointer)frame, signals[CHANGED], 0);
}

static void leadr_tcu_frame_init(leadrSensorFrame *frame) {
	leadrSensorFramePrivate *priv = leadr_TCU_FRAME_GET_PRIVATE(frame);
	GtkWidget *box;

	frame->priv = priv;

	box = gtk_hbox_new(FALSE, 0);
	priv->active_button = GTK_TOGGLE_BUTTON(gtk_check_button_new_with_label(_("Active")));
	priv->cal_button = GTK_BUTTON(gtk_button_new_with_label(_("(Re)calibrate")));

	gtk_container_add(GTK_CONTAINER(frame), box);
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(priv->active_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(priv->cal_button), FALSE, FALSE, 0);

	priv->active_handler = g_signal_connect(G_OBJECT(priv->active_button), "toggled", G_CALLBACK(active_toggled_cb), frame);
	priv->cal_handler = g_signal_connect(G_OBJECT(priv->cal_button), "clicked", G_CALLBACK(cal_clicked_cb), frame);

	gtk_frame_set_label(GTK_FRAME(frame), _("Tracking control unit"));

	gtk_widget_show_all(box);
}

static void leadr_tcu_frame_class_init(leadrSensorFrameClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrSensorFramePrivate));

	signals[CHANGED] = g_signal_new("changed",
			G_TYPE_FROM_CLASS(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			G_STRUCT_OFFSET(leadrSensorFrameClass, changed),
			NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}
