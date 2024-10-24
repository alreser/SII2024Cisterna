#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <../headers/init.h>


extern struct st_EstadoCisterna estado;
extern struct st_ParametrosConfiguracion configuracion;

nvs_handle_t nvsHandle; 


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
int64_t automan , bombaencendida; 
//verifico si ya tengo inicilizada la persistencia y sino, llamo a iniciarla 
er = Persistencia_Init(); 

//Verifico si 
if (er != ESP_OK) return ESP_FAIL ;  //No se pudo inicializar la persistencia 

// Leo y cargo los valores en la estructura de Estado y configuracion del sistema 


// Persisto la estructura Configuracion 
if ( ESP_OK != nvs_get_i64(nvsHandle, "nivelMin", &configuracion.nivelMin ) ) {printf("Error al leer de configuracion.nivelMin desde nvs\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_get_i64(nvsHandle, "nivelMax", &configuracion.nivelMax ) ) {printf("Error al leer de configuracion.nivelMax desde nvs\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_get_i64(nvsHandle, "caudalMax", &configuracion.caudalMax ) ) {printf("Error al leer de configuracion.caudalMax desde nvs\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_get_i64(nvsHandle, "maxAdmitivo", &configuracion.maxAdmitivo ) ) {printf("Error al leer de configuracion.maxAdmitivo desde nvs\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_get_i64(nvsHandle, "Agotamiento", &configuracion.nivelAgotamiento ) ) {printf("Error al leer de configuracion.nivelAgotamiento desde nvs\n"); return ESP_FAIL;};


//Persisto la estructura Estado 
if ( ESP_OK != nvs_get_i64(nvsHandle, "bombaEncendida", &bombaencendida ) ) {printf("Error al leer de estado.bombaEncendida desde nvs\n"); return ESP_FAIL;}; 
estado.bombaEncendida = (bombaencendida == 1) ? true : false ; 
if ( ESP_OK != nvs_get_i64(nvsHandle, "modoAuto",  &automan ) ) {printf("Error al leer de estado.modoAuto desde nvs\n"); return ESP_FAIL;}; 
estado.modoAuto  = (automan == 1) ? true : false; 

if ( ESP_OK != nvs_get_i64(nvsHandle, "nivelActual", &estado.nivelActual ) ) {printf("Error al leer de estado.nivelActua desde nvsl\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_get_i64(nvsHandle, "vertidoHora", &estado.vertidoHora ) ) {printf("Error al leer de estado.bombaEncendida desde nvs\n"); return ESP_FAIL;}; 

 int64_t niactual; 
 nvs_get_i64(nvsHandle, "nivelActual", &niactual );
 printf("Leido de NVS nivel actual: %"PRId64"\n", niactual);
 printf("Asignado a  estado.nivelactual: %"PRId64"\n", estado.nivelActual );
 nvs_close(nvsHandle); //cierro 
 printf("Valores cargados desde nvs correctamente !!!\n");


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

//estado.nivelActual = 10100; //todo eliminar

// Persisto la estructura Configuracion 
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelMin", configuracion.nivelMin ) ) {printf("Error persistiendo connfiguracion.nivelMin\n"); return ESP_FAIL;}; 
nvs_commit(nvsHandle);
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelMax", configuracion.nivelMax ) ) {printf("Error persistiendo connfiguracion.nivelMax\n"); return ESP_FAIL;};
nvs_commit(nvsHandle);
if ( ESP_OK != nvs_set_i64(nvsHandle, "caudalMax", configuracion.caudalMax ) ) {printf("Error persistiendo connfiguracion.caudalMax\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_set_i64(nvsHandle, "maxAdmitivo", configuracion.maxAdmitivo ) ) {printf("Error persistiendo connfiguracion.maxAdmitivo\n"); return ESP_FAIL;};
if ( ESP_OK != nvs_set_i64(nvsHandle, "Agotamiento", configuracion.nivelAgotamiento ) ) {printf("Error persistiendo connfiguracion.nivelAgotamiento\n"); return ESP_FAIL;};


//Persisto la estructura Estado 
if ( ESP_OK != nvs_set_i64(nvsHandle, "bombaEncendida", estado.bombaEncendida ) ) {printf("Error persistiendo estado.bombaEncendida\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "modoAuto", estado.modoAuto ) ) {printf("Error persistiendo estado.modoAuto\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "nivelActual", estado.nivelActual ) ) {printf("Error persistiendo estado.nivelActual\n"); return ESP_FAIL;}; 
if ( ESP_OK != nvs_set_i64(nvsHandle, "vertidoHora", estado.vertidoHora ) ) {printf("Error persistiendo estado.bombaEncendida\n"); return ESP_FAIL;}; 



er = nvs_commit(nvsHandle); 
//printf((er != ESP_OK) ? "Error al commit NVS!\n" : "Commit hecho\n");

nvs_close(nvsHandle); 
nvs_flash_deinit();



   return ESP_OK;
}
