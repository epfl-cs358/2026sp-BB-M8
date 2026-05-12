#pragma once

class AsyncWebServer;  // forward declaration — avoids pulling ESPAsyncWebServer into Camera.cpp

bool camera_init();
void camera_stream_begin();
void camera_register_photo_handler(AsyncWebServer& server);
