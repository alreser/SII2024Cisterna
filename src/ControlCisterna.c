#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs.h>
#include <../headers/init.h>


extern struct st_EstadoCisterna estado;
extern struct st_ParametrosConfiguracion configuracion;


 void ControlCisterna_task(void *arg)
{



   while (1)
   {/// 1. Leo estado de los sensores 
   //2. Consulto los valores configurados por el usuario.
   //3. Calculo las acciones de control (Encendido, apagados de válvulas) 
   //4. Actualizo el estado del sistema.  
   
    /* code */
   
   estado.nivelActual++; //Todo reemplazar por la lectura de los sensores de campo

   //TODO: QUITAR ESTE CODIGO DE PRUEBAS 
   if (estado.nivelActual == 32100) estado.nivelActual = 10000;  //TODO eliminar codigo basura
   
  

    //printf("Control Cisterna Task ->Nivel %d  puntero nivel %p \n", estado.nivelActual, &estado.nivelActual);  
    vTaskDelay(2000 / portTICK_PERIOD_MS); //lectura cada 2 segundos


   }
   
}; 

/// @brief Lee desde NVS los parametros de configuracion del sistema y el ultimo  estado del sistema que fue guardado.
/// @return  ESP_OK o alguna variante de  ESP_ERROR_*
esp_err_t PersistenciaLeer(){




   return ESP_OK;

}

/// @brief Guarda los valores actuales de Configuracion y estado del sistema en el almacen no volatil. NVS
/// @return ESP_OK o alguna variante de ESP_ERROR_*
esp_err_t PersistenciaGuardar(){




   return ESP_OK;
}
