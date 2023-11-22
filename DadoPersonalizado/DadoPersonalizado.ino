#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9        
#define SS_PIN          53         

MFRC522 mfrc522(SS_PIN, RST_PIN);  
MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;
void setup() {
  Serial.begin(9600);        
  SPI.begin();              
  mfrc522.PCD_Init();  
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; // sua chave       
}

void loop() {

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  byte buffer[17];
  byte block;
  byte len;

  Serial.setTimeout(20000L) ;     
  
  Serial.println(F("Escreva a mensagem e finalize com #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 17) ; 
  for (byte i = len; i < 17; i++) buffer[i] = ' ';     

  block = 1; // seu bloco 
   
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
}
