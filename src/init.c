#define  LV_USE_DRAW_SW_COMPLEX_GRADIENTS 1
#include <lvgl.h>
#include "src/draw/sw/lv_draw_sw_gradient.h"
#include <../headers/init.h>


/// @brief Funcion que recibe un objeto Boton y le aplica estilos predefinidos para toda la aplicacion
/// @param objBoton  Puntero al onbjeto Botón. 
/// @param LabelText Texto que se mostrará en el botón
/// @param PosicionX  Posición en pantalla en X  relativa al objeto Padre
/// @param PosicionY  Posición en pantalla en y  relativa al objeto Padre
void AplicarEstiloBoton(lv_obj_t * objBoton,  char * LabelText, int32_t PosicionX, int32_t PosicionY)
{
    lv_obj_t * btn = objBoton;
    lv_obj_set_style_bg_color(btn, grad_colors[0], 0);
    lv_obj_set_style_bg_grad_color(btn, grad_colors[1], 0);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_set_pos(btn, PosicionX, PosicionY);
    //lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, LabelText);
    lv_obj_center(label);
};

 void InicilizarComponentesPantalla()
{


// ESTILOS ///////// 
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, LV_FONT_MONTSERRAT_26);

    //Estilo Texto Normal
    lv_style_init(&style_text_Normal);
    lv_style_set_text_font(&style_text_Normal, LV_FONT_MONTSERRAT_24);
    lv_style_set_text_color(&style_text_Normal,  lv_color_hex(0x000000)); //Color Negro

    

    //Estilo Texto Apagado
    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);




//creo los elementos y estilos que se utilizarán en todas las pantallas de forma general .
//Botones
    /*lv_style_init(&StyleBtn);
    lv_gradient_init_stops(&StyleBtnLabel, grad_colors, NULL, NULL, sizeof(grad_colors) / sizeof(lv_color_t));
    lv_grad_linear_init(&linear_gradient_dsc, lv_pct(0), lv_pct(0), lv_pct(20), lv_pct(100), LV_GRAD_EXTEND_REFLECT);
    lv_style_set_bg_grad(&StyleBtn, &StyleBtnLabel);
    lv_style_set_bg_opa(&StyleBtn, LV_OPA_COVER);
*/



};

