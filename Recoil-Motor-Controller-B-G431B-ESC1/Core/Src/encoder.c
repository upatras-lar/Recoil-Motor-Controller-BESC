/*
 * encoder.c
 *
 *  Created on: Aug 24, 2022
 *      Author: TK
 */

#include "encoder.h"


HAL_StatusTypeDef Encoder_init(Encoder *encoder, TIM_HandleTypeDef *htim) {
  encoder->htim = htim;

  encoder->cpr = ENCODER_DIRECTION * ENCODER_CPR;

  encoder->position_offset = 0.f;

  // defaults to be 2000 Hz cutoff, out of 10 kHz loop
  encoder->velocity_filter_alpha = 0.7153904566639707f;

  encoder->position_raw = 0;
  encoder->n_rotations = 0;

  encoder->position = 0.f;
  encoder->velocity = 0.f;

  Encoder_resetFluxOffset(encoder);

  HAL_StatusTypeDef status = HAL_TIM_Encoder_Start(encoder->htim, TIM_CHANNEL_ALL);
  encoder->position_raw = __HAL_TIM_GET_COUNTER(encoder->htim);

  return status;
}

void Encoder_resetFluxOffset(Encoder *encoder) {
  encoder->n_rotations = 0;
  encoder->flux_offset = 0.f;
  memset((uint8_t *)encoder->flux_offset_table, 0, ENCODER_LUT_ENTRIES*sizeof(float));
}

HAL_StatusTypeDef Encoder_update(Encoder *encoder) {
  // Read the raw reading from the Timer
  uint16_t raw_reading = __HAL_TIM_GET_COUNTER(encoder->htim);

  // Calculate the change in reading
  int16_t reading_delta = (int16_t)raw_reading - (int16_t)encoder->position_raw;

  // Handle multi-rotation crossing (ARR = 19999, cpr = 20000)
  if (reading_delta < -10000) {
      encoder->n_rotations += 1;
  }
  else if (reading_delta > 10000) {
      encoder->n_rotations -= 1;
  }
  encoder->position_raw = raw_reading;

  // Convert the raw position to position in radians (rad)
  float position = (((float)raw_reading / (float)encoder->cpr) + (float)encoder->n_rotations) * (M_2PI_F);

  // Update the delta position
  float delta_position = position - encoder->position;
  encoder->position = position;

  // Update the filtered velocity
  float velocity = delta_position * (float)COMMUTATION_FREQ;
  encoder->velocity += encoder->velocity_filter_alpha * (velocity - encoder->velocity);

  return HAL_OK;
}
