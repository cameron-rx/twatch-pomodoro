#include <config.h>

TTGOClass *ttgo = nullptr;
bool screenActive = true;
bool irq = false;

void setup() {
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

    // Create background colour style for default screen
    static lv_style_t style_screen;
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&style_screen, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_add_style(lv_scr_act(), LV_OBJ_PART_MAIN, &style_screen);


    // White text style
    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_color(&style_text, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Create text to draw
    lv_obj_t *text = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(text, LV_OBJ_PART_MAIN, &style_text);
    lv_label_set_text(text, "Hello World!");
    lv_obj_align(text, NULL, LV_ALIGN_CENTER, 0, 0);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);

    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    ttgo->power->clearIRQ();
}

void loop() {
    lv_task_handler();

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {
            ttgo->power->clearIRQ();

            screenActive = !screenActive;

            if (screenActive) {
                ttgo->openBL();
                Serial.println("Backlight on");
            } else {
                ttgo->closeBL();
                Serial.println("Backlight off");
            }
        }
    }
}
