#include <stdio.h>
#include <lvgl.h>


#ifdef __cplusplus
extern "C" {
#endif


#define LCD_WIDTH               800
#define LCD_HEIGHT              480
#define LCD_NUM_FB              2
#define LCD_GPIO_BCKL           GPIO_NUM_2
#define tab_h                   70     //Alto del tab Bar.

//Estilos
//static lv_style_t StyleBtn; //Estilo general de los botones
//static lv_grad_dsc_t StyleBtnLabel;//Estilo del texto del Boton.
static lv_style_t style_title; //Estilo de los titulos de la aplicacion
static lv_style_t style_text_Normal; //Estilo de los textos normales de etiquetas 
static lv_style_t style_text_muted; // Estilo texto apagado




enum TextBoxEdicionEnum
{   
    Deshabilitado =0,  //El TextBox aparrecerá deshabilitadao
    EditableNumerico,   //Se podrá  editar y aparecerá el teclado numérico
    EditableTexto,      // Se podrá Editar y aparecerá el teclado de Texto simple. 
    EdtiableAlfanumerico //Se podrá Editar y aparecerá el teclado alfanumérico. 

} ;

struct st_EstadoCisterna
{
    long     nivelActual;
    long     vertidoHora;
    bool    modoAuto;
    bool    bombaEncendida;

} ;

struct st_ParametrosConfiguracion
{
    long     nivelMin;   //Mínimo nivel para encender bomba en modo automático
    long     nivelMax;   //Máximo nivel para apagar bomba en modo automático
    long     caudalMax;  //Caudal máximo permitivo antes de cerrar el vertido por fuga.
    long     maxAdmitivo; //Nivel de seguridad máximo permitido en modo Manual   
    long     nivelAgotamiento;  //nivel de activacion de alarma por agotamiento  
    bool    modoAuto; //Infica si el sistema debe estar en ModoAutomatico = True o Manual=False.

    
};

/// @brief Contiene los punteros a los objetos TextArea de LVGL para  acceder a ellos desde otros módulos.
struct st_ControlesPantallaOperacion
{
    lv_obj_t * pNivelTanque;
    lv_obj_t * pConsumo;
    lv_obj_t * pAutoMan;
    lv_obj_t * pBomba;
} ;



void InicilizarComponentesPantalla() ;//Iniciliza los componentes Graficos de la pantalla

void BotonAplicarEstilo(lv_obj_t * objBoton,  char * LabelText, int32_t PosicionX, int32_t PosicionY);


void TextBoxAplicarEstilo(lv_obj_t * objTextBox, enum TextBoxEdicionEnum Editable,   char * ValorDefecto, int32_t PosicionX, int32_t PosicionY);





#ifdef __cplusplus
}
#endif