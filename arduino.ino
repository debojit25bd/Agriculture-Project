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

// সেন্সরের শুকনো ও ভেজা অবস্থার মান (ক্যালিব্রেশন)
const int AirValue = 800;   // সম্পূর্ণ শুকনো বাতাসে অ্যানালগ মান
const int WaterValue = 350; // পানিতে বা খুব ভেজা মাটিতে অ্যানালগ মান

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
  // ১. সেন্সর থেকে অ্যানালগ ডেটা (Raw Data) পড়া
  int rawValue = analogRead(moisturePin);

  // ২. অ্যানালগ মানকে ০% থেকে ১০০% এ রূপান্তর করা
  int moisturePercent = map(rawValue, AirValue, WaterValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // ৩. আধুনিক OLED ডিসপ্লে ডিজাইন
  display.clearDisplay();
  
  // শিরোনাম (Header)
  display.setTextSize(1);
  display.setCursor(18, 0);
  display.print("SOIL MONITORING");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // ওপরের পার্টিশন লাইন

  // --- বাম পাশ: পার্সেন্টেজ (%) ---
  display.setTextSize(1);
  display.setCursor(5, 16);
  display.print("MOISTURE");
  
  display.setTextSize(2);
  display.setCursor(5, 28);
  display.print(moisturePercent);
  display.print("%");

  // --- মাঝখানের খাড়া বাউন্ডারি লাইন ---
  display.drawFastVLine(64, 12, 38, SSD1306_WHITE);

  // --- ডান পাশ: অ্যানালগ/ডিজিটাল র ডেটা (Raw Value) ---
  display.setTextSize(1);
  display.setCursor(72, 16);
  display.print("RAW DATA");

  display.setTextSize(2);
  display.setCursor(72, 28);
  display.print(rawValue);

  // নিচের পার্টিশন লাইন
  display.drawLine(0, 50, 128, 50, SSD1306_WHITE);

  // --- নিচের অংশ: স্ট্যাটাস ও বাজার কন্ডিশন ---
  display.setTextSize(1);
  display.setCursor(5, 54);

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
  delay(500); // প্রতি আধা সেকেন্ড পর পর ডিসপ্লে আপডেট হবে
}

