
#include "uselessbox.h"
bool Init_SD(void)
{
  bool state;
  if (!SD.begin(SDChipSelectPin)) {
    Serial.println("Card Mount Failed");

    state = false;
  }
  else {
    state = true;
  }
  return state;
}

void  TestServo(void) {

  int TimeTest = 5000 ;
  Serial.println("-----Test SERVOS-------\r");
  Serial.println("Caja abierta\r") ;
  WriteServoBox(BoxOpen); //ABRE CAJA
  delay(500);
  WriteServoBox(BoxOpen - 2); //ABRE CAJA
  delay(TimeTest);
  Serial.println("Mano fuera\r") ;
  WriteServoHand(HandPut);    // ACCIONAR MANO
  delay(TimeTest);
  Serial.println("Mano dentro\r") ;
  WriteServoHand(HandRemove);  // PLEGAR MANO
  delay(TimeTest);
  Serial.println("Caja cerrada\r") ;
  WriteServoBox(BoxClose); //CIERRA CAJA
  delay(TimeTest);
  return;
}

void  TestLed(void) {
  int TimeTest = 5000 ;
  Serial.println("-----TEST LED-------\r");

  Serial.println("Test LED ROJO\r");
  digitalWrite(LedRJ , HIGH );    // poner el Pin en HIGH
  delay(TimeTest);
  digitalWrite(LedRJ , LOW);    // poner el Pin en LOW


  Serial.println("Test LED BLANCO\r");
  digitalWrite(LedBL , 255 );    // poner el Pin en HIGH
  delay(TimeTest);
  digitalWrite(LedBL , LOW);    // poner el Pin en LOW
  return;
}

void  TestInterruptores(void) {
  int TimeTest = 5000 ;
  Serial.println("-----TEST ENTRADAS-------\r");

  Serial.println("Interruptor CAJA\r");
  Serial.println("Estado 1\r");
  Serial.println(digitalRead(InterruptorPuerta)) ;
  delay(TimeTest);
  Serial.println("Estado 2");
  Serial.println(digitalRead(InterruptorPuerta)) ;

  Serial.println("Interruptor Terror\r");
  Serial.println("Estado 1\r");
  Serial.println(digitalRead(InterrutorTerror)) ;
  delay(TimeTest);
  Serial.println("Estado 2\r");
  Serial.println(digitalRead(InterrutorTerror));
  return;
}


void  TestMeneo(void) {
  int TimeTest = 1000 ;
  Serial.println("-----Test MOTOR-------\r");
  analogWrite(MotorShake , 255 );    // Activar a la mitad de revoluciones
  delay(500);
  for (int i = 0; i < 3; i++)
  {
    analogWrite(MotorShake , LOW);    // Apagar
    delay(600);
    analogWrite(MotorShake , shakespeed );    // Activar a la mitad de revoluciones
    delay(300);
  }
  analogWrite(MotorShake , LOW);    // Apagar
  return;
}

void  TestAudio(void) {
  int TimeTest = 5000 ;
  Serial.println("-----Test AUDIO-------\r");
  if (Sd_state) {
    Serial.println("Vol " + String(VolAudio) + "\r");
    tmrpcm.setVolume(VolAudio);
    Serial.println("tictac.wav\r");
    tmrpcm.play((char *)"tictac.wav",1); 

  }

  Serial.println("Tono\r");
  tone(Altavoz, 1000, TimeTest);
  delay(TimeTest);
  return;
}
void  TestFunc(void) {

  TestInterruptores();
  TestLed();
  TestMeneo();
  TestServo();
  TestAudio();
  Serial.println("Fin Test\r");
  return;
}

void WriteServoBox(int grados) {
int pwm = (grados * 9) + 600 ;
  boxServo.write(pwm);
}

void WriteServoHand(int grados) {
int pwm = (grados * 9)+ 745 ;
  handServo.write(pwm);
}


void setup()
{
  Serial.begin(9600);
  Serial.println("#############Power On#############\r");
  Serial.println("##########Hardware Test###########\r");

  pinMode(InterruptorPuerta, INPUT_PULLUP); //Pin para interrupcion cuando entre en modo sleep
  pinMode(LedRJ, OUTPUT);   //El resto de pines como salida
  pinMode(LedBL, OUTPUT);
  pinMode(MotorShake, OUTPUT);
  pinMode(ServoAperturaBoxPin, OUTPUT);
  pinMode(ServoManoPin, OUTPUT);
  pinMode(Altavoz, OUTPUT);

  handServo.attach(ServoManoPin);
  boxServo.attach(ServoAperturaBoxPin);

  //DEFAULT
  WriteServoHand(HandRemove);   //Posizione di partenza del servomotore Mano
  WriteServoBox(BoxClose);    //Posizione di partenza del servomotore che apre la scatola
  analogWrite(MotorShake, LOW); //Poner el motor dc a 0 shakespeed
  digitalWrite(LedBL , LOW);    // poner el Pin en LOW
  digitalWrite(LedRJ , LOW);    // poner el Pin en LOW

  tmrpcm.speakerPin = Altavoz; // selecciona   audio ouput
  Sd_state = Init_SD();

}

void loop()
{


  if (Serial.available() > 0)
  {
    int inByte = Serial.read(); // send t,a1,a2

    switch (inByte) {
      case 't':
        Serial.println("-----------Iniciando Test-----------\n");
        action =  Serial.parseInt();
        switch (action)
        {
          case 1:
            TestServo();
            break;
          case 2:
            TestLed();
            break;
          case 3:
            TestInterruptores();
            break;
          case 4:
            TestMeneo();
            break;
          case 5:
            TestAudio();
            break;
          default:
            TestFunc();
            break;

        }

        break;
      default:
        break;
    }
  }



}
