//
// Created by shiyang1 on 2018/2/13.
//

#include <framework/mlt.h>
#include <string.h>
#include <assert.h>

#include "filter_glsl_manager.h"
#include <movit/alpha_matting_effect.h>

using namespace movit;

static int get_image( mlt_frame frame, uint8_t **image, mlt_image_format *format,
                      int *width, int *height, int writable )
{
    mlt_filter filter = (mlt_filter) mlt_frame_pop_service( frame );
    mlt_properties properties = MLT_FILTER_PROPERTIES( filter );
    GlslManager::get_instance()->lock_service( frame );
    mlt_position position = mlt_filter_get_position( filter, frame );
    mlt_position length = mlt_filter_get_length2( filter, frame );
    
    
    mlt_properties_set_double( properties, "_movit.parms.float.blackValueMin",
                               mlt_properties_anim_get_double( properties, "blackValueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.blackValueMax",
                               mlt_properties_anim_get_double( properties, "blackValueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.graySaturationMin",
                               mlt_properties_anim_get_double( properties, "graySaturationMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.graySaturationMax",
                               mlt_properties_anim_get_double( properties, "graySaturationMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.grayValueMin",
                               mlt_properties_anim_get_double( properties, "grayValueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.grayValueMax",
                               mlt_properties_anim_get_double( properties, "grayValueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.whiteSaturationMin",
                               mlt_properties_anim_get_double( properties, "whiteSaturationMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.whiteSaturationMax",
                               mlt_properties_anim_get_double( properties, "whiteSaturationMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.whiteValueMin",
                               mlt_properties_anim_get_double( properties, "whiteValueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.whiteValueMax",
                               mlt_properties_anim_get_double( properties, "whiteValueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.redHueMin",
                               mlt_properties_anim_get_double( properties, "redHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.redHueMax",
                               mlt_properties_anim_get_double( properties, "redHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.orangeHueMin",
                               mlt_properties_anim_get_double( properties, "orangeHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.orangeHueMax",
                               mlt_properties_anim_get_double( properties, "orangeHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.yellowHueMin",
                               mlt_properties_anim_get_double( properties, "yellowHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.yellowHueMax",
                               mlt_properties_anim_get_double( properties, "yellowHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMin",
                               mlt_properties_anim_get_double( properties, "greenHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.greenHueMax",
                               mlt_properties_anim_get_double( properties, "greenHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.cyanHueMin",
                               mlt_properties_anim_get_double( properties, "cyanHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.cyanHueMax",
                               mlt_properties_anim_get_double( properties, "cyanHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.blueHueMin",
                               mlt_properties_anim_get_double( properties, "blueHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.blueHueMax",
                               mlt_properties_anim_get_double( properties, "blueHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.purpleHueMin",
                               mlt_properties_anim_get_double( properties, "purpleHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.purpleHueMax",
                               mlt_properties_anim_get_double( properties, "purpleHueMax", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.pinkHueMin",
                               mlt_properties_anim_get_double( properties, "pinkHueMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.pinkHueMax",
                               mlt_properties_anim_get_double( properties, "pinkHueMax", position, length ) );
    // new
    mlt_properties_set_double( properties, "_movit.parms.float.targetColorRed",
                               mlt_properties_anim_get_double( properties, "targetColorRed", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColorGreen",
                               mlt_properties_anim_get_double( properties, "targetColorGreen", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColorBlue",
                               mlt_properties_anim_get_double( properties, "targetColorBlue", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.targetColorAlpha",
                               mlt_properties_anim_get_double( properties, "targetColorAlpha", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.diff",
                               mlt_properties_anim_get_double( properties, "diff", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.diffMin",
                               mlt_properties_anim_get_double( properties, "diffMin", position, length ) );
    mlt_properties_set_double( properties, "_movit.parms.float.diffMax",
                               mlt_properties_anim_get_double( properties, "diffMax", position, length ) );
    
    GlslManager::get_instance()->unlock_service( frame );
    *format = mlt_image_glsl;
    int error = mlt_frame_get_image( frame, image, format, width, height, writable );
    GlslManager::set_effect_input( MLT_FILTER_SERVICE( filter ), frame, (mlt_service) *image );
    GlslManager::set_effect( MLT_FILTER_SERVICE( filter ), frame, new AlphaMattingEffect() );
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
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "blackValueMin", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "blackValueMax", 0.25f );
    
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "graySaturationMin", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "graySaturationMax", 0.3f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "grayValueMin", 0.25f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "grayValueMax", 0.8f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "whiteSaturationMin", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "whiteSaturationMax", 0.3f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "whiteValueMin", 0.8f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "whiteValueMax", 1.0f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "redHueMin", 337.5f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "redHueMax", 22.5f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "orangeHueMin", 22.5f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "orangeHueMax", 45.0f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "yellowHueMin", 45.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "yellowHueMax", 90.0f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "greenHueMin", 90.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "greenHueMax", 205.5f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "cyanHueMin", 205.5f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "cyanHueMax", 228.0f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "blueHueMin", 228.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "blueHueMax", 292.5f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "purpleHueMin", 292.5f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "purpleHueMax", 337.5f );
        
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "pinkHueMin", 306.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "pinkHueMax", 324.0f );
    
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColorRed", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColorGreen", 1.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColorBlue", 0.0f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "targetColorAlpha", 1.0f );
    
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "diff", 0.1f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "diffMin", 0.1f );
        mlt_properties_set_double( MLT_FILTER_PROPERTIES(filter), "diffMax", 0.9f );
    }
    return filter;
}

}

