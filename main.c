/*main.c 
=========
IMPORTANTE: Por ahora faltan los includes

*/

#include <stdio.h>
#include <stdlib.h>

/*Definiciones globales:
========================
A continuacion se definen las 
	-señales (SIG_*)
	-teclas (KEY_*)
	-estados (ST_*)
	-errores (ERR_*)
de nuestra maquina de estados

NOTA: Por claridad y consistencia se ha numerado cada grupo a partir de cierto valor.*/

//Estados posibles (A partir del 1)
#define ST_Idle		1
#define ST_ReadPass	2
#define ST_WaitUSB 	3
#define ST_ReadUSB 	4
#define ST_Browser 	5
#define ST_Confirm 	6
#define ST_Processing 	7
#define ST_End 		8
#define ST_Error 	9

//Señales (A partir del 101)
#define SIG_GoodPass 	101
#define SIG_BadPass 	102
#define SIG_UsbFound 	103
#define SIG_UsbReady 	104
#define SIG_JobDone 	105
#define SIG_TimeOut 	106

//Teclas (A partir del 151)
//FALTAN MAS (se puede hasta 8)
#define KEY_Enter 	151
#define KEY_Cancel 	152
#define KEY_Up 		153
#define KEY_Down 	154

//Errores (A partir del 201)
//Error en el formato del archivo excellon
#define ERR_ParseFormat 201
//No se encontro el archivo excellon
#define ERR_ParseFile	202
//Error de lectura del USB
#define ERR_USB		203
#define ERR_Uart	204

/*Estructuras usadas para manejar los estados:
=============================================
A continuacion se definen las estructuras que guardaran los estados, sus funciones asociadas
y las senales o teclas que cambiaran al siguiente estados
	-La estructura STATE contiene la lista de estados y un puntero a su funcion asociada
	-La estructura NEXT_STATE contiene la lista de estados, y los posibles siguientes estados 
	segun la senal recibida*/
typedef struct{
	unsigned char current_state;
	char (*p2Func)(unsigned char);//MODIFICAR SI SE USARAN MAS VARIABLES
}STATE;

typedef struct{
	unsigned char current_state;
	unsigned char signal;
	unsigned char next_state;
}NEXT_STATE;

NEXT_STATE stateMapping[] = {
//current_state		//signal	next_state
{ST_Idle,		KEY_Enter,	ST_WaitUSB},

{ST_ReadPass,		KEY_Cancel,	ST_Idle},
{ST_ReadPass,		SIG_GoodPass,	ST_WaitUSB},
{ST_ReadPass,		SIG_TimeOut,	ST_Idle},

{ST_WaitUSB,		SIG_UsbFound,	ST_ReadUSB},
{ST_WaitUSB,		KEY_Cancel,	ST_Idle},
{ST_WaitUSB,		SIG_TimeOut,	ST_Idle},
{ST_WaitUSB,		ERR_USB,	ST_Error},

{ST_ReadUSB,		KEY_Cancel,	ST_WaitUSB},
{ST_ReadUSB,		SIG_UsbReady,	ST_Browser},
{ST_ReadUSB,		SIG_TimeOut,	ST_Idle},//Hay que ver bien cuanto demora leer el USB antes de timeoutear

{ST_Browser,		KEY_Enter,	ST_Confirm},
{ST_Browser,		KEY_Cancel,	ST_WaitUSB},
{ST_Browser,		SIG_TimeOut,	ST_Idle},
{ST_Browser,		ERR_USB,	ST_Error},

{ST_Confirm,		KEY_Enter,	ST_Processing},
{ST_Confirm,		KEY_Cancel,	ST_Browser},
{ST_Confirm,		SIG_TimeOut,	ST_Idle},
{ST_Confirm,		ERR_USB,	ST_Error},

{ST_Processing,		KEY_Cancel,	ST_Browser},//Ver bien a donde regresa si se cancela el procesamiento
{ST_Processing,		SIG_JobDone,	ST_End},
{ST_Processing,		ERR_ParseFormat,ST_Error},
{ST_Processing,		ERR_ParseFile,	ST_Error},
{ST_Processing,		ERR_Uart,	ST_Error},

{ST_End,		SIG_TimeOut,	ST_Idle},
{ST_End,		KEY_Enter,	ST_Idle},
{ST_End,		KEY_Cancel,	ST_Idle},
{ST_End,		KEY_Up,		ST_Idle},
{ST_End,		KEY_Down,	ST_Idle}
};

/*
Aca se crea el arreglo con cada estado y su funcion correspondiente

Una vez se hallan creado las funciones asociadas a cada estado descomentar
esta seccion y agregar la funcion para cada caso
===========================================================================
STATE stateFunctions[] = {
//current_state		//function
{ST_Idle,		Bienvenido()},
{ST_ReadPass,		FALTA},
{ST_WaitUSB,		FALTA},
{ST_ReadUSB,		FALTA},
{ST_Browser,		FALTA},
{ST_Confirm,		FALTA},
{ST_Processing,		FALTA},
{ST_End,		FALTA},
{ST_Error,		FALTA}
};
*/

void main(void){
	//Variables que mantendran las senales y entradas recibidas
	unsigned char inputKey;
	unsigned char signal;
	unsigned char error;

	
	while(1){
		signal=getSignals();
		if (keysEnabled){
			inputKey=getKey();
		}
	}
}