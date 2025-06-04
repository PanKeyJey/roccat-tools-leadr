#ifndef __ROCCAT_leadr_CPI_SELECTOR_H__
#define __ROCCAT_leadr_CPI_SELECTOR_H__

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
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define leadr_CPI_SELECTOR_TYPE (leadr_cpi_selector_get_type())
#define leadr_CPI_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_CPI_SELECTOR_TYPE, leadrCpiSelector))
#define IS_leadr_CPI_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_CPI_SELECTOR_TYPE))

typedef struct _leadrCpiSelector leadrCpiSelector;

GType leadr_cpi_selector_get_type(void);
GtkWidget *leadr_cpi_selector_new(void);

void leadr_cpi_selector_set_from_rmp(leadrCpiSelector *selector, leadrRmp *rmp);
void leadr_cpi_selector_update_rmp(leadrCpiSelector *selector, leadrRmp *rmp);

G_END_DECLS

#endif
