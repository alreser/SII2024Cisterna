#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <../headers/init.h>





static void ControlCisterna_task(void *arg)
{


   while (1)
   {/// 1. Leo estado de los sensores 
   //2. Consulto los valores configurados por el usuario.
   //3. Calculo las acciones de control (Encendido, apagados de válvulas) 
   //4. Actualizo el estado del sistema. 
   
    /* code */

    vTaskDelay(1000/ portTICK_PERIOD_MS);
   }
    
}; 



