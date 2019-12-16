/* ---------------Proyecto Version 1.0 - Diciembre 2019 --------------
 *  REalizado por m@tyas
 *  Se usa una pantalla 20x4 i2c
 *  Un modulo de rele
 *  un lector RFiD RC522
 *  
 *  La pantalla utiliza los puertos A4 para SCL y A5 para SDA, va a 5v
 *  El rele utiliza en este cas el puerto Digital 8 y va a 5v
 *  el RFID utiliza los puertos D9  Reset
 *                              D10 SDA
 *                              D11 MOSI
 *                              D12 MISO
 *                              D13 SCK
 */


#include <SPI.h>
#include <MFRC522.h>
//
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
//
LiquidCrystal_I2C lcd(0x27,20,4);  //

       byte validKey1[4] = { 0x2D, 0x2F, 0xBA, 0x79 };  // Ejemplo de clave valida
       byte validKey2[4] = { 0x90, 0x54, 0x13, 0x13 };  // Ejemplo de clave valida
       
      bool isEqualArray(byte* arrayA, byte* arrayB, int length)
      {
        for (int index = 0; index < length; index++)
        {
          if (arrayA[index] != arrayB[index]) return false;
        }
        return true;
      }

void setup() {
  Serial.begin(9600); //Iniciamos la comunicación  serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  Serial.println("Lectura del UID");

 //
   lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  

  lcd.begin(20,4); // Initialize LCD
  lcd.setCursor(0,0); // Set the cursor at the 4th column and 1st row
  lcd.print("Prueba de lectura");

    // Definimos reles:
  #define RELAY_ON 0    
  #define RELAY_OFF 1
  //iniciamos el rele apagado
  digitalWrite (8, RELAY_OFF);
  //definimos el pin de salida
  pinMode (8, OUTPUT);
}

void loop() {
    lcd.setCursor(0,1);
    lcd.print("Acerque una tarjeta");
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
               if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4))
                 { Serial.println("Tarjeta valida");
                  lcd.setCursor(0,1);
                  lcd.print(" Hola usuario 1   ");
                  digitalWrite (8, RELAY_ON);
                  delay(2000);
                  }

                 if (isEqualArray(mfrc522.uid.uidByte, validKey2, 4))
                 { Serial.println("Tarjeta valida");
                  lcd.setCursor(0,1);
                  lcd.print("Hola usuario 2      ");
                  digitalWrite (8, RELAY_ON);
                  delay(2000);
                  }
             else 
                  {Serial.println("Tarjeta invalida");
                  lcd.setCursor(0,1);
                  lcd.print("No sos Maty         ");
                  delay(1000);
                   Serial.print("Card UID:");
                                for (byte i = 0; i < mfrc522.uid.size; i++) {
                                        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                                        Serial.print(mfrc522.uid.uidByte[i], HEX);   
                                } 
                               Serial.println();
                  }
                  
             
              
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();   
                    
            }      
  }  
   
    digitalWrite (8, RELAY_OFF);
}
