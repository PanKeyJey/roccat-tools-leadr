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

#include "leadr_dcu_frame.h"
#include "leadr_control_unit.h"
#include "gtk_roccat_helper.h"
#include "i18n.h"

#define leadr_DCU_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_DCU_FRAME_TYPE, leadrDcuFrameClass))
#define IS_leadr_DCU_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_DCU_FRAME_TYPE))
#define leadr_DCU_FRAME_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_DCU_FRAME_TYPE, leadrDcuFramePrivate))

typedef struct _leadrDcuFrameClass leadrDcuFrameClass;
typedef struct _leadrDcuFramePrivate leadrDcuFramePrivate;

struct _leadrDcuFrame {
	GtkFrame parent;
	leadrDcuFramePrivate *priv;
};

struct _leadrDcuFrameClass {
	GtkFrameClass parent_class;
	void (*changed)(leadrDcuFrame *frame, guint value);
};

struct _leadrDcuFramePrivate {
	GSList *radios;
};

G_DEFINE_TYPE(leadrDcuFrame, leadr_dcu_frame, GTK_TYPE_FRAME);

enum {
	CHANGED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };
static gchar const * const state_key = "State";

static void radio_toggled_cb(GtkToggleButton *radio, gpointer user_data) {
	leadrDcuFrame *frame = (leadrDcuFrame *)user_data;

	if (gtk_toggle_button_get_active(radio))
		g_signal_emit((gpointer)frame, signals[CHANGED], 0);
}

void leadr_dcu_frame_set_value_blocked(leadrDcuFrame *frame, guint new_value) {
	GSList *child;
	guint value;

	for (child = frame->priv->radios; child; child = g_slist_next(child)) {
		value = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(child->data), state_key));
		if (value == new_value) {
			g_signal_handlers_block_by_func(G_OBJECT(child->data), radio_toggled_cb, frame);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE);
			g_signal_handlers_unblock_by_func(G_OBJECT(child->data), radio_toggled_cb, frame);
			break;
		}

	}
}

guint leadr_dcu_frame_get_value(leadrDcuFrame *frame) {
	GtkWidget *active = gtk_roccat_radio_button_group_get_active(frame->priv->radios);
	if (active)
		return GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(active), state_key));
	else
		return 0;
}

GtkWidget *leadr_dcu_frame_new(void) {
	leadrDcuFrame *leadr_dcu_frame;

	leadr_dcu_frame = leadr_DCU_FRAME(g_object_new(leadr_DCU_FRAME_TYPE,
			NULL));

	return GTK_WIDGET(leadr_dcu_frame);
}

static void pack_radio_button(gpointer button, gpointer parent) {
	gtk_box_pack_start(parent, button, TRUE, TRUE, 0);
}

static void leadr_dcu_frame_init(leadrDcuFrame *frame) {
	leadrDcuFramePrivate *priv = leadr_DCU_FRAME_GET_PRIVATE(frame);
	GtkWidget *box;
	GtkWidget *radio;
	int i;

	frame->priv = priv;

	box = gtk_vbox_new(FALSE, 0);

	gtk_container_add(GTK_CONTAINER(frame), box);

	struct {
		leadrControlUnitDcu state;
		gchar const *title;
	} radios[4] = {
		{leadr_DISTANCE_CONTROL_UNIT_OFF, N_("Off")},
		{leadr_DISTANCE_CONTROL_UNIT_EXTRA_LOW, N_("Extra low")},
		{leadr_DISTANCE_CONTROL_UNIT_LOW, N_("Low")},
		{leadr_DISTANCE_CONTROL_UNIT_NORMAL, N_("Normal")},
	};

	priv->radios = NULL;

	for (i = 3; i >= 0; --i) {
		radio = gtk_radio_button_new_with_label(priv->radios, _N(radios[i].title));
		g_object_set_data(G_OBJECT(radio), state_key, GUINT_TO_POINTER(radios[i].state));
		priv->radios = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));
		g_signal_connect(G_OBJECT(radio), "toggled", G_CALLBACK(radio_toggled_cb), frame);
	}

	g_slist_foreach(priv->radios, pack_radio_button, box);

	gtk_frame_set_label(GTK_FRAME(frame), _("Distance control unit"));

	gtk_widget_show_all(box);
}

static void leadr_dcu_frame_class_init(leadrDcuFrameClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrDcuFramePrivate));

	signals[CHANGED] = g_signal_new("changed",
			G_TYPE_FROM_CLASS(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			G_STRUCT_OFFSET(leadrDcuFrameClass, changed),
			NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}
