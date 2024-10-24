#include <../headers/pantalla.h>
#include <../headers/init.h>
#include <lvgl.h>

extern struct st_EstadoCisterna estado;
extern struct st_ParametrosConfiguracion configuracion;

extern bool lvgl_lock(int timeout_ms);
extern void lvgl_unlock(void);
extern esp_err_t PersistenciaGuardar();

TimerHandle_t timerPantalla; //Handler del timer que refresca la pantalla cada 500ms con los nuevos estados 

struct st_ControlesPantallaOperacion ControlesPantallaOperacion = {
    .pNivelTanque = NULL,
    .pConsumo = NULL,
    .pAutoMan = false,
    .pBomba = NULL
};
struct st_ControlesPantallaConfiguracion ControlesPantallaConfiguracion = {
    .pNivelMinimo = NULL,
    .pNivelMaximo = NULL,
    .pCaudalMaximo = NULL,
    .pMinimoAbsoluto = NULL,
    .pMaximoAbsoluto = NULL
} ;


lv_obj_t * tabOperacion; //Puntero a ibjeto TAB de valores de Operacion
lv_obj_t * tabConfiguracion;  //Puntero a TAB de Valores de configuracion del sistema
lv_obj_t * tabView; //Punto a TABVIEW principal de la aplicacion

lv_obj_t * kbNumeric;// puntero al teclado numerico.

//Actualiza los controles de la pantalla con los valores de estado del sistema de control de cisterna
void  ActualizarValoresTabOperacion()
{
       
   //printf("Entra en ActualizarValoresIoperaci\n");

    if (ControlesPantallaOperacion.pNivelTanque != NULL && ControlesPantallaOperacion.pConsumo != NULL)
    {   //Si el MUTEX  esta libre realizo actualizo los valores usando la libreria LVGL.  
       if ( lvgl_lock(-1))
       {
            char nivel[10];
            itoa(estado.nivelActual,nivel,10);
            lv_textarea_set_text( ControlesPantallaOperacion.pNivelTanque, nivel);


            char consumo[10];
            itoa(estado.vertidoHora ,consumo,10);
            lv_textarea_set_text( ControlesPantallaOperacion.pConsumo, consumo);

            if (estado.modoAuto) {
                // TRUE => Modo AUTO
                 lv_obj_add_state(ControlesPantallaOperacion.pAutoMan, LV_STATE_CHECKED);
                 }
                 else
                 {
                    //FALSE => Modo MAN
                    lv_obj_remove_state(ControlesPantallaOperacion.pAutoMan, LV_STATE_CHECKED); 
                    
                 }

             //TODO : Añadir indicacion de estado de operacion de la bomba [Encendida ! apagada]

            lvgl_unlock(); //Libero el mutex
       }
    // printf("Dentro de ActualizarValoresTabOperacion\n");

    }
   
};


static void keyboard_event_handler(lv_event_t * e){

	lv_obj_t * obj = lv_event_get_target(e);
	lv_keyboard_t * kb = (lv_keyboard_t *)obj;
	const char * txt = lv_keyboard_get_button_text(kb, lv_keyboard_get_selected_button(obj));

    //printf("Presiono el boton [%s]", txt);
	if(strcmp(txt, LV_SYMBOL_OK) == 0) {
		//printf("boton ok...");
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN); // Oculto el teclado ante el OK
		//lv_event_send(kb->ta, LV_EVENT_READY, NULL);
	};
}


/// @brief funcion de control de eventos de los TextArea
/// @param e 
static void ta_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb,ta); 
        lv_obj_remove_flag(kb,LV_OBJ_FLAG_HIDDEN); 
        //De acuardo al textarea que solicita el keyboard asigno la posicion en pantalla 
            if (lv_obj_get_x(ta) < (LCD_WIDTH/2)){
                //el control esta del lado izquierdo 
                //printf("Izq Posicion del control que llama [x,y]=[%" PRId32 "\n,%" PRId32 "\n]", lv_obj_get_x(ta), lv_obj_get_y(ta));
                lv_obj_set_pos(kbNumeric,180,-10); 
                 
            }
            else{
                // esta del lado derecho 
                //printf("der Posicion del control que llama [x,y]=[%" PRId32 "\n,%" PRId32 "\n]", lv_obj_get_x(ta), lv_obj_get_y(ta));
                lv_obj_set_pos(kbNumeric,-180,-10);
            }
       // lv_obj_set_pos(kbNumeric,200,200);

    }else if(code == LV_EVENT_DEFOCUSED){
         lv_keyboard_set_textarea(kb,NULL); 
        lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN); 

    }

}

//Evento que guarda la configuracion ingresada por el usuario 
static void btnGuardar_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e); //obtengo puntero al objeto que lanza el evento.
    if(code == LV_EVENT_CLICKED) {
        
    //TODO: Verificar y almacenar los valores introdicidos por el usuario en el NVS del esp32
    //1.cargo los valores de pantalla ingresados por el usuario en la estructura de Configuracion
    configuracion.nivelMin = (int64_t)atol(lv_textarea_get_text(ControlesPantallaConfiguracion.pNivelMinimo));
    configuracion.nivelMax = (int64_t)atol(lv_textarea_get_text(ControlesPantallaConfiguracion.pNivelMaximo));
    configuracion.caudalMax = (int64_t)atol(lv_textarea_get_text(ControlesPantallaConfiguracion.pCaudalMaximo));
    configuracion.nivelAgotamiento = (int64_t)atol(lv_textarea_get_text(ControlesPantallaConfiguracion.pMinimoAbsoluto));
    configuracion.maxAdmitivo = (int64_t)atol(lv_textarea_get_text(ControlesPantallaConfiguracion.pMaximoAbsoluto));

    //2. Guardo el estado actual tambien 
    //TODO. Cargo el estaado actual
      //2.1Asignar color segun estado de bomba Encendida=>Verde. Apagada=>Roja   //lv_led_set_color(ledBomba, lv_color_hex(0xff2816)); //lv_color_hex(0x43d319) <= verde -  Rojo => lv_color_hex(0xff2816)

     // 2.2 Asignar Modo de operacion bomba Auto/MAN Segun switch 


    //3. Persisto los valores en el nvs
     if (ESP_OK != PersistenciaGuardar()) 
        {printf("Error al Persistir Valores\n");}
        else
        {
            printf("Valores guardados Correctamente!!!\n");
            //Para solventar el error de LVGL que al utilizar NVS corre la pantalla y queda el tab cortado 
            // una vez almacenados los valores, elimino el objeto screen y lo vuelvo a cargar.
            lv_obj_delete(lv_screen_active());
            PantallaPrincipal();
        }
    
    }
};


static void automanual_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_STATE_CHECKED
        estado.modoAuto  = ( lv_obj_has_state(obj, LV_STATE_CHECKED)) ? true : false; 
        printf("estado del objeto %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off" );
        LV_UNUSED(obj);

        
    
    }
}


void PantallaPrincipal()
{
   
    InicilizarComponentesPantalla();
   
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x080808), LV_PART_MAIN);

    //Creo el componente TabView
   
    tabView = lv_tabview_create(lv_screen_active());
    //lv_tabview_set_tab_bar_size(tabView, LCD_WIDTH);
    //ATENCION: FALTA AGREGAR EVENTO AL TABVIEW
    //lv_obj_add_event_cb(tv, tabview_delete_event_cb, LV_EVENT_DELETE, NULL);
    //Agrego dos tabs al tabview
    
    //lv_obj_t * tabOperacion = lv_tabview_add_tab(tabView, "OPERACION");
    tabOperacion = lv_tabview_add_tab(tabView, "OPERACION");
   // lv_obj_t * tabConfiguracion = lv_tabview_add_tab(tabView, "CONFIGURACION");
    tabConfiguracion = lv_tabview_add_tab(tabView, "CONFIGURACION");
    
    //Creo la cabecera del TABView 
        lv_obj_t * tab_bar = lv_tabview_get_tab_bar(tabView);
        lv_obj_set_style_pad_left(tab_bar, LV_HOR_RES / 2, 0);
        //lv_obj_set_style_pad_left(tab_bar, 0, 0);
       lv_obj_t * logo = lv_image_create(tab_bar);
        lv_obj_add_flag(logo, LV_OBJ_FLAG_IGNORE_LAYOUT);
        LV_IMAGE_DECLARE(logoUTN43x43);
        lv_image_set_src(logo, &logoUTN43x43);
        lv_obj_align(logo, LV_ALIGN_LEFT_MID, -LV_HOR_RES / 2 + 25, 0);


        lv_obj_t * label = lv_label_create(tab_bar);
        lv_obj_add_style(label, &style_title, 0);
        lv_obj_add_flag(label, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_label_set_text_fmt(label, "UTN FRD");
        lv_obj_align_to(label, logo, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);

        label = lv_label_create(tab_bar);
        lv_label_set_text(label, "Control de Cisterna");
        lv_obj_add_flag(label, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_style(label, &style_text_muted, 0);
        lv_obj_align_to(label, logo, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);




    //creo el contenido de cada uno de los tabs
    CrearTabOperacion(tabOperacion);
    CrearTabConfiguracion(tabConfiguracion);

  

    //POngo un Timmer para que se actilicen los controles de pantalla con los valores de estado cargados 
    //en la estractura estado

    timerPantalla = xTimerCreate("ActualizarValoresTabOperacion", pdMS_TO_TICKS(500), pdTRUE, NULL, ActualizarValoresTabOperacion);
    xTimerStart(timerPantalla, 0);

    
};



/// @brief Funcion que crea el contenido del TAB Operación
/// @param parent 
static void CrearTabOperacion(lv_obj_t * parent)
{
    
   //creo un plane dentro del Tab que contendrá todos los controles.
    lv_obj_t * panel1 = lv_obj_create(parent);
    lv_obj_set_style_pad_all(panel1,2,0);
    lv_obj_set_height(panel1, LCD_HEIGHT - tab_h - 40);
    lv_obj_set_width(panel1, LCD_WIDTH - 40);
    
    
    
    /// Lista de controles para mostrar el estado del sistema  ////////////////////////////// 

    //Nivel tanque 
    lv_obj_t * lblNivel = lv_label_create(panel1); //Creo un Label para mostrar el texto Nivel del Tanque(Lts)
    
    lv_obj_align(lblNivel, LV_ALIGN_TOP_LEFT, 80,110);
    lv_obj_add_flag(lblNivel, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblNivel, &style_title,0);
    lv_label_set_text(lblNivel,"NIVEL TANQUE (Lts)");

    
    lv_obj_t * txtNivel = lv_textarea_create(panel1); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtNivel, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtNivel, 120, 42);
    lv_obj_set_pos(txtNivel, 230, 100);
    lv_textarea_set_align(txtNivel, LV_TEXT_ALIGN_RIGHT);
    char nivel[10];
    itoa(estado.nivelActual,nivel,10);
    lv_textarea_set_text(txtNivel, nivel); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 
     ControlesPantallaOperacion.pNivelTanque = txtNivel; //guardo el puntero a al txtBox para posterior actualizacion


    lv_obj_t * lblConsumo = lv_label_create(panel1); //Creo un Label para mostrar el texto Consumo(Hrs)
    lv_obj_align(lblConsumo, LV_ALIGN_TOP_LEFT, 80,220);
    lv_obj_add_flag(lblConsumo, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblConsumo, &style_title,0);
    lv_label_set_text(lblConsumo,"CONSUMO (Hrs)");

    lv_obj_t * txtConsumo = lv_textarea_create(panel1); //Creo la caja de textos txtConsumo
    lv_textarea_set_one_line(txtConsumo, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtConsumo, 120, 42);
    lv_obj_set_pos(txtConsumo, 230, 210);
    lv_textarea_set_align(txtConsumo, LV_TEXT_ALIGN_RIGHT);
    char consumo[10];
    itoa(estado.vertidoHora,consumo,10);
    lv_textarea_set_text(txtConsumo, consumo); //TODO : Este valor debe obtenerse de la lectura del caudal 
    ControlesPantallaOperacion.pConsumo = txtConsumo; //almeceno puntero a txtConsumo para posterior actualizacion


    lv_obj_t * lblAutoManual = lv_label_create(panel1); //Creo un Label para mostrar el texto Consumo(Hrs)
    lv_obj_align(lblAutoManual, LV_ALIGN_TOP_LEFT, 480,110);
    lv_obj_add_flag(lblAutoManual, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblAutoManual, &style_title,0);
    lv_label_set_text(lblAutoManual,"AUTO/MAN");

    lv_obj_t * swAutoManual; //Creo switch
    swAutoManual = lv_switch_create(panel1);
    lv_obj_add_event_cb(swAutoManual, automanual_event, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(swAutoManual, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_state(swAutoManual, LV_STATE_CHECKED);
    lv_obj_align(swAutoManual, LV_ALIGN_TOP_LEFT, 565,105);
    ControlesPantallaOperacion.pAutoMan = swAutoManual; //almaceno el puntero al swith auto/Manual
    
    lv_obj_t * lblBomba = lv_label_create(panel1); //Creo un Label para mostrar el texto Consumo(Hrs)
    lv_obj_align(lblBomba, LV_ALIGN_TOP_LEFT, 480,220);
    lv_obj_add_flag(lblBomba, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblBomba, &style_title,0);
    lv_label_set_text(lblBomba,"BOMBA");

    lv_obj_t * ledBomba = lv_led_create(panel1);
    lv_led_set_color(ledBomba, lv_color_hex(0x43d319)); //lv_color_hex(0x43d319) <= verde -  Rojo => lv_color_hex(0xff2816)
    lv_led_set_brightness(ledBomba, 200);
    lv_obj_align(ledBomba, LV_ALIGN_TOP_LEFT, 565,215);
    ControlesPantallaOperacion.pBomba = ledBomba;



};




/// @brief Función que crea el contenido del TAB Configuración
/// @param parent 
static void CrearTabConfiguracion(lv_obj_t * parent)
{
    // Creo el panel donde estarán todos los controles (TextBox, Botones, etc.)
    lv_obj_t * panel2 = lv_obj_create(parent);
    lv_obj_set_style_pad_all(panel2,2,0);
    lv_obj_set_height(panel2, LCD_HEIGHT - tab_h - 40);
    lv_obj_set_width(panel2, LCD_WIDTH - 40);



    lv_obj_t * lblNivelMin = lv_label_create(panel2); //Creo un Label para mostrar el texto NIVEL MINIMO
    lv_obj_align(lblNivelMin, LV_ALIGN_TOP_LEFT, 70,75);
    lv_obj_add_flag(lblNivelMin, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblNivelMin, &style_title,0);
    lv_label_set_text(lblNivelMin,"NIVEL MINIMO");

    lv_obj_t * txtNivelMin = lv_textarea_create(panel2); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtNivelMin, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtNivelMin, 120, 42); //Tamaño de caja de texto
    lv_obj_set_pos(txtNivelMin, 210, 62); //Posicion de caja de texto, +140 en X, -13 en Y (para alinear al texto plano)
    lv_textarea_set_align(txtNivelMin, LV_TEXT_ALIGN_RIGHT);
    ControlesPantallaConfiguracion.pNivelMinimo = txtNivelMin; // Almaceno el punto al textarea de nivelminimo
    char valorNivelMin[10];
    itoa(configuracion.nivelMin, valorNivelMin,10);
    lv_textarea_set_text(txtNivelMin,valorNivelMin); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 

      


    lv_obj_t * lblNivelMax = lv_label_create(panel2); //Creo un Label para mostrar el texto NIVEL MAXIMO
    lv_obj_align(lblNivelMax, LV_ALIGN_TOP_LEFT, 70,175);
    lv_obj_add_flag(lblNivelMax, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblNivelMax, &style_title,0);
    lv_label_set_text(lblNivelMax,"NIVEL MAXIMO");

    lv_obj_t * txtNivelMax = lv_textarea_create(panel2); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtNivelMax, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtNivelMax, 120, 42); //Tamaño de caja de texto
    lv_obj_set_pos(txtNivelMax, 210, 162); //Posicion de caja de texto, +140 en X, -13) en Y (para alinear al texto plano)
    lv_textarea_set_align(txtNivelMax, LV_TEXT_ALIGN_RIGHT);
    ControlesPantallaConfiguracion.pNivelMaximo = txtNivelMax; // Almaceno el punto al textarea de txtNivelMax
    char valorNivelMax[10];
    itoa(configuracion.nivelMax, valorNivelMax,10);
    lv_textarea_set_text(txtNivelMax,valorNivelMax); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 


    lv_obj_t * lblCaudalMax = lv_label_create(panel2); //Creo un Label para mostrar el texto CAUDAL MAXIMO
    lv_obj_align(lblCaudalMax, LV_ALIGN_TOP_LEFT, 70,275);
    lv_obj_add_flag(lblCaudalMax, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblCaudalMax, &style_title,0);
    lv_label_set_text(lblCaudalMax,"CAUDAL MAXIMO");

    lv_obj_t * txtCaudalMax = lv_textarea_create(panel2); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtCaudalMax, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtCaudalMax, 120, 42); //Tamaño de caja de texto
    lv_obj_set_pos(txtCaudalMax, 210, 262); //Posicion de caja de texto, +140 en X, -13) en Y (para alinear al texto plano)
    lv_textarea_set_align(txtCaudalMax, LV_TEXT_ALIGN_RIGHT);
    ControlesPantallaConfiguracion.pCaudalMaximo = txtCaudalMax; // Almaceno el punto al textarea de txtCaudalMax
    char valorCaudalMax[10];
    itoa(configuracion.caudalMax,valorCaudalMax,10);
    lv_textarea_set_text(txtCaudalMax,valorCaudalMax); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 



    lv_obj_t * lblMinAbsoluto = lv_label_create(panel2); //Creo un Label para mostrar el texto MINIMO ABSOLUTO
    lv_obj_align(lblMinAbsoluto, LV_ALIGN_TOP_LEFT, 400,75);
    lv_obj_add_flag(lblMinAbsoluto, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblMinAbsoluto, &style_title,0);
    lv_label_set_text(lblMinAbsoluto,"MINIMO ABSOLUTO");

    lv_obj_t * txtMinAbsoluto = lv_textarea_create(panel2); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtMinAbsoluto, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtMinAbsoluto, 120, 42); //Tamaño de caja de texto
    lv_obj_set_pos(txtMinAbsoluto, 555, 62); //Posicion de caja de texto, +155 en X, -13 en Y (para alinear al texto plano)
    lv_textarea_set_align(txtMinAbsoluto, LV_TEXT_ALIGN_RIGHT);
    ControlesPantallaConfiguracion.pMinimoAbsoluto = txtMinAbsoluto; // Almaceno el punto al textarea de txtMinAbsoluto
    char valorMinAbsoluto[10];
    itoa(configuracion.nivelAgotamiento,valorMinAbsoluto,10);
    lv_textarea_set_text(txtMinAbsoluto,valorMinAbsoluto); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 



    lv_obj_t * lblMaxAbsoluto = lv_label_create(panel2); //Creo un Label para mostrar el texto MAXIMO ABSOLUTO
    lv_obj_align(lblMaxAbsoluto, LV_ALIGN_TOP_LEFT, 400,175);
    lv_obj_add_flag(lblMaxAbsoluto, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblMaxAbsoluto, &style_title,0);
    lv_label_set_text(lblMaxAbsoluto,"MAXIMO ABSOLUTO");

    lv_obj_t * txtMaxAbsoluto = lv_textarea_create(panel2); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtMaxAbsoluto, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtMaxAbsoluto, 120, 42); //Tamaño de caja de texto
    lv_obj_set_pos(txtMaxAbsoluto, 555, 162); //Posicion de caja de texto, +155 en X, -13) en Y (para alinear al texto plano)
    lv_textarea_set_align(txtMaxAbsoluto, LV_TEXT_ALIGN_RIGHT);
    ControlesPantallaConfiguracion.pMaximoAbsoluto = txtMaxAbsoluto; // Almaceno el punto al textarea de txtMaxAbsoluto
    char valorMaxAbsoluto[10];
    itoa(configuracion.maxAdmitivo,valorMaxAbsoluto,10);
    lv_textarea_set_text(txtMaxAbsoluto,valorMaxAbsoluto); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 










    //Creo un primero boton y lo asocio a un evento 
    lv_obj_t * btnGuardar = lv_button_create(panel2); 
    BotonAplicarEstilo(btnGuardar, "Guardar" , 640,0);     
    lv_obj_add_event_cb(btnGuardar, btnGuardar_click, LV_EVENT_ALL, NULL);            




    //Creo un boton Salir y no le asocio ningún evento. A modo de ejemplo. 
    lv_obj_t * btnSalir = lv_button_create(panel2);
    BotonAplicarEstilo(btnSalir, "Salir", 640, 310); 
    

  //Teclado numerico 
       
    kbNumeric = lv_keyboard_create(panel2); //Creo el teclado
    lv_keyboard_set_mode(kbNumeric, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_add_flag(kbNumeric, LV_OBJ_FLAG_HIDDEN); 
    lv_obj_set_width(kbNumeric,400);
    
    
    lv_obj_add_event_cb(kbNumeric, keyboard_event_handler, LV_EVENT_VALUE_CHANGED, NULL );

    //// TECLADO NUMERICO 
     /*Create a keyboard and add the new map as USER_1 mode*/
    lv_obj_add_event_cb(txtNivelMin,ta_event_cb,LV_EVENT_ALL,kbNumeric);
    lv_keyboard_set_textarea(kbNumeric,txtNivelMin);
    
    lv_obj_add_event_cb(txtNivelMax,ta_event_cb,LV_EVENT_ALL,kbNumeric);
    lv_keyboard_set_textarea(kbNumeric,txtNivelMax);
    
    lv_obj_add_event_cb(txtCaudalMax,ta_event_cb,LV_EVENT_ALL,kbNumeric);
    lv_keyboard_set_textarea(kbNumeric,txtCaudalMax);

    lv_obj_add_event_cb(txtMinAbsoluto,ta_event_cb,LV_EVENT_ALL,kbNumeric);
    lv_keyboard_set_textarea(kbNumeric,txtMinAbsoluto);


    lv_obj_add_event_cb(txtMaxAbsoluto,ta_event_cb,LV_EVENT_ALL,kbNumeric);
    lv_keyboard_set_textarea(kbNumeric,txtMaxAbsoluto);

};



