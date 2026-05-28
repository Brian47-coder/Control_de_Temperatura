# Sistema de Control de Temperatura por PWM de Hardware - ATmega2560

Este proyecto implementa un sistema de control de potencia para una resistencia calefactora utilizando técnicas de modulación por ancho de pulso (PWM) por hardware de bajo nivel en un microcontrolador ATmega2560 (Arduino Mega). 

El sistema permite configurar de manera modular la frecuencia de conmutación mediante software y visualizar los parámetros en tiempo real a través de una interfaz de pantalla LCD gestionada por el protocolo de comunicación I2C.

## 💡 Características Principales

* **PWM por Hardware de 16 bits:** Utilización del **Timer 1** en Modo 10 (*Phase Correct PWM* con TOP en `ICR1`) para garantizar una señal limpia, libre de las interrupciones del bucle principal (`loop()`).
* **Frecuencia Configurable por Software:** Cálculo dinámico del valor `TOP` en el bloque de inicialización para operar en frecuencias óptimas de baja conmutación térmica (ej. 20 Hz), reduciendo las pérdidas de energía en los transistores.
* **Etapa de Potencia Robusta:** Control de un único MOSFET de canal N (`IRFZ44N`) mediante un driver de compuerta (*Gate Driver*) dedicado `IR2110` para un disparo seguro y eficiente a 12V DC.
* **Interfaz Visual Dinámica:** Monitoreo en pantalla LCD 20x4 mediante bus **I2C** (pines 20/21) mostrando el ciclo de trabajo (*Duty Cycle*) actual en porcentaje, la frecuencia programada y el valor de registro `TOP`.

## 🛠️ Especificaciones de Hardware y Conexiones

* **Microcontrolador:** Arduino Mega 2560 (Salida PWM por pin hardware `OC1A` / Pin Digital 11).
* **Manejo de Potencia:** Driver IR2110 de baja conmutación comandando el Gate del MOSFET IRFZ44N.
* **Filtros y Protección:** Masa unificada estrella (GND Arduino / Negativo 12V de Potencia), red de Pull-Down de $10\text{ k}\Omega$ y resistencia de compuerta serie de $330\ \Omega$ (optimizada para la atenuación de picos).
* **Retroalimentación Visual:** LED testigo en paralelo con la carga resistiva conectado al Drenador (*Drain*).
