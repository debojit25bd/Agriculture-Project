#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED ডিসপ্লে সাইজ সেটআপ (128x64 pixels)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// পিন ডিফাইন
const int moisturePin = A0; // সেন্সরের AO পিন আর্ডুইনোর A0-তে
const int buzzerPin = D6;    // বাজার আর্ডুইনোর D6 পিনে

// সেন্সরের শুকনো ও ভেজা অবস্থার মান (প্রয়োজনে টিউন করে নিতে পারেন)
const int AirValue = 800;   // সম্পূর্ণ শুকনো বাতাসে মান
const int WaterValue = 350; // পানিতে বা খুব ভেজা মাটিতে মান

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // শুরুতে বাজার বন্ধ থাকবে

  // OLED ডিসপ্লে চালু করা
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;); // ডিসপ্লে না পেলে আটকে থাকবে
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // সেন্সর থেকে অ্যানালগ রিডিং নেওয়া
  int sensorValue = analogRead(moisturePin);

  // অ্যানালগ রিডিংকে ০% থেকে ১০০% এ রূপান্তর করা
  int moisturePercent = map(sensorValue, AirValue, WaterValue, 0, 100);
  
  // মান যেন ০% থেকে ১০০% এর বাইরে না যায়
  moisturePercent = constrain(moisturePercent, 0, 100);

  // OLED ডিসপ্লেতে ডাটা দেখানো
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.print("SOIL MONITORING");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print("Moist: ");
  display.print(moisturePercent);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(10, 48);

  // ২৫% এর কম হলে বাজার বাজবে এবং সতর্কবার্তা দেখাবে
  if (moisturePercent < 25) {
    display.print("Status: VERY DRY!");
    digitalWrite(buzzerPin, HIGH); // বাজার চালু
    delay(200);
    digitalWrite(buzzerPin, LOW);  // বিপ বিপ শব্দের জন্য
  } else if (moisturePercent >= 25 && moisturePercent < 50) {
    display.print("Status: DRY");
    digitalWrite(buzzerPin, LOW);  // বাজার বন্ধ
  } else if (moisturePercent >= 50 && moisturePercent < 75) {
    display.print("Status: PERFECT");
    digitalWrite(buzzerPin, LOW);  // বাজার বন্ধ
  } else {
    display.print("Status: TOO WET");
    digitalWrite(buzzerPin, LOW);  // বাজার বন্ধ
  }

  display.display();
  delay(500); // প্রতি আধা সেকেন্ড পর পর আপডেট হবে
}
