#include "Camera.hpp"
#include <ESPAsyncWebServer.h>
#include "esp_camera.h"

// Isolated from Camera.cpp to avoid HTTP_GET ambiguity between
// esp_http_server.h and ESPAsyncWebServer.h.

void camera_register_photo_handler(AsyncWebServer& server) {
    server.on("/photo", HTTP_GET, [](AsyncWebServerRequest* req) {
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) { req->send(503, "text/plain", "Capture failed"); return; }
        AsyncResponseStream* res = req->beginResponseStream("image/jpeg", fb->len);
        res->write(fb->buf, fb->len);
        esp_camera_fb_return(fb);
        req->send(res);
    });
}
