#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          53          

MFRC522 mfrc522(SS_PIN, RST_PIN);  
int trailer;
void setup() {
  Serial.begin(9600);       
  SPI.begin();               
  mfrc522.PCD_Init();        
  trailer = 3;
}

void loop() {

 
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

 while ( trailer <= 63){ 
  
 // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailer, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }else Serial.println(F("PCD_Authenticate() success "));

    byte trailerBuffer[] = {
        254, 255, 255, 255, 255, 255,       // atualizacao key A
        0, 0, 0,
        0,
        255, 255, 255, 255, 255, 255};      // atualizacao key B
   
    //      g[0]    acesso bits bloco 0 
    //      g[1]    acesso bits bloco 1 
    //      g[2]    acesso bits bloco 2 
    //      g[3]    acesso bits do Trailer
    //      mfrc522.MIFARE_SetAccessBits(&trailerBuffer[6],g[0], g[1], g[2], g[3]);
   
    mfrc522.MIFARE_SetAccessBits(&trailerBuffer[6], 0, 0, 0, 1); // definindo acesso (decimal)

    status = mfrc522.MIFARE_Write(trailer, trailerBuffer, 16);
    if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
    }else Serial.println(F("MIFARE_Write() success "));

    trailer = trailer + 4;
  }
  
}
