#ifndef uselessbox_h
#define uselessbox_h




#include <avr/sleep.h>        // libreria para poner a dormir al ardruino
#include <SD.h>               // libreria SDCARD
#define  SDChipSelectPin  10 
#include <SPI.h>             // libreria SPI
#include <TMRpcm.h>           // libreria AUDIO
                              // $HOME/Arduino/libraries/ServoTimer2-master/pcmConfig.h
                              // #define buffSize 128
                              // #define buffSize 128                              
#include "./pcmConfig.h"
#include <ServoTimer2.h>    //Mofify ServoTimer2
                            //$HOME/Arduino/libraries/ServoTimer2-master/ServoTimer2.h
                            //#define MIN_PULSE_WIDTH       600
                            //#define MAX_PULSE_WIDTH      2600
                          
//DEFINE PINES
const int  InterruptorPuerta = 2 ;
const int  MotorShake = 3 ;
const int  InterrutorTerror = 4 ;
const int  ServoManoPin = 5 ;
const int  ServoAperturaBoxPin = 6 ;
const int  LedRJ = 7 ;
const int  LedBL = 8 ;
const int  Altavoz = 9 ;



//DEFINE POSICIONES SERVOS
const int  DefineHandCero = 745 ;
const int  HandPut = 0 ; // 0 // 544
const int  HandRemove = 205;//180 // 2500  // 231
const int  DefineBoxCero = 600 ;
const int  BoxOpen = 127; //115 // 1720    //170
const int  BoxClose = 85 ;//80 //1400   //141

//VOLUMEN AUDIO
const int VolAudio = 5 ;
bool Sd_state = false ;

//Numero de veces que pasa por el bucle antes de entrar en reposo
const long Sleep_Time = 9999999 ; //999999 20s  9999999 1min

//DEFINE TIPOS 
ServoTimer2 handServo;
ServoTimer2 boxServo;


TMRpcm tmrpcm;

int switchAnimazione = 0;
int action = 1;
int shakespeed = 255 ; //speed shake 0 - 255;
long countLoops = 0; //cuenta el numero de pasadas antes de entrara en modo sleep




#endif
