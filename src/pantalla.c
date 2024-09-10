#include <../headers/pantalla.h>
#include <../headers/init.h>
#include <lvgl.h>


static void btnGuardar_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Guardar Nro: %d", cnt);

    }
};



void PantallaPrincipal(void)
{
   
    InicilizarComponentesPantalla();
   
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x080808), LV_PART_MAIN);

    //Creo el componente TabView
    lv_obj_t * tabView;
    tabView = lv_tabview_create(lv_screen_active());
    //lv_tabview_set_tab_bar_size(tabView, LCD_WIDTH);
    //ATENCION: FALTA AGREGAR EVENTO AL TABVIEW
    //lv_obj_add_event_cb(tv, tabview_delete_event_cb, LV_EVENT_DELETE, NULL);
    //Agrego dos tabs al tabview
    lv_obj_t * tabOperacion = lv_tabview_add_tab(tabView, "OPERACION");
    lv_obj_t * tabConfiguracion = lv_tabview_add_tab(tabView, "CONFIGURACION");

    
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



    
};



/// @brief Funcion que crea el contenido del TAB Operación
/// @param parent 
static void CrearTabOperacion(lv_obj_t * parent)
{
    
   //creo un plane dentro del Tab que contendrá todos los controles.
    lv_obj_t * panel1 = lv_obj_create(parent);
    lv_obj_set_style_pad_all(panel1,2,0);
    lv_obj_set_height(panel1, LCD_HEIGHT - tab_h - 20);
    lv_obj_set_width(panel1, LCD_WIDTH - 30);
    
    
    
    /// Lista de controles para mostrar el estado del sistema  ////////////////////////////// 

    //Nivel tanque 
    lv_obj_t * lblNivel = lv_label_create(panel1); //Creo un Label para mostrar el texto Nivel del Tanque(Lts)
   
    lv_obj_align(lblNivel, LV_ALIGN_TOP_LEFT, 10,20);
    lv_obj_add_flag(lblNivel, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblNivel, &style_title,0);
    lv_label_set_text(lblNivel," NIVEL TANQUE (Lts)");
    
    

    lv_obj_t * txtNivel = lv_textarea_create(panel1); //Creo la caja de textos txtNivel
    lv_textarea_set_one_line(txtNivel, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtNivel, 120, 40);
    lv_obj_set_pos(txtNivel, 160, 10);
    lv_textarea_set_align(txtNivel, LV_TEXT_ALIGN_RIGHT);
    char nivel[10];
    itoa(estado.nivelActual,nivel,10);
    lv_textarea_set_text(txtNivel, nivel); //TODO : Este valor debe obtenerse de la lectura del nivel del tanque 


    lv_obj_t * lblConsumo = lv_label_create(panel1); //Creo un Label para mostrar el texto Consumo(Hrs)
    lv_obj_align(lblConsumo, LV_ALIGN_TOP_LEFT, 10,70);
    lv_obj_add_flag(lblConsumo, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_style(lblConsumo, &style_title,0);
    lv_label_set_text(lblConsumo,"CONSUMO (Hrs)");

    lv_obj_t * txtConsumo = lv_textarea_create(panel1); //Creo la caja de textos txtConsumo
    lv_textarea_set_one_line(txtConsumo, true); //Indico que es de tipo linea simple.
    lv_obj_set_size(txtConsumo, 120, 40);
    lv_obj_set_pos(txtConsumo, 160, 60);
    lv_textarea_set_align(txtConsumo, LV_TEXT_ALIGN_RIGHT);
    char consumo[10];
    itoa(estado.vertidoHora,consumo,10);
    lv_textarea_set_text(txtConsumo, consumo); //TODO : Este valor debe obtenerse de la lectura del caudal 


      
    //lv_obj_set_style_text_font(lblNivelValor, LV_FONT_MONTSERRAT_12, 0);

/* 
    lv_obj_t * txtNivel = lv_textarea_create(panel1) ;  //Creo un ob
    lv_obj_align(txtNivel, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_set_size(txtNivel, 50, 50); // lv_pct(90)
    lv_obj_add_state(txtNivel, LV_STATE_FOCUSED);
*/
    
    //A modo de ejemplo creo un boton y le asigno los estilos por defecto que cree dentro de la funcion AplicarEstiloBoton
    lv_obj_t * btnSalir = lv_button_create(panel1); //Creo el objeto como hijo del padre=>Tab
    BotonAplicarEstilo(btnSalir, "Salir", LCD_WIDTH-150, 300); //Aplico el estilo por defecto que hemos definido para los botones

};



/// @brief Función que crea el contenido del TAB Configuración
/// @param parent 
static void CrearTabConfiguracion(lv_obj_t * parent)
{
    // Creo el panel donde estarán todos los controles (TextBox, Botones, etc.)
    lv_obj_t * panelc = lv_obj_create(parent);
    lv_obj_set_style_pad_all(panelc,2,0);
    lv_obj_set_height(panelc, LCD_HEIGHT - tab_h - 20);
    lv_obj_set_width(panelc, LCD_WIDTH - 30);



    //Creo un primero boton y lo asocio a un evento 
    lv_obj_t * btnGuardar = lv_button_create(panelc); 
    BotonAplicarEstilo(btnGuardar, "Guardar" , 100,100);     
    lv_obj_add_event_cb(btnGuardar, btnGuardar_click, LV_EVENT_ALL, NULL);            




    //Creo un boton Salir y no le asocio ningún evento. A modo de ejemplo. 
    lv_obj_t * btnSalir = lv_button_create(panelc);
    BotonAplicarEstilo(btnSalir, "Salir", 100, 300); 
    


};



