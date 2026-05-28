// =========================================================================
// PROYECTO: CONTROL DE TEMPERATURA POR PWM (FRECUENCIA VARIABLE POR SOFTWARE)
// HARDWARE: Arduino Mega 2560 + LCD 20x4 I2C + Driver + 1 MOSFET (IRFZ44N)
// =========================================================================

#include <Arduino.h>
#include <Wire.h>               // Librería necesaria para la comunicación I2C
#include <LiquidCrystal_I2C.h>  // Librería específica para el módulo de la pantalla

// --- CONFIGURACIÓN GLOBAL DE FRECUENCIA ---
const float FRECUENCIA_DESEADA = 20.0; 

// --- CONFIGURACIÓN DE HARDWARE ---
// Inicialización de LCD I2C: (Dirección I2C, Columnas, Filas)
// La dirección típica de estos módulos es 0x27 o 0x3F.
LiquidCrystal_I2C lcd(0x27, 20, 4); 

const int pinMOSFET = 11; // OC1A (Pin de salida PWM de hardware del Timer 1)
const int potPin = A0;    // Pin analógico para el potenciómetro

// Variables calculadas dinámicamente en el setup()
unsigned int valorTOP = 0; 

void setup() {
  pinMode(pinMOSFET, OUTPUT);

  // 1. RECALCULAR EL VALOR TOP SEGÚN LA FRECUENCIA CONFIGURADA (Prescaler = 256)
  valorTOP = (unsigned int)(16000000.0 / (2.0 * 256.0 * FRECUENCIA_DESEADA));

  // 2. CONFIGURACIÓN DEL TIMER 1 (16 BITS) DEL ATMEGA2560
  TCCR1A = 0; 
  TCCR1B = 0;

  // Modo 10: Phase Correct PWM con TOP definido por ICR1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13);

  // Configurar canal A (OC1A - Pin 11) en modo No Invertido
  TCCR1A |= (1 << COM1A1);

  // Configurar el Prescaler en 256
  TCCR1B |= (1 << CS12);

  // Cargar el valor TOP en el registro ICR1 para fijar la frecuencia
  ICR1 = valorTOP;

  // 3. INICIALIZACIÓN DE LA PANTALLA LCD 20x4 MEDIANTE I2C
  lcd.init();          // Inicializa el módulo I2C
  lcd.backlight();     // Enciende la luz de fondo de la pantalla
  
  lcd.setCursor(0, 0);
  lcd.print("   CTRL TEMP PWM    ");
  lcd.setCursor(0, 1);
  lcd.print("  SISTEMA ACTIVO    ");
  delay(1500);
  lcd.clear();
}

void loop() {
  // Leer el valor del potenciómetro (0 a 1023)
  int potValue = analogRead(potPin);

  // Mapear el potenciómetro dinámicamente al valor TOP calculado
  int dutyCycle = map(potValue, 0, 1023, 0, valorTOP);
  
  // Actualizar el registro de comparación para cambiar el ancho del pulso (Potencia)
  OCR1A = dutyCycle;

  // Calcular el porcentaje real de potencia entregada
  float porcentajePotencia = ((float)potValue / 1023.0) * 100.0;

  // --- INTERFAZ VISUAL EN LCD 20x4 I2C ---
  lcd.setCursor(0, 0);
  lcd.print("--- ESTADO PWM ---");

  lcd.setCursor(0, 1);
  lcd.print("Freq. Prog: ");
  lcd.print(FRECUENCIA_DESEADA, 1);
  lcd.print(" Hz");

  lcd.setCursor(0, 2);
  lcd.print("Valor TOP : ");
  lcd.print(valorTOP);

  lcd.setCursor(0, 3);
  lcd.print("Potencia  : ");
  lcd.print(porcentajePotencia, 1);
  lcd.print("%   ");

  delay(200); // Retraso de refresco para evitar parpadeos molestos
}