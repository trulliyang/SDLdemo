//
// Created by shiyang1 on 2018/2/11.
//

#include <framework/mlt.h>
#include <string.h>
#include <assert.h>

#include "filter_glsl_manager.h"
#include <movit/chroma_key_effect.h>

using namespace movit;

static int get_image( mlt_frame frame, uint8_t **image, mlt_image_format *format,
                      int *width, int *height, int writable )
{
    mlt_filter filter = (mlt_filter) mlt_frame_pop_service( frame );
    mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
    GlslManager::get_instance()->lock_service( frame );
    mlt_position position = mlt_filter_get_position( filter, frame );
    mlt_position length = mlt_filter_get_length2( filter, frame );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColor_r",
                               mlt_properties_anim_get_double( properties, "targetColor_r", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColor_g",
                               mlt_properties_anim_get_double( properties, "targetColor_g", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColor_b",
                               mlt_properties_anim_get_double( properties, "targetColor_b", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.thresholdSensitivity",
                               mlt_properties_anim_get_double( properties, "thresholdSensitivity", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.smoothing",
                               mlt_properties_anim_get_double( properties, "smoothing", position, length ) );
    GlslManager::get_instance()->unlock_service( frame );
    *format = mlt_image_glsl;
    int error = mlt_frame_get_image( frame, image, format, width, height, writable );
    GlslManager::set_effect_input( MLT_FILTER_SERVICE( filter ), frame, (mlt_service) *image );
    GlslManager::set_effect( MLT_FILTER_SERVICE( filter ), frame, new ChromaKeyEffect() );
    *image = (uint8_t *) MLT_FILTER_SERVICE( filter );
    return error;
}

static mlt_frame process( mlt_filter filter, mlt_frame frame )
{
    mlt_frame_push_service( frame, filter );
    mlt_frame_push_get_image( frame, get_image );
    return frame;
}

extern "C" {

mlt_filter filter_movit_chroma_key_init( mlt_profile profile, mlt_service_type type,
                                         const char *id, char *arg )
{
    mlt_filter filter = NULL;
    GlslManager* glsl = GlslManager::get_instance();

    if ( glsl && ( filter = mlt_filter_new() ) ) {
        mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
        glsl->add_ref( properties );
        filter->process = process;
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColor_r", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColor_g", 1.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColor_b", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "thresholdSensitivity", 0.3f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "smoothing", 0.1f );
    }
    return filter;
}

}

