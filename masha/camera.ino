// #include "passwords.h"
// #include "camera.h"

// #define BOT_TICK 5000

// FastBot2 bot;
// ulong last_bot_tick_millis = 0;

// void updateh(fb::Update& u) {
//   Serial.println("New message!");

//   if (u.message().text() == "/start") {
//     // пропустить прошлый кадр
//     camera_fb_t* fbj = esp_camera_fb_get();
//     esp_camera_fb_return(fbj);

//     fbj = esp_camera_fb_get();
//     if (fbj) {
//         fb::File f("frame.jpg", fb::File::Type::photo, fbj->buf, fbj->len);
//         f.chatID = u.message().chat().id();
//         bot.sendFile(f, true);  // обязательно синхронная отправка
//     }
//     esp_camera_fb_return(fbj);
//   }
// }

// void camera_init() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASS);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("Connected");
//   Serial.println(WiFi.localIP());

//   bool res = cam_init(FRAMESIZE_VGA, PIXFORMAT_JPEG);
//   Serial.println(res ? "Camera OK" : "Camera ERROR");

//   bot.attachUpdate(updateh);
//   bot.setToken(F(BOT_TOKEN));
//   bot.setPollMode(fb::Poll::Long, 20000);
// }

// void camera_tick() {
//   if (millis() - last_bot_tick_millis >= BOT_TICK) {
//     last_bot_tick_millis = millis();
//   }
//   bot.tick();
// }