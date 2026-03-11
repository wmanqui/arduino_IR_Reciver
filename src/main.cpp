/******************************************************************************
 *                                                                             *
 *                          ANALIZADOR IR AVANZADO                             *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  Autor      : Walter Manqui                                                 *
 *  Fecha      : 11/03/2026                                                    *
 *  Versión    : 1.0                                                           *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  DESCRIPCIÓN                                                                *
 *                                                                             *
 *  Este programa utiliza la librería Arduino-IRremote (v4.x) para analizar    *
 *  señales de controles remotos infrarrojos.                                  *
 *                                                                             *
 *  El sistema detecta automáticamente el protocolo IR recibido y muestra      *
 *  en el monitor serie:                                                       *
 *                                                                             *
 *    • Protocolo detectado                                                    *
 *    • Address                                                                *
 *    • Command                                                                *
 *    • Número de bits                                                         *
 *    • Datos RAW en hexadecimal                                               *
 *    • Timings RAW de la señal                                                *
 *                                                                             *
 *  Además genera automáticamente el código necesario para retransmitir la    *
 *  señal desde Arduino.                                                       *
 *                                                                             *
 *  Cada vez que se recibe un comando IR, un LED conectado al pin 8            *
 *  parpadea como indicador visual de recepción.                               *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  LIBRERÍAS NECESARIAS                                                       *
 *                                                                             *
 *  Arduino-IRremote                                                           *
 *  https://github.com/Arduino-IRremote/Arduino-IRremote                       *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  PINES UTILIZADOS                                                           *
 *                                                                             *
 *  Pin 11  → Receptor IR                                                      *
 *  Pin 8   → LED indicador de recepción IR                                    *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  HARDWARE NECESARIO                                                         *
 *                                                                             *
 *  • Receptor IR (VS1838, TSOP4838 o similar)                                 *
 *  • LED + resistencia 220Ω                                                   *
 *                                                                             *
 *---------------------------------------------------------------------------- *
 *  NOTAS                                                                      *
 *                                                                             *
 *  Compatible con Arduino UNO, Nano, Mega, ESP32 y derivados.                *
 *                                                                             *
 *  Protocolos detectables (entre otros):                                      *
 *    NEC, RC5, RC6, SONY, SAMSUNG, LG, PANASONIC, JVC, DENON, SHARP           *
 *                                                                             *
 ******************************************************************************/



#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11
#define LED_IR 8

//------------------------------------------------
// PARPADEO LED (no bloqueante)
//------------------------------------------------
void blinkIR() {

  digitalWrite(LED_IR, HIGH);
  delay(30);
  digitalWrite(LED_IR, LOW);
}


//------------------------------------------------
// IMPRIMIR CODIGO PARA TRANSMITIR
//------------------------------------------------
void codeToSend() {

  Serial.println("----- CODIGO PARA TRANSMITIR -----");

  Serial.print("IrSender.send");
  Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
  Serial.print("(");

  Serial.print(IrReceiver.decodedIRData.address);
  Serial.print(", ");

  Serial.print(IrReceiver.decodedIRData.command);
  Serial.println(", 0);");

  Serial.println("----------------------------------");
}


//------------------------------------------------
// MOSTRAR INFORMACION IR
//------------------------------------------------
void printReceivedData() {

  Serial.println();
  Serial.println("=========== SEÑAL IR ===========");

  Serial.print("Protocolo: ");
  Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

  Serial.print("Address : ");
  Serial.println(IrReceiver.decodedIRData.address);

  Serial.print("Command : ");
  Serial.println(IrReceiver.decodedIRData.command);

  Serial.print("Bits : ");
  Serial.println(IrReceiver.decodedIRData.numberOfBits);

  Serial.print("Raw HEX : 0x");
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
}


//------------------------------------------------
// SETUP
//------------------------------------------------
void setup() {

  Serial.begin(115200);

  pinMode(LED_IR, OUTPUT);

  Serial.println();
  Serial.println("=================================");
  Serial.println("      ANALIZADOR IR AVANZADO");
  Serial.println("=================================");

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}


//------------------------------------------------
// LOOP PRINCIPAL
//------------------------------------------------
void loop() {

  if (!IrReceiver.decode())
    return;

  blinkIR();

  printReceivedData();

  codeToSend();

  Serial.println("----- TIMINGS RAW -----");
  IrReceiver.printIRResultRawFormatted(&Serial, true);

  Serial.println("----- INFO CORTA -----");
  IrReceiver.printIRResultShort(&Serial);

  Serial.println("----------------------");

  IrReceiver.resume();
}