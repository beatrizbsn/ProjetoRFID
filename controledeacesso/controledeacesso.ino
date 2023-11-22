#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9     
#define SS_PIN 53     

MFRC522 mfrc522(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;
int bloco;  
byte tam;
byte buffer[18];
String uid = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; // sua chave 
  bloco = 1; // seu bloco
  tam = 18;
}

void loop() {
  // Verifica se uma tag está presente
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Lê a UID da tag
    
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

   if (VerificarUID()){
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
       }
      status = mfrc522.MIFARE_Read(bloco, buffer, &tam);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
      }
      if (VerificarDado()){
        Abrir();
      }else {
        return ;
      }
    } else {
        return;
    }

    delay(1000);  // Aguarda 1 segundo para evitar leituras múltiplas
  }
}
