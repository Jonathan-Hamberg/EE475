#include "camera.h"

CameraState camera_states[3] = {CAMERA_SHUTDOWN,
    CAMERA_SHUTDOWN,
    CAMERA_SHUTDOWN};

void setCameraState(uint8_t cameraNumber, CameraState state) {
    if(cameraNumber <= 2) {
        camera_states[cameraNumber] = state;
    }
}

uint8_t isOperational(uint8_t cameraNumber) {
    if(cameraNumber <= 2) {
        return camera_states[cameraNumber] != CAMERA_MALFUNCTION;
    } else { 
        return 0;
    }
}

uint8_t getCameraData(uint8_t cameraNumber, uint8_t* data) {
    if(cameraNumber > 2) {
        return 0;
    }
    
    if(camera_states[cameraNumber] != CAMERA_FILMING) {
        return 0;
    }
    
    *data = cameraNumber;
    
    return 1;
}