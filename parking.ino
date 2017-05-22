#include<Servo.h>
#include <LiquidCrystal.h>

#define sEspera 1
#define sAbrir 2
#define sAbierto 3
#define sCerrar 4
#define sMantenimiento 5

Servo barrera;
int pinBarrera = 21;

int coches = 0;

int estado = sEspera;
String mensaje = "Espera";

int pinParaEntrar = 22;
int pinParaCerrar = 53;

int pinAbajo = 47;
int pinArriba = 51;
int pinMedio = 49;

int ledAbajo = 30;
int ledArriba = 28;
int ledMedio = 32;

int servo_malo = 13;
int servo_arreglado = 10;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int tiempoAbierto = 0;
int valor = 9;

boolean sefue = false;
void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Estado: ");
  lcd.setCursor(0, 1);
  lcd.print("Cantidad: 0");
  lcd.setCursor(8, 0);
  lcd.print("Bueno");

  pinMode(pinParaEntrar,INPUT);
  pinMode(pinParaCerrar,INPUT);


  pinMode(pinAbajo,INPUT);
  pinMode(pinArriba,INPUT);
  pinMode(pinMedio,INPUT);

  pinMode(ledAbajo,OUTPUT);
  pinMode(ledArriba,OUTPUT);
  pinMode(ledMedio,OUTPUT);


  pinMode(servo_malo,INPUT);
  pinMode(servo_arreglado,INPUT);

  barrera.attach(pinBarrera);
}

void loop() {

    int irEntrar =  digitalRead(pinParaEntrar);
    int irCerrar =  digitalRead(pinParaCerrar);

    int irBajo   =  digitalRead(pinAbajo);
    int irArriba =  digitalRead(pinArriba);
    int irMedio  =  digitalRead(pinMedio);


    if (digitalRead(servo_malo) == HIGH)  {
        estado = sMantenimiento;
    }

      if (digitalRead(servo_arreglado) == HIGH) {
      estado = sEspera;
    }


    if (coches >= 5) {
      mensaje = "FULL :(";
      showLCD(mensaje);
    } else {
      switch(estado){
        case sEspera:
          barreraMedia();

          mensaje = "Espera ";
          showLCD(mensaje);

          if(irEntrar == 1){
            estado = sAbrir;
          }

          break;

        case sAbrir:
          mensaje = "Carro  ";
          showLCD(mensaje);

          barreraArriba();


          if(irBajo == 0 ){
            estado = sAbierto;
          }
        break;
        case sAbierto:

          mensaje = "Abierto";
          showLCD(mensaje);

          tiempoAbierto = millis() / 1000;

          // lcd.setCursor(13, 1);
          // lcd.print(tiempoAbierto);


          if(tiempoAbierto == 9 ){
            sefue = true;
            estado = sCerrar;
          }

          if(irCerrar == 1 ){
            sefue = false;
            estado = sCerrar;
          }

        break;
        case sCerrar:
          barreraMedia();

          Serial.println(irMedio);
          if(irMedio == 0){
            estado = sEspera;
            coches = coches + 1;
            lcd.setCursor(10, 1);
            lcd.print(coches);
          }
        break;
        case sMantenimiento:
          mensaje = "Manteni";
          showLCD(mensaje);

          barreraBaja();
          if(irArriba == 1){
            estado = sMantenimiento;
          }
        break;
      }

    }
}

void barreraArriba(){
  digitalWrite(ledAbajo, LOW);
  digitalWrite(ledArriba, HIGH);
  digitalWrite(ledMedio, LOW);
  barrera.write(3);
}

void barreraMedia(){
  digitalWrite(ledAbajo, LOW);
  digitalWrite(ledArriba, LOW);
  digitalWrite(ledMedio, HIGH);
  barrera.write(15);
}

void barreraBaja(){
  digitalWrite(ledAbajo, HIGH);
  digitalWrite(ledArriba, LOW);
  digitalWrite(ledMedio, LOW);
  barrera.write(28);
}

void showLCD(String mensaje) {
  lcd.setCursor(8, 0);
  lcd.print(mensaje);
}
