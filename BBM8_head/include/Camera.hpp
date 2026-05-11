#pragma once

bool camera_init();       // init OV2640; returns false on failure
void camera_stream_begin(); // start MJPEG HTTP server on port 82
