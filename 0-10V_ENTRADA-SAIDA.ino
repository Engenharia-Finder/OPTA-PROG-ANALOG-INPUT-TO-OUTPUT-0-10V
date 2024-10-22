#include "OptaBlue.h"

#define PERIODIC_UPDATE_TIME 200
#define DELAY_AFTER_SETUP 200
#define INPUT_PIN A0 // Defina o pino analógico de entrada

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
  /* -------------------------------------------------------------------------- */
  if (t == EXPANSION_NOT_VALID) {
    Serial.print("Unknown!");
  } else if (t == EXPANSION_OPTA_DIGITAL_MEC) {
    Serial.print("Opta --- DIGITAL [Mechanical]  ---");
  } else if (t == EXPANSION_OPTA_DIGITAL_STS) {
    Serial.print("Opta --- DIGITAL [Solid State] ---");
  } else if (t == EXPANSION_DIGITAL_INVALID) {
    Serial.print("Opta --- DIGITAL [!!Invalid!!] ---");
  } else if (t == EXPANSION_OPTA_ANALOG) {
    Serial.print("~~~ Opta  ANALOG ~~~");
  } else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo() {
  /* -------------------------------------------------------------------------- */
  static long int start = millis();

  if (millis() - start > 5000) {
    start = millis();
    Serial.print("Number of expansions: ");
    Serial.println(OptaController.getExpansionNum());

    for (int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.print("Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(OptaController.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(OptaController.getExpansionI2Caddress(i));
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
  /* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();

  // Configurar o canal DAC para saída de tensão
  for (int device = 0; device < OptaController.getExpansionNum(); device++) {
    for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      // Configure os canais ímpares como DAC de tensão
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         device,
                                         ch,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);
    }
  }

  pinMode(INPUT_PIN, INPUT); // Defina o pino de entrada como INPUT
}

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
  /* -------------------------------------------------------------------------- */
  static long int start = millis();

  // Execute esta parte a cada PERIODIC_UPDATE_TIME milissegundos
  if (millis() - start > PERIODIC_UPDATE_TIME) {
    start = millis();

    
    uint16_t input_value = analogRead(INPUT_PIN);

    // Converter o valor lido para a escala de 0 a 6000 (usada pelo DAC)
    uint16_t dac_value = map(input_value, 0, 1023, 0, 7490);

    // Enviar o valor para o DAC
    for (int i = 0; i < OptaController.getExpansionNum(); i++) {
      AnalogExpansion exp = OptaController.getExpansion(i);
      if (exp) {
        Serial.println("Setting DAC value " + String(dac_value) + " on expansion n. " + String(exp.getIndex()));
        exp.setDac(OA_CH_7, dac_value);
      }
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
  /* -------------------------------------------------------------------------- */
  OptaController.update();
  printExpansionInfo();
  optaAnalogTask();
}
