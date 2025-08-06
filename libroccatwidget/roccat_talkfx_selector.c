#include "roccat_talkfx_selector.h"
#include "gtk_roccat_helper.h"

G_DEFINE_TYPE(RoccatTalkFXSelector, roccat_talkfx_selector, GTK_TYPE_TOGGLE_BUTTON)

struct _RoccatTalkFXSelectorPrivate {
	GtkWidget *label;
};

static void roccat_talkfx_selector_class_init(RoccatTalkFXSelectorClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	// Brak specjalnych metod w tym uproszczonym modelu
}

static void roccat_talkfx_selector_init(RoccatTalkFXSelector *selector) {
	selector->priv = g_new0(RoccatTalkFXSelectorPrivate, 1);

	selector->priv->label = gtk_label_new("Tyon TalkFX selector initialized");
	gtk_container_add(GTK_CONTAINER(selector), selector->priv->label);
	gtk_widget_show(selector->priv->label);
}

GtkWidget *roccat_talkfx_selector_new(void) {
	return g_object_new(ROCCAT_TALKFX_SELECTOR_TYPE, NULL);
}

void roccat_talkfx_selector_set_active(RoccatTalkFXSelector *selector, gboolean active) {
	g_return_if_fail(ROCCAT_IS_TALKFX_SELECTOR(selector));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(selector), active);
}

gboolean roccat_talkfx_selector_get_active(RoccatTalkFXSelector *selector) {
	g_return_val_if_fail(ROCCAT_IS_TALKFX_SELECTOR(selector), FALSE);
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(selector));
}
