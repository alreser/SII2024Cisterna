#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <../headers/init.h>





static void ControlCisterna_task(void *arg)
{


   while (1)
   {
    /* code */

    vTaskDelay(1000/ portTICK_PERIOD_MS);
   }
    
}; 



