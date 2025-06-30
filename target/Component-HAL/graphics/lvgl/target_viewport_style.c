
#include "target_viewport_style.h"


lv_color_t ehs_gauge_color(ehs_uint16 part, ehs_uint16 custom_style){
    switch (part)
    {
    case GAUGE_SCALE_MAIN_ARC:{
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            return lv_palette_main(LV_PALETTE_GREY);
        }else{
            return lv_palette_main(LV_PALETTE_GREY);
        }
        break;
    }
    case GAUGE_SCALE_LEFT_ARC:{
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            static const lv_color_t color = LV_COLOR_MAKE(144,141,254);
            return color;
        }else{
            return lv_palette_main(LV_PALETTE_BLUE);
        }
        break;
    }
    case GAUGE_SCALE_RIGHT_ARC:{
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            static const lv_color_t color = LV_COLOR_MAKE(255,141,185);
            return color;
        }else{
            return lv_palette_main(LV_PALETTE_RED);
        }
        break;
    }
    case GAUGE_SCALE_NEEDLE:{
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            return lv_palette_main(LV_PALETTE_GREY);
        }else{
            return lv_palette_main(LV_PALETTE_GREY);
        }
        break;
    }
    default:
        break;
    }
    
    return lv_palette_main(LV_PALETTE_GREY);
}

lv_color_t ehs_chart_color(ehs_uint16 data_id, ehs_uint16 custom_style)
{
    if(data_id == EHS_CHART_DATA1_ID){
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            static const lv_color_t color = LV_COLOR_MAKE(60,120,216);
            return color;
        }else{
            return lv_palette_main(LV_PALETTE_RED);
        }
    }else if(data_id == EHS_CHART_DATA2_ID){
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            static const lv_color_t color = LV_COLOR_MAKE(95,145,72);
            return color;
        }else{
            return lv_palette_main(LV_PALETTE_BLUE);
        }
    }else if(data_id == EHS_CHART_DATA3_ID){
        if(custom_style == EHS_CUSTOM_STYLE_9_ID){
            static const lv_color_t color = LV_COLOR_MAKE(252,3,215);
            return color;
        }else{
            return lv_palette_main(LV_PALETTE_GREEN);
        }
    }
    return lv_palette_main(LV_PALETTE_GREY);
}