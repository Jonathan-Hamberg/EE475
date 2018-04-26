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
 * @param data
 * @return 1 error, 0 success
 */
uint8_t getCameraData(uint8_t cameraNumber, uint8_t* data);

#endif	/* CAMERA_H */

