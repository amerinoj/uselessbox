



/* www.labdomotic.com
  Youtube channel TARTAGLIA DANIELE
*/
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

void  PlayAudio(char * song, int Vol) {
  if (Sd_state) {
    tmrpcm.setVolume(Vol);
    tmrpcm.play((char *)song);

  }
  return;
}

void  StopAudio(void) {
  if (Sd_state) {
    tmrpcm.setVolume(0);
    tmrpcm.disable();

  }
  return;
}


void  Meneo(void) {


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

void Fix_Servo(void)//fix ruido servos
{
  int pos = boxServo.read();
  boxServo.write(pos + 5); //fix ruido servos
  delay(500);
  boxServo.write(pos - 10); //fix ruido servos

  return;
}

void OutSleep(void)
{
  Serial.write("Sleep mode OFF\n");
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(InterruptorPuerta));


}
void EnterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(InterruptorPuerta), OutSleep , LOW);// attach interrupt to pin d2
  Serial.write("Sleep mode ON\n");
  delay(100);
  // Now enter sleep mode.
  sleep_mode(); //sleep_cpu();

}

void WriteServoBox(int grados) {
  int pwm = (grados * 9) + DefineBoxCero ;
  boxServo.write(pwm);
}

void WriteServoHand(int grados) {
  int pwm = (grados * 9) + DefineHandCero ;
  handServo.write(pwm);
}



void DefaultOuput(void)
{
  StopAudio();
  WriteServoHand(HandRemove); //fix ruido servos
  WriteServoBox(BoxClose);    //Posizione di partenza del servomotore che apre la scatola

  analogWrite(MotorShake, LOW); //Poner el motor dc a 0 shakespeed
  digitalWrite(LedBL , LOW);    // poner el Pin en LOW
  digitalWrite(LedRJ , LOW);    // poner el Pin en LOW
  delay(300);
  WriteServoHand(HandRemove - 2); //fix ruido servos
}
void setup()
{
  Serial.begin(9600);
  Serial.println("#############Power On#############\r");

  pinMode(InterruptorPuerta, INPUT_PULLUP); //Pin para interrupcion cuando entre en modo sleep
  pinMode(LedRJ, OUTPUT);   //El resto de pines como salida
  pinMode(LedBL, OUTPUT);
  pinMode(MotorShake, OUTPUT);
  pinMode(ServoAperturaBoxPin, OUTPUT);
  pinMode(ServoManoPin, OUTPUT);
  pinMode(Altavoz, OUTPUT);

  handServo.attach(ServoManoPin);
  boxServo.attach(ServoAperturaBoxPin);

  tmrpcm.speakerPin = Altavoz; // selecciona   audio ouput
  tmrpcm.setVolume(VolAudio);
  tmrpcm.quality(1);
  Sd_state = Init_SD();
  randomSeed(analogRead(0));
  action = random(1, 17);
  DefaultOuput();

}


void loop()
{

  if (Serial.available() > 0)
  {
    int inByte = Serial.read(); // send a1,a2

    switch (inByte) {

      case 'a':
        action =  Serial.parseInt();
        if (action <= 0 ) {
          action = 1 ;
        }
        Serial.println("-----------Forzando Accion " + String(action) + "-----------");

        break;

      default:
        break;
    }
  }

  switchAnimazione = digitalRead(InterruptorPuerta);
  int  Terror = digitalRead(InterrutorTerror);

  // ----------------------------------------------------------------------------------------------------------------------------
  // -------------------------------                  PRIMA ANIMAZIONE                -------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------

  if (switchAnimazione == LOW) {

    if (action == 1)
    {

      Serial.println("Action 1");
      PlayAudio((char *)"elisa.wav" , VolAudio);
      delay(5000);

      for (int i = BoxClose; i <= BoxOpen; i++)
      {

        WriteServoBox(i);
        if (Terror == LOW) {
          digitalWrite(LedRJ , HIGH );
          delay(70);
          digitalWrite(LedRJ , LOW );
          delay(70);
        }
        else
        {
          digitalWrite(LedBL , HIGH );
          delay(140);
        }


      }
      Fix_Servo();

      if (Terror == LOW) {
        digitalWrite(LedBL , HIGH );
      }
      for (int i = HandRemove; i >= 35; i--)
      {
        WriteServoHand(i);
        delay(20);
      }
      delay(1000);
      WriteServoHand(HandPut);
      if (Terror == LOW) {
        digitalWrite(LedBL , LOW );
      }
      delay(200);

      for (int i = HandPut; i <= HandRemove; i++)
      {
        WriteServoHand(i);
        delay(20);

      }

      for (int i = BoxOpen; i >= BoxClose; i--)
      {
        WriteServoBox(i);
        delay(20);
      }

      delay(3000);
      if (Terror == LOW) {
        Meneo();
      }
      else {
        delay(3500);
      }


    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  SECONDA ANIMAZIONE                -----------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 2)
    {
      Serial.println("Action 2");

      Meneo();
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      }
      else {
        digitalWrite(LedBL , HIGH );
      }
      WriteServoHand(HandPut);
      delay(550);
      WriteServoHand(HandRemove);
      delay(550);
      WriteServoBox(BoxClose);
      delay(1500);

      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      delay(1500);
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      }
      else {
        digitalWrite(LedBL , HIGH );
      }
      delay(1000);
      WriteServoBox(BoxClose);

    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  TERZA ANIMAZIONE                -------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------



    else if (action == 3)
    {
      Serial.println("Action 3");
      if (Terror == LOW) {
        PlayAudio((char *)"puertaL2.wav" , VolAudio);
      }
      else {
        PlayAudio((char *)"timbre.wav" , VolAudio);
        delay(2000);
        digitalWrite(LedBL , HIGH );

      }

      for (int i = BoxClose; i <= BoxOpen; i++) {
        if (Terror == LOW) {
          digitalWrite(LedBL , HIGH );
          digitalWrite(LedRJ , LOW );
          delay(50);
        }
        WriteServoBox(i);
        delay(6);
        if (Terror == LOW) {
          digitalWrite(LedBL , LOW );
          digitalWrite(LedRJ , HIGH );
          delay(50);
        }
      }
      Fix_Servo();
      StopAudio();
      delay(500);
      WriteServoBox(BoxClose);
      if (Terror == LOW) {
        digitalWrite(LedRJ , LOW );
      }
      else {
        digitalWrite(LedBL , LOW );
      }
      delay(2000);
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      }
      Fix_Servo();
      WriteServoHand(HandPut);
      delay(550);
      WriteServoHand(HandRemove);
      delay(500);
      WriteServoBox(BoxClose);

    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  QUARTA ANIMAZIONE                ------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------



    else if (action == 4)
    {
      Serial.println("Action 4");
      PlayAudio((char *)"tictac.wav" , VolAudio);
      digitalWrite(LedRJ , HIGH );
      digitalWrite(LedBL , HIGH );
      delay(5000);
      StopAudio();
      delay(3000);
      WriteServoBox(BoxOpen);
      Fix_Servo();
      for (int i = HandRemove; i >= 35; i--)
      {
        WriteServoHand(i);
        delay(20);
      }
      delay(2000);
      WriteServoHand(HandPut);
      delay(200);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);

    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  QUINTA ANIMAZIONE                ------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 5)
    {
      Serial.println("Action 5");

      digitalWrite(LedBL , HIGH );
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      delay(1500);
      WriteServoHand(65);
      delay(550);
      WriteServoHand(40);
      delay(200);
      WriteServoHand(65);
      delay(200);
      WriteServoHand(40);
      delay(200);
      WriteServoHand(65);
      delay(200);
      WriteServoHand(40);
      delay(200);
      WriteServoHand(65);
      delay(200);
      WriteServoHand(40);
      delay(200);
      WriteServoHand(65);
      delay(200);
      WriteServoHand(40);
      delay(200);
      WriteServoHand(65);
      delay(2000);
      WriteServoHand(HandPut);
      delay(400);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);
      digitalWrite(LedBL , LOW );
      delay(3000);
      if (Terror == LOW) {
        PlayAudio((char *)"nudillos.wav" , VolAudio);
        delay(2000);
        PlayAudio((char *)"nudillos.wav" , VolAudio);
        delay(2000);
      }
    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  SESTA ANIMAZIONE                -------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------

    else if (action == 6)
    {
      Serial.println("Action 6");
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      }

      for (int i = BoxClose; i < BoxOpen; i = i + 2)
      {
        WriteServoBox(i);
        if (Terror == LOW) {
          digitalWrite(LedBL , LOW );
          digitalWrite(LedRJ , HIGH );
          delay(100);
        }
        digitalWrite(LedRJ , LOW );
        digitalWrite(LedBL , HIGH );
        delay(100);
      }

      Fix_Servo();
      delay(500);
      if (Terror == LOW) {
        digitalWrite(LedRJ , LOW );
      }

      for (int i = 0; i < 12; i++)
      {
        for (int j = BoxClose; j <= BoxOpen; j++) {
          WriteServoBox(j);
          delay(6);
        }
        delay(200);
      }
      Fix_Servo();
      for (int i = HandRemove; i >= 35; i--)
      {
        WriteServoHand(i);
        delay(30);
      }
      WriteServoHand(HandPut);
      delay(400);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);
      delay(2000);
      Meneo();
      delay(4000);
      if (Terror == LOW) {

        PlayAudio((char *)"perro2.wav" , VolAudio);
        digitalWrite(LedRJ , HIGH );

      }
      else {
        PlayAudio((char *)"perrop.wav" , VolAudio);
        digitalWrite(LedBL , HIGH );
      }

      delay(3000);
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      delay(2500);
      WriteServoBox(80);

    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  SETTIMA ANIMAZIONE                -----------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------

    else if (action == 7)
    {
      Serial.println("Action 7");
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      WriteServoHand(HandPut);
      delay(1000);
      digitalWrite(LedBL , HIGH );
      for (int i = 0; i < 6; i++)
      {
        if (Terror == LOW) {
          digitalWrite(LedRJ , LOW );
          digitalWrite(LedBL , HIGH );
        }
        for (int j = BoxClose; j <= BoxOpen; j++) {
          WriteServoBox(j);
          delay(6);
        }
        if (Terror == LOW) {
          digitalWrite(LedRJ , HIGH );
          digitalWrite(LedBL , LOW );
        }

        delay(200);
      }
      digitalWrite(LedBL , LOW );
      delay(500);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);
      delay(1000);
      Meneo();

    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  OTTAVA ANIMAZIONE                ------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 8)
    {
      Serial.println("Action 8");
      if (Terror == HIGH) {
        digitalWrite(LedBL , HIGH );

      }
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      WriteServoHand(HandPut + 10);
      delay(2000);
      for (int i = 0; i < 7; i++)
      {
        WriteServoHand(75);
        delay(100);
        WriteServoHand(40);
        delay(100);
      }
      delay(500);
      WriteServoHand(HandRemove);
      delay(1000);
      WriteServoBox(BoxClose);
      delay(2000);
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );

      }
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);

        delay(0.5);
      }
      WriteServoHand(HandPut);
      delay(300);
      WriteServoHand(HandRemove);
      delay(30);
      WriteServoBox(BoxClose);


    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  NONA ANIMAZIONE                --------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------

    else if (action == 9)
    {
      Serial.println("Action 9");

      if (Terror == LOW) {
        PlayAudio((char *)"campana.wav" , VolAudio);
        delay(8000);

      }

      for (int i = BoxClose; i <= 105; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      delay(1500);
      WriteServoBox(BoxOpen);
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      };
      Fix_Servo();
      for (int i = HandRemove; i >= 35; i--)
      {
        WriteServoHand(i);
        delay(40);
      }
      delay(500);
      WriteServoHand(HandPut);
      delay(200);
      for (int i = HandPut; i <= HandRemove; i++)
      {
        WriteServoHand(i);
        delay(40);
      }
      WriteServoBox(BoxClose);
      delay(2000);
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
        analogWrite(MotorShake , shakespeed );
      } else {
        digitalWrite(LedBL , HIGH );
      }
      WriteServoBox(BoxClose + 30);
      Fix_Servo();
      delay(1000);
      if (Terror == LOW) {
        analogWrite(MotorShake , LOW );
      }
      delay(1500);
      WriteServoBox(BoxClose);

    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  DECIMA ANIMAZIONE                ------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 10)
    {
      Serial.println("Action 10");
      PlayAudio((char *)"cristal.wav" , VolAudio);
      for (int i = BoxClose; i <= 105; i++)
      {
        WriteServoBox(i);
        delay(100);
      }
      delay(1000);
      for (int i = 0; i < 10; i++)
      {
        for (int j = BoxClose; j <= 100; j++) {
          WriteServoBox(j);
          digitalWrite(LedBL , LOW );
          delay(6);
        }
        digitalWrite(LedBL , HIGH );
        delay(100);
      }
      WriteServoBox(BoxOpen);
      Fix_Servo();
      WriteServoHand(HandPut);
      delay(650);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);

    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  UNDICESIMA ANIMAZIONE                --------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 11)
    {
      Serial.println("Action 11");
      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }

      Fix_Servo();

      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
        PlayAudio((char *)"cucu2.wav" , VolAudio);
        delay(1000);
        digitalWrite(LedRJ , LOW );

      } else {
        digitalWrite(LedBL , HIGH );
        PlayAudio((char *)"donde.wav" , VolAudio);
        delay(2000);
        digitalWrite(LedBL , LOW );
      }

      delay(400);
      WriteServoBox(BoxClose);
      delay(1500);


      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }

      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
        PlayAudio((char *)"bu.wav" , VolAudio);
        delay(1000);


      } else {
        digitalWrite(LedBL , HIGH );
        PlayAudio((char *)"aqui.wav" , VolAudio);
        delay(1000);

      }

      Fix_Servo();


      WriteServoHand(HandPut);
      delay(1000);
      WriteServoHand(HandRemove);
      delay(400);
      digitalWrite(LedBL , LOW );
      WriteServoBox(BoxClose);

    }


    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  DODICESIMA ANIMAZIONE                -------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 12)
    {
      Serial.println("Action 12");
      int mid_box = BoxClose + (BoxOpen - BoxClose) / 2;

      for (int i = BoxClose; i < mid_box; i++)
      {
        WriteServoBox(i);
        if (Terror == LOW) {
          digitalWrite(LedRJ , HIGH );
          delay(100);
          digitalWrite(LedRJ , LOW );
          delay(100);
        }
        else {
          delay(10);
          digitalWrite(LedBL , HIGH );

        }

      }
      for (int i = mid_box; i <= BoxOpen; i++)
      {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      delay(500);
      for (int i = HandRemove; i >= (HandPut + 15); i--)
      {
        WriteServoHand(i);
        delay(50);
      }
      delay(2000);
      WriteServoHand(HandPut);
      Meneo();
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
        digitalWrite(LedBL , HIGH );
      }
      delay(200);
      WriteServoHand(HandRemove);
      delay(400);
      WriteServoBox(BoxClose);

    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  TREDICESIMA ANIMAZIONE                -------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 13)
    {
      Serial.println("Action 13");
      WriteServoBox(BoxOpen);

      Fix_Servo();
      delay(1500);
      for (int i = HandRemove; i >= HandRemove - 30; i--)
      {
        WriteServoHand(i);
        delay(1);
      }
      for (int i = 0; i < 3; i++)
      {
        WriteServoHand(HandPut + 20);
        delay(1000);
        WriteServoHand(HandRemove - 20);
        delay(2000);
      }
      delay(1000);
      WriteServoHand(HandPut);
      if (Terror == LOW) {
        PlayAudio((char *)"perro1.wav" , VolAudio);
        digitalWrite(LedRJ , HIGH );

      }
      else {
        PlayAudio((char *)"perrop.wav" , VolAudio);
        digitalWrite(LedBL , HIGH );
      }
      delay(3000);
      WriteServoHand(HandRemove);
      delay(500);
      WriteServoBox(BoxClose);
      delay(2000);


    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------                  QUATTORDICESIMA ANIMAZIONE                ---------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------


    else if (action == 14)
    {
      Serial.println("Action 14");
      if (Terror == LOW) {
        PlayAudio((char *)"trueno.wav" , VolAudio);
      } else
      {
        PlayAudio((char *)"agua.wav" , VolAudio);
      }

      WriteServoBox(BoxClose + 25);
      Fix_Servo();

      if (Terror == LOW) {
        for (int i = 1; i <= 50; i++)
        {

          digitalWrite(LedRJ , HIGH );
          digitalWrite(LedBL , LOW );
          delay(70);
          digitalWrite(LedRJ , LOW );
          digitalWrite(LedBL , HIGH );
          delay(70);



        }
        digitalWrite(LedRJ , LOW );
        digitalWrite(LedBL , LOW );

      } else
      {
        delay(5000);
        digitalWrite(LedBL , HIGH );
      }



      delay(2000);

      WriteServoBox(BoxOpen);
      Fix_Servo();
      WriteServoHand(HandPut);
      delay(650);
      WriteServoHand(HandRemove);
      delay(500);
      WriteServoBox(BoxClose + 25);
      Fix_Servo();
      delay(4000);
      WriteServoBox(BoxClose);

    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------               QUICEAVA ANIMACION             -----------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------

    else if (action == 15)
    {
      Serial.println("Action 15");
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      } else {
        digitalWrite(LedBL , HIGH );
      }
      for (int i = BoxClose; i <= BoxClose + 25; i++) {
        WriteServoBox(i);
        delay(6);
      }
      Fix_Servo();
      digitalWrite(LedBL , LOW );
      digitalWrite(LedRJ , LOW );
      delay(500);
      for (int i = 0; i < 3; i++)
      {
        for (int j = BoxClose; j <= BoxClose + 30; j++) {
          if (Terror == LOW) {
            digitalWrite(LedRJ , LOW );
          } else {
            digitalWrite(LedBL , HIGH );
          }
          WriteServoBox(j);
          delay(50);
        }
        for (int j = BoxClose + 30; j >= BoxClose; j--)
        {
          if (Terror == LOW) {
            digitalWrite(LedRJ , HIGH );
          } else {
            digitalWrite(LedBL , LOW );
          }
          WriteServoBox(j);
          delay(50);
        }
      }
      digitalWrite(LedBL , LOW );
      digitalWrite(LedRJ , LOW );
      for (int j = BoxClose; j <= BoxOpen; j++) {
        WriteServoBox(j);
        delay(50);
      }
      Fix_Servo();
      for (int i = HandRemove; i >= HandRemove + 35; i--)
      {
        WriteServoHand(i);
        delay(40);
      }
      delay(1000);
      WriteServoHand(HandPut);
      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      } else {
        digitalWrite(LedBL , HIGH );
      }
      delay(400);
      WriteServoHand(HandRemove);
      digitalWrite(LedBL , LOW );
      digitalWrite(LedRJ , LOW );
      delay(400);
      WriteServoBox(BoxClose);
    }

    // ----------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------           DIECISEISAVA ANIMACION             -----------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------
    else if (action == 16)
    {
      Serial.println("Action 16");

      for (int i = BoxClose; i <= BoxOpen; i++) {
        WriteServoBox(i);
        delay(6);
      }

      if (Terror == LOW) {
        digitalWrite(LedRJ , HIGH );
      } else {
        digitalWrite(LedBL , HIGH );
      }
      WriteServoHand(HandPut);
      Fix_Servo();
      delay(2000);
      WriteServoHand(HandRemove);
      delay(150);
      WriteServoBox(BoxClose);
      delay(2500);



      for (int i = 1; i <= 3; i++) {
        if (i % 2 != 0) {
          delay(200);
          PlayAudio((char *)"leon.wav" , VolAudio);
        }



        int a = 0;
        int calcboxpos = 0;
        calcboxpos = HandRemove - 50 - HandPut;
        calcboxpos = calcboxpos / (BoxOpen - (BoxClose + 20));
        int boxpos = 0;
        for (int j = HandRemove - 50; j >= HandPut; j--) {
          a++;
          WriteServoHand(j);
          delay(1);
          boxpos = BoxClose + 20 + (a / calcboxpos);
          if (boxpos <= BoxOpen) {
            WriteServoBox(boxpos);
          }

        }
        if (Terror == LOW) {
            digitalWrite(LedRJ , HIGH );
          } else {
            digitalWrite(LedBL , HIGH );
          }
        Fix_Servo();
        delay(2000);
        digitalWrite(LedBL , LOW);
        digitalWrite(LedRJ , LOW);
        a = 0;
        for (int j = HandPut; j <= HandRemove - 50; j++) {
          a++;
          WriteServoHand(j);
          delay(16);
          boxpos = BoxOpen - (a / calcboxpos);
          if (boxpos >= BoxClose + 20) {
            WriteServoBox(boxpos);
          }

        }
        delay(500);
        StopAudio();

        WriteServoHand(HandRemove);
        delay(10);
        WriteServoBox(BoxClose);
      }


    }
    DefaultOuput();
    randomSeed(analogRead(0));
    action = random(1, 17);
    countLoops = 0 ;
  }


  if (countLoops <= Sleep_Time )
  {
    countLoops++;
  }
  else
  {
    countLoops = 0 ;
    EnterSleep();
  }

}
