/*
            Balança de Precisão para a Gilda
                Componentes:
                - Arduino Nano
                - Display LCD I2C
                - Modulo HX711 Amplificador e conversor A/D
                - Sensor de Peso

*/
// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//The library used for arduino  https://github.com/bogde/HX711
// LCD can also be used instead of serial 
#include "HX711.h"

#define DOUT 5
#define CLK 6

HX711 scale(DOUT, CLK);

// --- Declaração de Objetos ---
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

// --- Variáveis Globais ---
float weight;
float calibration_factor = 90; // é o valor que foi encontrado usando o hx711_calibration

// --- Configurações Iniciais ---
void setup() {
  lcd.begin (16,2);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Inicializando...");

  Serial.begin(9600);
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t\t");
  Serial.println(scale.read());     // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

  lcd.clear();
  lcd.print("Setando tara ...");

  Serial.print("get_scale: \t\t\t");Serial.println(scale.get_scale());
  scale.set_scale(calibration_factor);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
  Serial.println("Calibration");
  Serial.println(scale.get_units(10), 1);
  Serial.print("get_scale: \t\t\t");Serial.println(scale.get_scale());
  
  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  lcd.clear();
  lcd.print("Peso:           ");
  lcd.setCursor(0,1);
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.print(scale.get_units(20), 1);

  Serial.print("\t| Weight: \t");                        
  /* ---------Weight in terms of KG-------------*/
  weight = (scale.get_units(20)*0.001);
  Serial.print(weight, 3);
  Serial.println(" Kg");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(weight, 3);
  lcd.print(" Kg");

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
    scale.tare();
  }
  
  scale.power_down();             // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}
