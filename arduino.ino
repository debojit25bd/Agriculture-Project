#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD ডিসপ্লে সেটআপ (অ্যাড্রেস 0x27, ১৬ কলাম, ২ সারি)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// পিন ডিফাইন
const int moisturePin = A0; // সেন্সরের AO পিন আর্ডুইনোর A0-তে
const int buzzerPin = 6; // বাজার আর্ডুইনোর D6 পিনে

// সেন্সরের শুকনো ও ভেজা অবস্থার মান (ক্যালিব্রেশন)
const int AirValue = 800; // সম্পূর্ণ শুকনো বাতাসে অ্যানালগ মান
const int WaterValue = 350; // পানিতে বা খুব ভেজা মাটিতে অ্যানালগ মান

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // শুরুতে বাজার বন্ধ থাকবে

  // LCD ডিসপ্লে চালু করা
  lcd.init();          
  lcd.backlight();     // ব্যাকলাইট অন করা
}

void loop() {
  // ১. সেন্সর থেকে অ্যানালগ ডেটা (Raw Data) পড়া
  int rawValue = analogRead(moisturePin);

  // ২. অ্যানালগ মানকে ০% থেকে ১০০% এ রূপান্তর করা
  int moisturePercent = map(rawValue, AirValue, WaterValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // ৩. LCD ডিসপ্লে ডিজাইন (১ম লাইন)
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(moisturePercent);
  lcd.print("% ");
  
  lcd.print("Raw:");
  lcd.print(rawValue);
  lcd.print("   "); // আগের কোনো বড় সংখ্যা থাকলে তা মুছে ফেলার জন্য অতিরিক্ত স্পেস

  // ৪. LCD ডিসপ্লে ডিজাইন (২য় লাইন - স্ট্যাটাস)
  lcd.setCursor(0, 1);

  if (moisturePercent < 25) {
    lcd.print("Status: DRY!    ");
    digitalWrite(buzzerPin, HIGH); // বাজার চালু
    delay(200);
    digitalWrite(buzzerPin, LOW); // বিপ বিপ শব্দের জন্য
  } else if (moisturePercent >= 25 && moisturePercent < 50) {
    lcd.print("Status: DRY     ");
    digitalWrite(buzzerPin, LOW); // বাজার বন্ধ
  } else if (moisturePercent >= 50 && moisturePercent < 75) {
    lcd.print("Status: PERFECT ");
    digitalWrite(buzzerPin, LOW); // বাজার বন্ধ
  } else {
    lcd.print("Status: TOO WET ");
    digitalWrite(buzzerPin, LOW); // বাজার বন্ধ
  }

  delay(500); // প্রতি আধা সেকেন্ড পর পর ডিসপ্লে আপডেট হবে
}
