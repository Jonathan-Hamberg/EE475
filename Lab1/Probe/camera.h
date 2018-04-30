#ifndef CAMERA_H
#define	CAMERA_H

#include <stdint.h>        /* For uint8_t definition */

typedef enum {
    CAMERA_SHUTDOWN = 0,
    CAMERA_STANDBY,
    CAMERA_FILMING,
    CAMERA_MALFUNCTION,
} CameraState;


/**
 * 
 * @param cameraNumber
 * @param state
 */
void privateSetCameraState(uint8_t cameraNumber, CameraState state);

/**
 * 
 * @param cameraNumber
 * @param state
 */
void setCameraState(uint8_t cameraNumber, CameraState state);

/**
 * 
 * @param cameraNumber
 * @return 1 true, 0 false
 */
uint8_t isOperational(uint8_t cameraNumber);

/**
 * 
 * @param cameraNumber
 * @return 
 */
uint8_t getCameraData(uint8_t cameraNumber);

/**
 * 
 * @param cameraNumber
 * @return 
 */
uint8_t startFilming(uint8_t cameraNumber);

/**
 * 
 * @param cameraNumber
 * @return 
 */
uint8_t cameraHasData(uint8_t cameraNumber);
/**
 * 
 * @param currentCamera
 * @return 
 */
int8_t findOtherCamera(uint8_t currentCamera);

/**
 * 
 */
void updateCameraLED();

#endif	/* CAMERA_H */

