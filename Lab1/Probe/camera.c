#include "camera.h"
#include "mcc_generated_files/mcc.h"
CameraState camera_states[3] = {CAMERA_SHUTDOWN,
    CAMERA_SHUTDOWN,
    CAMERA_SHUTDOWN};

void setCameraState(uint8_t cameraNumber, CameraState state) {
    if (cameraNumber <= 2) {
        camera_states[cameraNumber] = state;
    }
}

uint8_t isOperational(uint8_t cameraNumber) {
    if (cameraNumber <= 2) {
        return camera_states[cameraNumber] != CAMERA_MALFUNCTION;
    } else {
        return 0;
    }
}

uint8_t getCameraData(uint8_t cameraNumber, uint8_t* data) {
    if (cameraNumber > 2) {
        return 0;
    }

    if (camera_states[cameraNumber] != CAMERA_FILMING) {
        return 0;
    }

    *data = cameraNumber;

    return 1;
}

void updateCameraLED() {

    for (int8_t i = 2; i >= 0; i--) {

        if (i != 2) {
            // Set the shutdown LED.
            if (camera_states[i] == CAMERA_FILMING) {
                SHIFT_SER_SetHigh();
            } else {
                SHIFT_SER_SetLow();
            }

            // Shift the data into the register.
            SHIFT_SRCLK_LED_SetHigh();
            SHIFT_SRCLK_LED_SetLow();
        }

        // Set the standby LED.
        if (camera_states[i] == CAMERA_STANDBY) {
            SHIFT_SER_SetHigh();
        } else {
            SHIFT_SER_SetLow();
        }

        // Shift bit into the register.
        SHIFT_SRCLK_LED_SetHigh();
        SHIFT_SRCLK_LED_SetLow();

        // Set the filming LED.
        if (camera_states[i] == CAMERA_SHUTDOWN) {
            SHIFT_SER_SetHigh();
        } else {
            SHIFT_SER_SetLow();
        }

        // Shift bit into the register.r
        SHIFT_SRCLK_LED_SetHigh();
        SHIFT_SRCLK_LED_SetLow();
    }

    // Update the data.
    SHIFT_RCLK_SetHigh();
    SHIFT_RCLK_SetLow();

    // Update the last bit that didn't fit in the shift register.
    if (camera_states[2] == CAMERA_FILMING) {
        LED_9_SetHigh();
    } else {
        LED_9_SetLow();
    }
}