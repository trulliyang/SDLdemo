/*
 * filter_movit_resize.cpp
 * Copyright (C) 2013 Dan Dennedy <dan@dennedy.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <framework/mlt.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "filter_glsl_manager.h"
#include <movit/init.h>
#include <movit/overlay_effect.h>
#include <android/log.h>
#include "optional_effect.h"

using namespace movit;


static int get_image( mlt_frame frame, uint8_t **image, mlt_image_format *format,
                      int *width, int *height, int writable )
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang",
                        "shiyang filter_movit_overlay get_image w=%d, h=%d", *width, *height);
	mlt_filter filter = (mlt_filter) mlt_frame_pop_service( frame );
	*format = mlt_image_glsl;
	int error = mlt_frame_get_image( frame, image, format, width, height, writable );
	GlslManager::set_effect_input( MLT_FILTER_SERVICE( filter ), frame, (mlt_service) *image );
	GlslManager::set_effect( MLT_FILTER_SERVICE( filter ), frame, new OverlayEffect );
	*image = (uint8_t *) MLT_FILTER_SERVICE( filter );
	return error;
}

static mlt_frame process( mlt_filter filter, mlt_frame frame )
{
//	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang filter_movit_resize process");
	mlt_frame_push_service( frame, filter );
	mlt_frame_push_get_image( frame, get_image );
	return frame;
}

extern "C"
mlt_filter filter_movit_overlay_init( mlt_profile profile, mlt_service_type type, const char *id, char *arg )
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang",
                        "shiyang filter_movit_resize filter_movit_resize_init");
	mlt_filter filter = NULL;
	GlslManager* glsl = GlslManager::get_instance();

	if ( glsl && ( filter = mlt_filter_new() ) ) {
		mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
		glsl->add_ref( properties );
		filter->process = process;
	}
	return filter;
}
