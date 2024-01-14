#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

int ledRojo = 6;
int buzzer = 7;
int ledVerde = 1;
int servoMotor = 13;

Servo myservo; // Crear el objeto Servo

#define clave_correcta "DB1945"
int cantidad_letras = 1;
int posicion = 5;
String clave_ingresada = "";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {A5, A4, A3, A2};
byte colPins[COLS] = {A1, 10, 9, 8};
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
    pinMode(buzzer, OUTPUT);
    pinMode(ledRojo, OUTPUT);
    pinMode(ledVerde, OUTPUT);

    myservo.attach(servoMotor);
    myservo.write(0);

    lcd.begin(16, 2);
    lcd.print("Bienvenido Iesus");
    delay(2000);
    lcd.clear();
}

void loop()
{
    char key = teclado.getKey();

    for (posicion; posicion <= 10; posicion++)
    {
        lcd.setCursor(0, 0);
        lcd.print("Inserte codigo:");
        key = teclado.waitForKey();
        cantidad_letras += 1;
        lcd.setCursor(posicion, 1);
        lcd.print("*");
        clave_ingresada = clave_ingresada + key;
    }
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    if (clave_ingresada == clave_correcta)
    {
        digitalWrite(ledVerde, HIGH);
        myservo.write(90);
        delay(3000);
        myservo.write(0);
        digitalWrite(ledVerde, LOW);
    }
    else
    {
        lcd.setCursor(4, 0);
        lcd.print("Intruso");
        digitalWrite(buzzer, HIGH);
        digitalWrite(ledRojo, HIGH);
        delay(4000);
        digitalWrite(buzzer, LOW);
        digitalWrite(ledRojo, LOW);
    }
    lcd.clear();
    posicion = 5;
    clave_ingresada = "";
    setup();
}