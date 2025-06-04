#ifndef __ROCCAT_leadr_PROFILE_PAGE_H__
#define __ROCCAT_leadr_PROFILE_PAGE_H__

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

#include "roccat_profile_page.h"
#include "leadr_rmp.h"

G_BEGIN_DECLS

#define leadr_PROFILE_PAGE_TYPE (leadr_profile_page_get_type())
#define leadr_PROFILE_PAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_PROFILE_PAGE_TYPE, leadrProfilePage))
#define IS_leadr_PROFILE_PAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_PROFILE_PAGE_TYPE))

typedef struct _leadrProfilePage leadrProfilePage;

GType leadr_profile_page_get_type(void);
GtkWidget *leadr_profile_page_new(void);

void leadr_profile_page_set_rmp(leadrProfilePage *profile_page, leadrRmp *rmp);
leadrRmp *leadr_profile_page_get_rmp(leadrProfilePage *profile_page);

G_END_DECLS

#endif
