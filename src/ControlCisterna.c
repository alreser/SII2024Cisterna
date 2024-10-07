#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#include <../headers/init.h>

extern struct st_EstadoCisterna estado;

 void ControlCisterna_task(void *arg)
{



   while (1)
   {/// 1. Leo estado de los sensores 
   //2. Consulto los valores configurados por el usuario.
   //3. Calculo las acciones de control (Encendido, apagados de válvulas) 
   //4. Actualizo el estado del sistema.  
   
    /* code */
   
   estado.nivelActual++;

   //TODO: QUITAR ESTE CODIGO DE PRUEBAS 
   if (estado.nivelActual == 32100) estado.nivelActual = 10000; 
   
   //estado.nivelActual+=100;

    printf("Control Cisterna Task ->Nivel %d  puntero nivel %p \n", estado.nivelActual, &estado.nivelActual); 
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    ActualizarValoresTabOperacion();
   }
   
}; 



