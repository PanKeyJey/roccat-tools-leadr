#include "roccat_talk_target_combo_box.h"
#include "roccat_talk.h"
#include "tyon.h"
#include "gtk_roccat_helper.h"

#define ROCCAT_TALK_TARGET_COMBO_BOX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), ROCCAT_TALK_TARGET_COMBO_BOX_TYPE, RoccatTalkTargetComboBoxPrivate))

typedef struct _RoccatTalkTargetComboBoxClass {
	GtkComboBoxClass parent_class;
	GtkListStore *store;
} RoccatTalkTargetComboBoxClass;

struct _RoccatTalkTargetComboBoxPrivate {
	GtkTreeModel *model;
	guint source_talk_device_type;
};

enum {
	PROP_0,
	PROP_VALUE,
	PROP_SOURCE_TALK_DEVICE_TYPE,
};

enum {
	NAME_COLUMN,
	VALUE_COLUMN,
	N_COLUMNS,
};

G_DEFINE_TYPE(RoccatTalkTargetComboBox, roccat_talk_target_combo_box, GTK_TYPE_COMBO_BOX);

static GtkListStore *list_store_new(void) {
	GtkListStore *list_store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_UINT);

	gtk_roccat_list_store_append_with_values(list_store, NULL,
											 NAME_COLUMN, roccat_talk_device_get_text_static(ROCCAT_TALK_DEVICE_MOUSE),
											 VALUE_COLUMN, ROCCAT_TALK_DEVICE_MOUSE,
										  -1);

	gtk_roccat_list_store_append_with_values(list_store, NULL,
											 NAME_COLUMN, roccat_talk_device_get_text_static(USB_DEVICE_ID_ROCCAT_TYON_BLACK),
											 VALUE_COLUMN, USB_DEVICE_ID_ROCCAT_TYON_BLACK,
										  -1);

	gtk_roccat_list_store_append_with_values(list_store, NULL,
											 NAME_COLUMN, roccat_talk_device_get_text_static(USB_DEVICE_ID_ROCCAT_TYON_WHITE),
											 VALUE_COLUMN, USB_DEVICE_ID_ROCCAT_TYON_WHITE,
										  -1);

	return list_store;
}

static gboolean visible_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data) {
	RoccatTalkTargetComboBoxPrivate *priv = ROCCAT_TALK_TARGET_COMBO_BOX(user_data)->priv;
	guint target_talk_device;

	gtk_tree_model_get(model, iter, VALUE_COLUMN, &target_talk_device, -1);
	return roccat_talk_device_get_type(target_talk_device) != roccat_talk_device_get_type(priv->source_talk_device_type);
}

guint roccat_talk_target_combo_box_get_value(RoccatTalkTargetComboBox *talk_target_combo_box) {
	RoccatTalkTargetComboBoxPrivate *priv = talk_target_combo_box->priv;
	GtkTreeIter iter;
	gboolean valid;
	guint value;

	valid = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(talk_target_combo_box), &iter);
	if (!valid)
		return ROCCAT_TALK_DEVICE_MOUSE;

	gtk_tree_model_get(priv->model, &iter, VALUE_COLUMN, &value, -1);
	return value;
}

void roccat_talk_target_combo_box_set_value(RoccatTalkTargetComboBox *talk_target_combo_box, guint value) {
	RoccatTalkTargetComboBoxPrivate *priv = talk_target_combo_box->priv;
	GtkTreeIter iter;
	if (gtk_roccat_tree_model_iter_find_int(priv->model, VALUE_COLUMN, value, &iter, NULL))
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(talk_target_combo_box), &iter);
}

guint roccat_talk_target_combo_box_get_source_talk_device_type(RoccatTalkTargetComboBox *talk_target_combo_box) {
	return talk_target_combo_box->priv->source_talk_device_type;
}

void roccat_talk_target_combo_box_set_source_talk_device_type(RoccatTalkTargetComboBox *talk_target_combo_box, guint value) {
	RoccatTalkTargetComboBoxPrivate *priv = talk_target_combo_box->priv;
	priv->source_talk_device_type = value;
	if (priv->model)
		gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(priv->model));
}

GtkWidget *roccat_talk_target_combo_box_new(guint source_talk_device_type) {
	return GTK_WIDGET(g_object_new(ROCCAT_TALK_TARGET_COMBO_BOX_TYPE,
								   "source_talk_device_type", source_talk_device_type,
								NULL));
}

static void roccat_talk_target_combo_box_set_property(GObject *object, guint prop_id, GValue const *value, GParamSpec *pspec) {
	RoccatTalkTargetComboBox *combo_box = ROCCAT_TALK_TARGET_COMBO_BOX(object);
	switch (prop_id) {
		case PROP_VALUE:
			roccat_talk_target_combo_box_set_value(combo_box, g_value_get_uint(value));
			break;
		case PROP_SOURCE_TALK_DEVICE_TYPE:
			roccat_talk_target_combo_box_set_source_talk_device_type(combo_box, g_value_get_uint(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
	}
}

static void roccat_talk_target_combo_box_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	RoccatTalkTargetComboBox *combo_box = ROCCAT_TALK_TARGET_COMBO_BOX(object);
	switch (prop_id) {
		case PROP_VALUE:
			g_value_set_uint(value, roccat_talk_target_combo_box_get_value(combo_box));
			break;
		case PROP_SOURCE_TALK_DEVICE_TYPE:
			g_value_set_uint(value, roccat_talk_target_combo_box_get_source_talk_device_type(combo_box));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
	}
}

static void roccat_talk_target_combo_box_init(RoccatTalkTargetComboBox *talk_target_combo_box) {
	RoccatTalkTargetComboBoxPrivate *priv = ROCCAT_TALK_TARGET_COMBO_BOX_GET_PRIVATE(talk_target_combo_box);
	RoccatTalkTargetComboBoxClass *klass = G_TYPE_INSTANCE_GET_CLASS(talk_target_combo_box, ROCCAT_TALK_TARGET_COMBO_BOX_TYPE, RoccatTalkTargetComboBoxClass);
	talk_target_combo_box->priv = priv;

	GtkCellRenderer *cell = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(talk_target_combo_box), cell, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(talk_target_combo_box), cell, "text", NAME_COLUMN, NULL);

	priv->model = gtk_tree_model_filter_new(GTK_TREE_MODEL(klass->store), NULL);
	gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(priv->model), visible_func, talk_target_combo_box, NULL);
	gtk_combo_box_set_model(GTK_COMBO_BOX(talk_target_combo_box), priv->model);
}

static void roccat_talk_target_combo_box_class_init(RoccatTalkTargetComboBoxClass *klass) {
	GObjectClass *gobject_class = (GObjectClass *)klass;
	gobject_class->set_property = roccat_talk_target_combo_box_set_property;
	gobject_class->get_property = roccat_talk_target_combo_box_get_property;

	g_type_class_add_private(klass, sizeof(RoccatTalkTargetComboBoxPrivate));
	klass->store = list_store_new();

	g_object_class_install_property(gobject_class, PROP_VALUE,
									g_param_spec_uint("value", "value", "Reads or sets value", 0, 0xffff, 0, G_PARAM_READWRITE));
	g_object_class_install_property(gobject_class, PROP_SOURCE_TALK_DEVICE_TYPE,
									g_param_spec_uint("source_talk_device_type", "source_talk_device_type", "Reads or sets source talk device type", 0, 0xffff, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}
