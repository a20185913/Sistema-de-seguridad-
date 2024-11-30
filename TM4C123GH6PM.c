/**********************************************************************************
******/
/**********************************************************************************
******/
/***** PONTIFICIA UNIVERSIDAD CATÓLICA DEL PERÚ *****/
/***** PONTIFICIA UNIVERSIDAD CATÓLICA DEL PERÚ *****/
/***** FACULTAD DE CIENCIAS E INGENIERÍA *****/
/***** SISTEMAS DIGITALES *****/
/**********************************************************************************
******/
/***** Microcontrolador: TM4C123GH6PM *****/
/***** EvalBoard: Tiva C Series TM4C123G LaunchPad *****/
/***** Autor: Alessandro Romero Santos y Yhamil Favio Munive Calcina *****/
/***** Fecha: 14/11/2021 *****/
/***** *****/
/**********************************************************************************
******/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "bitmapPhotos.h"
#include "TivaES.h"
uint32_t Sistema=0,Cursor=1, Seleccionar=0, Clave[10], Digitos=3, Intentos=3,
Comprobar[10], flag=0, flag1=0, flag2=0, flag3=0, Numero=1, flag4=0;
uint32_t Puerta_Principal=0, Ventana_Principal=0, Sala_Principal=0, Jardin=0,
Ventana_Cocina=0, Ventana_Cuarto=0, Cuarto_Invitado=0, Cuarto_Nino=0;
void config_portF(void)
{
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // activamos la señal de reloj
del puerto F
while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0 ) {} // espera a que el puerto este
listo para operar
GPIO_PORTF_LOCK_R = 0x4C4F434B;
// Desbloquear puerto F
GPIO_PORTF_CR_R |= 0x01;
// Se habilita PF0 (SW2)
//SW1 está conectado al pin PF4, SW2 al pin PF0.
GPIO_PORTF_DIR_R &= ~(0x11);
// Configura el pin 0 (bit0) y pin 4 (bit4) como entrada
GPIO_PORTF_AFSEL_R &= ~(0x11);
// No utilizar funciones alternas
GPIO_PORTF_PUR_R |= 0x11;
// Activa resistencias de pull-up en pin 0 (bit0) y pin
4 (bit4)
GPIO_PORTF_DEN_R |= 0x11;
// Habilita pines pin 0 y pin 4 para funcionar con
niveles digitales.
//Led Azul esta conectado al pin PF2, Led Rojo esta conectado al pin PF1, Led Verde
esta conectado al pin PF3
GPIO_PORTF_DIR_R |= 0x0E;
// Configura el pin 1, 2, 3 del puerto F como salida
GPIO_PORTF_DR8R_R |= 0x0E;
// Activa el driver de 8 mA en el pin 1, 2, 3.
GPIO_PORTF_DEN_R |= 0x0E;
// Habilita el pin 1, 2, 3; para funcionar con niveles
digitales.
GPIO_PORTF_DATA_R &= ~(0x0E);
// Apaga el led conectado a PF1, PF2, PF3 (Rojo, Azul, Verde)
} // fin config_portF
void config_portD(void){
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; //
Activamos el Reloj de Puerto D
while( (SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3)==0) { } // Se espera a que se active
el reloj
GPIO_PORTD_LOCK_R = 0x40007520;
// Desbloquear puerto D
GPIO_PORTD_CR_R |= 0x40007524;
// Se habilita PD3, PD2, PD1 y PD0
GPIO_PORTD_DIR_R &= ~(0x0E);
// Configura el pin 2 (bit3), pin 3 (bit4), pin 4
(bit5), pin 5 (bit6) como entrada
GPIO_PORTD_AFSEL_R &= ~(0x0E);
// No utilizar funciones alternas
GPIO_PORTD_PUR_R |= 0x0E;
// Activa resistencias de pull-up en pin 2 (bit3), pin 3
(bit4), pin 4 (bit5), pin 5 (bit6)
GPIO_PORTD_DEN_R |= 0x0E;
// Habilita pines pin 2 (bit3), pin 3 (bit4), pin
4 (bit5), pin 5 (bit6) para funcionar con niveles digitales.
}
void config_portE(void){
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; //
Activamos el Reloj de Puerto D
while( (SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R4)==0) { } // Se espera a que se active
el reloj
GPIO_PORTE_LOCK_R = 0x40024520;
// Desbloquear puerto D
GPIO_PORTE_CR_R |= 0x40024524;
// Se habilita PD3, PD2, PD1 y PD0
GPIO_PORTE_DIR_R &= ~(0x0E);
// Configura el pin 2 (bit3), pin 3 (bit4), pin 4
(bit5), pin 5 (bit6) como entrada
GPIO_PORTE_AFSEL_R &= ~(0x0E);
// No utilizar funciones alternas
GPIO_PORTE_PUR_R |= 0x0E;
// Activa resistencias de pull-up en pin 2 (bit3), pin 3
(bit4), pin 4 (bit5), pin 5 (bit6)
GPIO_PORTE_DEN_R |= 0x0E;
// Habilita pines pin 2 (bit3), pin 3 (bit4), pin
4 (bit5), pin 5 (bit6) para funcionar con niveles digitales.
}
void ConfiguraUART0(void){
// Habilitamos reloj para el UART0
SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
// Habilitamos reloj para GPIOA
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
// Inhabilitamos el UART0
UART0_CTL_R &= ~UART_CTL_UARTEN;
// Velocidad 9600bps (Fsysclk = 16MHz)
UART0_IBRD_R = (UART0_IBRD_R & 0xFFFF0000) | 104;
UART0_FBRD_R = (UART0_FBRD_R & 0xFFFFFFC0) | 11;
// 8, N, 1, FIFOs habilitados
UART0_LCRH_R = (UART0_LCRH_R & 0xFFFFFF00) | 0x70;
// Habilitamos el UART0
UART0_CTL_R |= UART_CTL_UARTEN;
// Desactivamos modo analógico en PA0 y PA1
GPIO_PORTA_AMSEL_R &= ~(0x03);
// Conectamos UART0 a PA0 y PA1
GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011;
// Activamos funciones alternas en PA0 y PA1
GPIO_PORTA_AFSEL_R |= 0x03;
// Activamos funciones digitales en PA0 y PA1
GPIO_PORTA_DEN_R |= 0x03;
}
void TxCar( uint8_t Car )
{
while((UART0_FR_R & UART_FR_TXFF) != 0 ) ; //Espera mientras pila llena
UART0_DR_R = Car;
}
uint8_t RxCar(void)
{
//while((UART0_FR_R & UART_FR_RXFE) != 0) ; //Espera mientra pila vacía
//return UART0_DR_R & 0xFF;
if((UART0_FR_R & UART_FR_RXFE) == 0){
return UART0_DR_R & 0xFF;
}else{
return 0;
}
}
void TxCadena( uint8_t Cadena[] )
{
uint8_t q;
for( q = 0; Cadena[q] != '\0'; q++ )
TxCar( Cadena[q] );
}
void Retardo(uint32_t NumCiclos){
uint32_t i;
for(i=0;i<=NumCiclos*2000000;i++) {}
// Lazo de control FOR para generar un
retardo.
}
void Ingresar_Contrasena_Inicial(void){
uint32_t i=0;
Nokia5110_SetCursor(5, 5);
Nokia5110_OutChar(48+Numero);
while(i<=Digitos){
do{
TxCar(0x1B);
TxCadena("[");
TxCadena("2");
TxCadena("J");
TxCadena("\r");
TxCadena("Ingrese Contrasena Inicial");
if((GPIO_PORTF_DATA_R&0x10) == 0x00){
while((GPIO_PORTF_DATA_R & 0x11) == 0x01){};
Numero++;
}
if(RxCar()==119 | RxCar()==87){
while(RxCar()==119 | RxCar()==87){};
Numero++;
}
if(RxCar()== 83| RxCar()== 115){
while(RxCar()== 83| RxCar()== 115){};
Numero--;
}
if(Numero>9){
Numero=1;
}
if(Numero<1){
Numero=9;
}
Nokia5110_SetCursor(5, 5);
Nokia5110_OutChar(48+Numero);
TxCadena("\n\r");
TxCar(Numero+48);
Retardo(2);
}while((RxCar()!=13) && ((GPIO_PORTF_DATA_R&0x01)!=0x00));
while(RxCar()==13){};
while((GPIO_PORTF_DATA_R & 0x11) == 0x10){};
flag4=0;
TxCadena("$");
Nokia5110_SetCursor(3, 5);
Nokia5110_OutString("$");
Retardo(2);
Nokia5110_SetCursor(3, 5);
Nokia5110_OutString(" ");
Clave[i]=Numero;
i++;
}
}
uint32_t Validar_Contrasena(){
uint32_t i=0,j=0,k=0;
while(k<Intentos & flag1==0){
flag2=0;
Numero=1;
while(i<=Digitos){
Nokia5110_SetCursor(5, 5);
Nokia5110_OutChar(48+Numero);
do{
TxCar(0x1B);
TxCadena("[");
TxCadena("2");
TxCadena("J");
TxCadena("\r");
TxCadena("Ingrese Contrasena Inicial");
if((GPIO_PORTF_DATA_R&0x10) == 0x00){
while((GPIO_PORTF_DATA_R & 0x11) == 0x01){};
Numero++;
}
if(RxCar()==119 | RxCar()==87){
while(RxCar()==119 | RxCar()==87){};
Numero++;
}
if(RxCar()== 83| RxCar()== 115){
while(RxCar()== 83 | RxCar()== 115){};
Numero--;
}
if(Numero>9){
Numero=1;
}
if(Numero<1){
Numero=9;
}
Nokia5110_SetCursor(5, 5);
Nokia5110_OutChar(48+Numero);
TxCadena("\n\r");
TxCar(Numero+48);
Retardo(2);
}while((RxCar()!=13) && ((GPIO_PORTF_DATA_R&0x01)!=0x00));
while((GPIO_PORTF_DATA_R & 0x11) == 0x10){};
Nokia5110_SetCursor(3, 5);
Nokia5110_OutString("$");
TxCadena("$");
Retardo(2);
Nokia5110_SetCursor(3, 5);
Nokia5110_OutString(" ");
Comprobar[i]=Numero;
i++;
}
for(j=0; j<=Digitos;j++){
if(Comprobar[j]!=Clave[j]){
Nokia5110_DrawFullImage((char*)contrasena_incorrecta);
Retardo(2);
Nokia5110_DrawFullImage((char*)ingre_contra_d_sis);
i=0;
k++;
flag2=1;
break;
}
}
if(flag2==0){
flag1=1;
}
}
if(k==Intentos){
flag1=0;
return 0;
}else{
flag1=0;
return 1;
}
}
void Ingresar_Nueva_Contrasena(void){
uint32_t i=0;
