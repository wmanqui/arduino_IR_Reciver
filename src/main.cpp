#include <Arduino.h>
#include <IRremote.hpp>

#define RECV_PIN 11
#define LED_PIN 13

#define RELE1 4
#define RELE2 5
#define RELE3 6

#define BLOQUEO 500

unsigned long ultimoTiempo = 0;

bool estadoRele1 = false;
bool estadoRele2 = false;
bool estadoRele3 = false;

void setup() {

  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(RELE3, OUTPUT);

  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);

  // Inicializa receptor IR
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Sistema listo...");
}

void loop() {

  if (IrReceiver.decode()) {

    unsigned long ahora = millis();

    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);

    if (IrReceiver.decodedIRData.protocol == SONY &&
        ahora - ultimoTiempo > BLOQUEO) {

      uint16_t codigo = IrReceiver.decodedIRData.command;

      Serial.print("Codigo recibido: ");
      Serial.println(codigo);

      switch (codigo) {

        case 656:
          estadoRele1 = !estadoRele1;
          digitalWrite(RELE1, estadoRele1);
          Serial.println("Tecla Muting: Rele_1");
          break;

        case 2640:
          estadoRele2 = !estadoRele2;
          digitalWrite(RELE2, estadoRele2);
          Serial.println("Tecla Input: Rele_2");
          break;

        case 2704:
          estadoRele3 = !estadoRele3;
          digitalWrite(RELE3, estadoRele3);
          Serial.println("Tecla Power: Rele_3");
          break;
      }

      ultimoTiempo = ahora;
    }

    IrReceiver.resume();
  }
}