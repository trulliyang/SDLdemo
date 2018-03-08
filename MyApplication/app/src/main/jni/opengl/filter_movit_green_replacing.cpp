//
// Created by shiyang1 on 2018/2/26.
//

#include <framework/mlt.h>
#include <string.h>
#include <assert.h>

#include "filter_glsl_manager.h"
#include <movit/green_replacing_effect.h>

using namespace movit;

static int get_image( mlt_frame frame, uint8_t **image, mlt_image_format *format,
                      int *width, int *height, int writable )
{
    mlt_filter filter = (mlt_filter) mlt_frame_pop_service( frame );
    mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
    GlslManager::get_instance()->lock_service( frame );
    mlt_position position = mlt_filter_get_position( filter, frame );
    mlt_position length = mlt_filter_get_length2( filter, frame );
    double value = 0.0;
    
    value = mlt_properties_anim_get_double( properties, "greenSaturationMin0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenSaturationMin0", value );
    
    value = mlt_properties_anim_get_double( properties, "greenSaturationMax0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenSaturationMax0", value );
    
    value = mlt_properties_anim_get_double( properties, "greenValueMin0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenValueMin0", value );
    
    value = mlt_properties_anim_get_double( properties, "greenValueMax0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenValueMax0", value );
    
    value = mlt_properties_anim_get_double( properties, "greenHueMin0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMin0", value);
    
    value = mlt_properties_anim_get_double(properties, "greenHueMax0", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMax0", value );
    
    value = mlt_properties_anim_get_double( properties, "greenSaturationMin1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenSaturationMin1", value );
    
    value = mlt_properties_anim_get_double( properties, "greenSaturationMax1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenSaturationMax1", value );
    
    value = mlt_properties_anim_get_double( properties, "greenValueMin1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenValueMin1", value );
    
    value = mlt_properties_anim_get_double( properties, "greenValueMax1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenValueMax1", value );
    
    value = mlt_properties_anim_get_double( properties, "greenHueMin1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMin1", value);
    
    value = mlt_properties_anim_get_double(properties, "greenHueMax1", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMax1", value );
    
    value = mlt_properties_anim_get_double( properties, "threshold", position, length );
    mlt_properties_set_double( properties, "_movit.parms.float.threshold", value);
    
    GlslManager::get_instance()->unlock_service( frame );
    *format = mlt_image_glsl;
    int error = mlt_frame_get_image( frame, image, format, width, height, writable );
    GlslManager::set_effect_input( MLT_FILTER_SERVICE( filter ), frame, (mlt_service) *image );
    GlslManager::set_effect( MLT_FILTER_SERVICE( filter ), frame, new GreenReplacingEffect() );
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

mlt_filter filter_movit_alpha_matting_init( mlt_profile profile, mlt_service_type type,
                                         const char *id, char *arg )
{
    mlt_filter filter = NULL;
    GlslManager* glsl = GlslManager::get_instance();

    if ( glsl && ( filter = mlt_filter_new() ) ) {
        mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
        glsl->add_ref( properties );
        filter->process = process;

        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "graySaturationMin", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "graySaturationMax", 0.3f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "grayValueMin", 0.25f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "grayValueMax", 0.8f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "greenHueMin", 90.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "greenHueMax", 180.5f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "threshold", 0.0f );
    }
    return filter;
}

}

