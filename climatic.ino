
/*********************************************************************
Makeware -2010 -2018

Codigo base para desarrollo climatic house

sensor calidad aire
sensor humedad y temperatura

display resultados

*********************************************************************/


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT11.h>

#include "qrcode.h"

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6, 5, 4);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
/// for quality air sensor used
int adc_MQ;
float aire=0;

float tem=0, hu=0;

int pin=52;///52 mega
DHT11 dht11(pin);

//Hardware pin definitions UV
int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board

void setup() {

  
  // put your setup code here, to run once:
   Serial.begin(9600);

   init_disnokia();
 //  testrect();
    drawQR(); 
   //draw();
}

void loop() {
  // put your main code here, to run repeatedly:
sensor_ambiente();
sensor_aire();
 drawQR(); 
delay(2000);  


}

void init_disnokia(void){
  


  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(500);
  display.clearDisplay();   // clears the screen and buffer 
  
  
  }

 void testrect(void){
  
   // draw multiple rectangles
  //testfillrect();

  display.display();
  delay(60000);
  display.clearDisplay(); 
  

  
  } 

void draw(){

   int scale=4;
   
  
   for (int16_t i=0; i<scale; i++) {
    display.drawLine(0, 0+i, scale, i, BLACK);
    display.display();
  } 
  
  }


void drawQR(){
  QRCode qrcode;
  
  uint8_t qrcodeBytes[qrcode_getBufferSize(3)];
  char url[100];
  char air[6];
  char hum[6];
  char temp[6];
 
 // char url[100] = "http://climatichouse.com/core/data.php?id=1&air="; 

  dtostrf(aire, 6, 4, air);
  dtostrf(hu, 6, 4, hum);
  dtostrf(tem, 6, 4, temp);

/*
  
  strcat(airv,air);
  
  strcat(url,airv);
*/
  String message = (String)"http://climatichouse.com/core/data.php?id=1";



  message=message + "&air=";
  message=message + air; 
  message=message + "&hum=";
  message=message + hum;
  message=message + "&temp=";
  message=message + temp;
  
  
  message.toCharArray(url,100);

  
qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, url);
  
 int pos_x=25;
 int pos_y=9;

  
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
        if (qrcode_getModule(&qrcode, x, y)) {
               display.drawPixel(x+pos_x, y+pos_y, BLACK);
            //   displayQRCode(x,y,0);
         
        } else {
              display.drawPixel(x+pos_x, y+pos_y, WHITE);
            //  displayQRCode(x,y,1);
      
        }

        display.display();
    }
  //  Serial.print("\n");
}

  }





void displayQRCode(int x,int y,uint8_t color)
{
   int scale=2;
   x=x+scale;
   y=y+scale;

    for (int16_t i=0; i<scale; i++) {
    display.drawLine(x, y+i, x+scale, y+i, color);
    display.display();
  } 

}
  
  
void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void sensor_aire(void){
  
  adc_MQ = analogRead(A0); //Lemos la salida analógica del MQ
  aire = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  Serial.println(aire);  
  
  }

 void sensor_ambiente(void){
 
  int err;

   if((err = dht11.read(hu, tem)) == 0)    // Si devuelve 0 es que ha leido bien
          {
             Serial.print("Temperature: ");
             Serial.print(tem);
             Serial.print(" /Humidity: ");
             Serial.print(hu);
             
          }
  
  
  
  } 

