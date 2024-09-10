#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <../headers/init.h>





static void ControlCisterna_task(void *arg)
{

   

   estado.nivelActual = 20000; ///TODO, quitar valores hardcoded
   estado.vertidoHora = 300; 
   
 

   while (1)
   {/// 1. Leo estado de los sensores 
   //2. Consulto los valores configurados por el usuario.
   //3. Calculo las acciones de control (Encendido, apagados de válvulas) 
   //4. Actualizo el estado del sistema. 
   
    /* code */

   printf("Nivel %d  vertidi %d \n", estado.nivelActual, estado.vertidoHora); 
    vTaskDelay(2000/ portTICK_PERIOD_MS);
   }
    
}; 



