#include <LilyGoWatch.h>

TTGOClass *ttgo = nullptr;

void setup() {
    Serial.begin(115200);

    // 1. Get the singleton watch instance and initialize hardware
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    
    // 2. Turn on the screen backlight
    ttgo->openBL();

    // 3. Clear screen to black
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor(20, 40);
    ttgo->tft->print("T-Watch 2020 V3");
}

void loop() {
    // Fetch time from the RTC chip
    RTC_Date datetime = ttgo->rtc->getDateTime();

    char timeBuffer[16];
    sprintf(timeBuffer, "%02d:%02d:%02d", datetime.hour, datetime.minute, datetime.second);

    // Print live time to the screen
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextSize(3);
    ttgo->tft->setCursor(35, 100);
    ttgo->tft->print(timeBuffer);

    delay(1000);
}
