#include <stdio.h>
#include <lvgl.h>
#ifdef __cplusplus
extern "C" {
#endif


void PantallaPrincipal();
static void CrearTabOperacion(lv_obj_t * parent);
static void CrearTabConfiguracion(lv_obj_t * parent);
void ActualizaValoresPantallas_task(void *arg);
void ActualizarValoresTabOperacion();
////TECLADO NUMERICO //////////////////////
  /*Create an AZERTY keyboard map*/
   // static const char * kb_map[] = {"1", "2", "3","\n", "4", "5", "6", "\n",  "7", "8", "9", "\n", LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, NULL };

    /*Set the relative width of the buttons and other controls*/
   // static const lv_buttonmatrix_ctrl_t kb_ctrl[] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

    //static  lv_obj_t * kbNumeric;// puntero al teclado numerico.
////// TECLADO NUMERICO ///////////////




#ifdef __cplusplus
}
#endif