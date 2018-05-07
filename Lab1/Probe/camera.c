#include "camera.h"
#include "mcc_generated_files/mcc.h"

volatile uint8_t cameraState = 0;

void setCameraState(uint8_t cameraNumber, CameraState state) {
    // Cannot set camera state that does not exist.
    if (cameraNumber > 2) {
        return;
    }


    // User cannot set the malfunction state.
    if (state == CAMERA_MALFUNCTION) {
        return;
    }

    // Cannot change the state of the camera is malfunctioning.
    if (getCameraState(cameraNumber) != CAMERA_MALFUNCTION) {
        privateSetCameraState(cameraNumber, state);
    }
}

void privateSetCameraState(uint8_t cameraNumber, CameraState state) {
    // Make sure state is valid.
    if (state != CAMERA_SHUTDOWN &&
            state != CAMERA_STANDBY &&
            state != CAMERA_FILMING &&
            state != CAMERA_MALFUNCTION) {
        return;
    }

    // If the camera number is valid, set the internal camera state.
    if (cameraNumber <= 2) {
        uint8_t mask = 0x03u << (2u * cameraNumber);
        uint8_t data = (uint8_t)((uint8_t)(state) << (uint8_t)(2u * cameraNumber));
        cameraState = (uint8_t)((cameraState & (uint8_t)(~mask)) | data);
    }
}

CameraState getCameraState(uint8_t cameraNumber) {
    if(cameraNumber > 2) {
        return CAMERA_MALFUNCTION;
    }
    
    uint8_t data = ((uint8_t)(cameraState >> (2u * cameraNumber)) & 0x03u);
    
    return (CameraState)(data);
}

uint8_t isOperational(uint8_t cameraNumber) {
    return getCameraState(cameraNumber) != CAMERA_MALFUNCTION;
}

uint8_t cameraHasData(uint8_t cameraNumber) {
    
    return getCameraState(cameraNumber) == CAMERA_FILMING;
}

uint8_t getCameraData(uint8_t cameraNumber) {
    // Cannot collect data if the camera is not filming.
    if(getCameraState(cameraNumber) != CAMERA_FILMING) {
        return 0xFF;
    }

    // Camera data is just the number of the camera.
    return cameraNumber;
}

uint8_t startFilming(uint8_t cameraNumber) {
    // Cannot set camera state that does not exist.
    if (cameraNumber > 2) {
        return 0;
    }

    // Cannot change the state of the camera is malfunctioning.
    if(getCameraState(cameraNumber) != CAMERA_MALFUNCTION) {
        privateSetCameraState(cameraNumber, CAMERA_FILMING);
        return 1;
    }

    return 0;
}

int8_t findOtherCamera(uint8_t currentCamera) {
    for (uint8_t i = 0; i < 3; i++) {
        if (i != currentCamera && 
                getCameraState(i) != CAMERA_MALFUNCTION) {
            return i;
        }
    }

    return -1;
}

uint8_t getCameraStateByte() {
    return cameraState;
}