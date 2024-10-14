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





#ifdef __cplusplus
}
#endif