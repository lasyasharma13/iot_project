#include <Wire.h>  // must be included here so that Arduino library object file references work
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define OLED_RESET D5
/* Object named display, of the class Adafruit_SSD1306 */
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* for normal hardware wire use above */
char datestring[20], datestring1[4];
uint8_t s = 0, m = 0, h = 0;

int buzz8amHH = 9;
int buzz8amMM = 58;
int buzz8amSS = 00;
int buzz2pmHH = 12;
int buzz2pmMM = 35;
int buzz2pmSS = 00;
int buzz8pmHH = 20;
int buzz8pmMM = 00;
int buzz8pmSS = 00 ;
RtcDateTime now = Rtc.GetDateTime();
void setup ()
{
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT);
    pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
 
  Serial.begin(115200);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  //--------RTC SETUP ------------
  // if you are using ESP-01 then uncomment the line below to reset the pins to
  // the available pins for SDA, SCL
  // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();


  if (!Rtc.IsDateTimeValid())
  {
    if (Rtc.LastError() != 0)
    {
      // we have a communications error
      // see https://www.arduino.cc/en/Reference/WireEndTransmission for
      // what the number means
      Serial.print("RTC communications error = ");
      Serial.println(Rtc.LastError());
    }
    else
    {
      // Common Cuases:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");
      // following line sets the RTC to the date & time this sketch was compiled
      // it will also reset the valid flag internally unless the Rtc device is
      // having an issue

      Rtc.SetDateTime(compiled);
    }
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }


  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
  display.clearDisplay();  /* Clear display */

}

void loop ()
{
  if (!Rtc.IsDateTimeValid())
  {
    if (Rtc.LastError() != 0)
    {
      // we have a communications error
      // see https://www.arduino.cc/en/Reference/WireEndTransmission for
      // what the number means
      Serial.print("RTC communications error = ");
      Serial.println(Rtc.LastError());
    }
    else
    {
      // Common Cuases:
      //    1) the battery on the device is low or even missing and the power line was disconnected
      Serial.println("RTC lost confidence in the DateTime!");
    }
  }

  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();
  display.setTextSize(1);  /* Select font size of text. Increases with size of argument. */
  display.setTextColor(WHITE);  /* Color of text*/
  display.clearDisplay();
  drawStr(25, 5, "Time for medicine");
  Serial.print(datestring);
  display.setTextSize(2);
  drawStr(0, 20, datestring);
  Serial.printf("time is");
  display.display();
  delay(5000); // ten seconds
  
// function to start buzzing at 2pm
  if (int(now.Hour()) == buzz2pmHH) {
    if (int(now.Minute()) == buzz2pmMM) {
  
        ///////////////////////////////////////////////////
        digitalWrite(D3, HIGH);
        digitalWrite(D6, HIGH);
        delay(1000);
        ////////////////////////////////////////////////
    }
          else
      {
        digitalWrite(D3, LOW);
        digitalWrite(D6, LOW);
      }
  }
        else
      {
        digitalWrite(D3, LOW);
      digitalWrite(D6, LOW);
      }
 // function to start buzzing at 8pm
  if (int(now.Hour()) == buzz8pmHH) {
    if (int(now.Minute()) == buzz8pmMM) {

              /////////////////////////////////////////////////////
        digitalWrite(D4, HIGH);
        digitalWrite(D6, HIGH);
        delay(1000);
        /////////////////////////////////////////////////////

      
    }
          else
      {
        digitalWrite(D4, LOW);
      digitalWrite(D6, LOW);
      }
  }
        else
      {
        digitalWrite(D4, LOW);
      
      digitalWrite(D6, LOW);
      }
       // function to start buzzing at 8am
  if (int(now.Hour()) == buzz8amHH) {
    if (int(now.Minute()) == buzz8amMM) {
        ////////////////////////////////////////////////////
        digitalWrite(D0, HIGH);
digitalWrite(D6, HIGH);
        delay(1000);
        /////////////////////////////////////////////////////

      }
          else
      {
        digitalWrite(D0, LOW);
        digitalWrite(D6, LOW);
      }
  }
        else
      {
        digitalWrite(D0, LOW);
        digitalWrite(D6, LOW);
      }      
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{


  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u   %02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  //display.display();

}

void drawStr(uint8_t x, uint8_t y, char* str) {
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}
