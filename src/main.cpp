#include <TinyPICO.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET     -1 // Reset pin = -1 as no reset pin on this screen
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String readBattery(){
  /* An improved battery estimation function 
    This software, the ideas and concepts is Copyright (c) David Bird 2019 and beyond.
    All rights to this software are reserved.
    It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
    1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
    2. You may copy the content to individual third parties for their personal use, but only if you acknowledge
        the author David Bird as the source of the material.
    3. You may not, except with my express written permission, distribute or commercially exploit the content.
    4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
    5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies
        or substantial portions of the software and where the software use is visible to an end-user.
    THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
    FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
    AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
  */
  uint8_t percentage = 100;
  float voltage = analogRead(15) / 4096.0 * 7.23;      // Works almost 100% accurately on ESP32 with 100K+100K voltage divider, might need tweaking depending on the health of the battery
  Serial.println("Voltage = " + String(voltage));
  percentage = 2808.3808 * pow(voltage, 4) - 43560.9157 * pow(voltage, 3) + 252848.5888 * pow(voltage, 2) - 650767.4615 * voltage + 626532.5703;
  if (voltage > 4.19) percentage = 100;
  else if (voltage <= 3.50) percentage = 0;
  return String(percentage)+"%";
}

// Display Battery Level
void displayBatteryLevel(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.println(readBattery());
}

// Display something else
void displayAnotherElement(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(44,0);

  display.println("Other Data");
}


void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Initial dot display
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
}


void loop() {
  display.clearDisplay();
  displayBatteryLevel();
  displayAnotherElement();
  display.display();
  
  delay(1000);
}
