#define  LV_USE_DRAW_SW_COMPLEX_GRADIENTS 1
#include <lvgl.h>
#include "src/draw/sw/lv_draw_sw_gradient.h"
#include <../headers/init.h>

extern struct st_EstadoCisterna estado;
extern struct st_ControlesPantallaOperacion ControlesPantallaOperacion;


/// @brief Funcion que recibe un objeto Boton y le aplica estilos predefinidos para toda la aplicacion
/// @param objBoton  Puntero al onbjeto Botón. 
/// @param LabelText Texto que se mostrará en el botón
/// @param PosicionX  Posición en pantalla en X  relativa al objeto Padre
/// @param PosicionY  Posición en pantalla en y  relativa al objeto Padre
void BotonAplicarEstilo(lv_obj_t * objBoton,  char * LabelText, int32_t PosicionX, int32_t PosicionY)
{
    lv_obj_t * btn = objBoton;

    lv_obj_set_style_bg_color(btn, lv_color_hex(0x348b50), 0); //color verde
    lv_obj_set_style_bg_grad_color(btn, lv_color_hex(0x389c58), 0);  
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_size(btn, 110, 45);
    lv_obj_set_pos(btn, PosicionX, PosicionY);
    //lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, LabelText);
    lv_obj_center(label);
};


/// @brief Función que recibe un obj de tipo TextArea(LVGL) y le aplica los estilos predefinidos de la aplicación
/// @param objTextBox Puntero al objeto TextArea
/// @param Editable   Indica si será editable por el usuario y el tipo de teclado a presentar para el ingreso de datos. S 
/// @param ValorDefecto  Es el valor que se mostrará por defecto y podrá ser editado si Editable=True
/// @param PosicionX Posición en pantalla en X  relativa al objeto Padre
/// @param PosicionY Posición en pantalla en y  relativa al objeto Padre
void TextBoxAplicarEstilo(lv_obj_t * objTextBox, enum TextBoxEdicionEnum Editable,   char * ValorDefecto, int32_t PosicionX, int32_t PosicionY)
{

//lv_textarea_add_text(objTextBox, ValorDefecto); //Añado el valor por defecto .
//Si Editable = True hay que habilitar el teclado flotante para ingreso de datos. 



};


 void InicilizarComponentesPantalla()
{



// ESTILOS ///////// 
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, LV_FONT_MONTSERRAT_46);
    lv_style_set_text_color(&style_title, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_text_color(&style_title, lv_color_hex(0x4509f2)); //azul

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

 