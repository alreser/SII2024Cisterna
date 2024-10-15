#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <../headers/init.h>


extern struct st_EstadoCisterna estado;
extern struct st_ParametrosConfiguracion configuracion;

nvs_handle_t nvsHandle = NULL; 


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


/// @brief Inicializo la persistencia de datos en NVS del Esp32
esp_err_t Persistencia_Init(){

esp_err_t er; 

er = nvs_flash_init(); 
if (er == ESP_ERR_NVS_NO_FREE_PAGES || er == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS particion truncada. Debo eliminarla
        // Reintento nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        er = nvs_flash_init();
    }
    ESP_ERROR_CHECK( er );

   er = nvs_open("cisterna", NVS_READWRITE, &nvsHandle) ;
   if ( er != ESP_OK) {
      printf("Error al abrir el NVS namespace[cisterna]\n") ; 
      return ESP_FAIL;
   }
   printf("Se abrio el almacenamiento namespace [cisterna]\n ");
   return ESP_OK; 

}




/// @brief Lee desde NVS los parametros de configuracion del sistema y el ultimo  estado del sistema que fue guardado.
/// @return  ESP_OK o alguna variante de  ESP_ERROR_*
esp_err_t PersistenciaLeer(){

esp_err_t er; 
//verifico si ya tengo inicilizada la persistencia y sino, llamo a iniciarla 
er = Persistencia_Init(); 

//Verifico si 
if (er != ESP_OK) return ESP_FAIL ;  //No se pudo inicializar la persistencia 

// Leo y cargo los valores en la estructura de Estado y configuracion del sistema 
//nvs_get_i64(nvsHandle, )



   return ESP_OK;

}

/// @brief Guarda los valores actuales de Configuracion y estado del sistema en el almacen no volatil. NVS
/// @return ESP_OK o alguna variante de ESP_ERROR_*
esp_err_t PersistenciaGuardar(){

esp_err_t er; 
//verifico si ya tengo inicilizada la persistencia y sino, llamo a iniciarla 
if (ESP_OK != Persistencia_Init()) return ESP_FAIL ; //No se pudo inicializar la persistencia   

  

//Persisto los valores 

//TODO: Verificar si tengo la estructura inicializada con valores de estado y configuracion 

//TODO: Eliminar. A modo de prueba antes de persisitir modifico los valor de maxAdminitido = 989898 


configuracion.maxAdmitivo = 989898; 
// Persisto la estructura Configuracion 
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelMin", configuracion.nivelMin ) ) {printf("Error persistiendo connfiguracion.nivelMin\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelMax", configuracion.nivelMax ) ) {printf("Error persistiendo connfiguracion.nivelMax\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_set_i64(nvsHandle, "caudalMax", configuracion.caudalMax ) ) {printf("Error persistiendo connfiguracion.caudalMax\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_set_i64(nvsHandle, "maxAdmitivo", configuracion.maxAdmitivo ) ) {printf("Error persistiendo connfiguracion.maxAdmitivo\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelAgotamiento", configuracion.nivelAgotamiento ) ) {printf("Error persistiendo connfiguracion.nivelAgotamiento\n"); return ESP_FAIL;};


//Persisto la estructura Estado 
if ( ESP_OK != nvs_set_i64(nvsHandle, "bombaEncendida", estado.bombaEncendida ) ) {printf("Error persistiendo estado.bombaEncendida\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "modoAuto", estado.modoAuto ) ) {printf("Error persistiendo estado.modoAuto\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelActual", estado.nivelActual ) ) {printf("Error persistiendo estado.nivelActual\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "vertidoHora", estado.vertidoHora ) ) {printf("Error persistiendo estado.bombaEncendida\n"); return ESP_FAIL;}; 



er = nvs_commit(nvsHandle); 
printf((er != ESP_OK) ? "Error al commit NVS!\n" : "Commit hecho\n");

nvs_close(nvsHandle); 



   return ESP_OK;
}
