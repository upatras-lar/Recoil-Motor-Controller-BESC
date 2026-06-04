/*
 * encoder.h
 *
 *  Created on: Aug 24, 2022
 *      Author: TK
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "stm32g4xx_hal.h"
#include "foc_math.h"
#include "motor_controller_conf.h"

/**
 * @brief Encoder object.
 */
typedef struct {
  TIM_HandleTypeDef *htim;          // pointer that calculates the pulse count of AEDT-9810-Z00

  int32_t   cpr;                    // counts per revolution
  float     position_offset;        // in range (-inf, inf)

  float     velocity_filter_alpha;

  uint16_t  position_raw;           // in range [0, cpr-1] -- __HAL_TIM_GET_COUNTER
  int32_t   n_rotations;

  float     position;               // in range (-inf, inf), with offset
  float     velocity;

  float     flux_offset;            // for FOC
  float     flux_offset_table[128]; // for FOC
} Encoder;


/**
 * @brief Get the position offset of the encoder.
 *
 * @param encoder Pointer to the Encoder struct.
 * @return The offset value in radians (rad).
 */
static inline float Encoder_getPositionOffset(Encoder *encoder) {
  return encoder->position_offset;
}

/**
 * @brief Set the position offset of the encoder.
 *
 * @param encoder Pointer to the Encoder struct.
 * @param encoder The offset value in radians (rad).
 */
static inline void Encoder_setPositionOffset(Encoder *encoder, float offset) {
  encoder->position_offset = offset;
}

/**
 * @brief Get the measured position of the encoder.
 *
 * This method returns the actual position value of the encoder without offset.
 *
 * @param encoder Pointer to the Encoder struct.
 * @return The current measured position in radians (rad).
 */
static inline float Encoder_getPositionMeasured(Encoder *encoder) {
  return encoder->position;
}

/**
 * @brief Get the position of the encoder.
 *
 * This method returns the position value of the encoder compensated with offset.
 *
 * @param encoder Pointer to the Encoder struct.
 * @return The current position in radians (rad).
 */
static inline float Encoder_getPosition(Encoder *encoder) {
  return encoder->position + encoder->position_offset;
}

/**
 * @brief Get the velocity of the encoder.
 *
 * @param encoder Pointer to the Encoder struct.
 * @return The current velocity in radians per second (rad/s).
 */
static inline float Encoder_getVelocity(Encoder *encoder) {
  return encoder->velocity;
}

/**
 * @brief Initialize the Encoder instance with default values.
 *
 * This function initializes an Encoder instance by setting various parameters and performing initializations
 * required for its operation. It configures the I2C interface and initializes other variables and settings.
 *
 * It initiates a position memory read request on the I2C. The subsequent updates can therefore stream the
 * measured position with only I2C read frames.
 *
 * @param encoder Pointer to the Encoder struct.
 * @param htim Pointer to the TIM_HandleTypeDef structure that configures the Timer interface.
 * @return Status of the initialization process. HAL_OK if successful.
 */
HAL_StatusTypeDef Encoder_init(Encoder *encoder, TIM_HandleTypeDef *htim);

/**
 * @brief Reset the flux offset and rotation count of the Encoder instance.
 *
 * This function resets the rotation count and flux offset of the provided Encoder object.
 * It sets the rotation count to 0 and clears the flux offset and the flux offset table.
 *
 * @param encoder Pointer to the Encoder struct.
 */
void Encoder_resetFluxOffset(Encoder *encoder);

/**
 * @brief Update encoder readings.
*
 * This function reads the current hardware counter from the Timer, handles overflows/underflows
 * for multiple rotations, and updates the position and velocity.
 *
 * @param encoder Pointer to the Encoder struct.
 * @return Status of the update process. HAL_OK if successful.
 */
HAL_StatusTypeDef Encoder_update(Encoder *encoder);

#endif /* INC_ENCODER_H_ */
