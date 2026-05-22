/*
 * motor_profiles.h
 *
 *  Created on: Sep 5, 2023
 *      Author: tk
 *      Revise：csx
 */

#ifndef INC_MOTOR_PROFILES_H_
#define INC_MOTOR_PROFILES_H_

#include "motor_controller_conf.h"

/** ======== Motor Profile Definitions ======== **/

#ifdef MOTORPROFILE_MAD_M6C12_150KV
#define MOTOR_POLE_PAIRS                      14
#define MOTOR_TORQUE_CONSTANT                 0.0636611977f
#define MOTOR_PHASE_RESISTANCE                0.090f
#define MOTOR_PHASE_INDUCTANCE                0.00003065f
#define MOTOR_CALIBRATION_CURRENT             3
#endif

#ifdef MOTORPROFILE_MAD_5010_110KV
#define MOTOR_POLE_PAIRS                      14
#define MOTOR_TORQUE_CONSTANT                 0.1176f
#define MOTOR_PHASE_RESISTANCE                0.6193f
#define MOTOR_PHASE_INDUCTANCE                0.0000850f
#define MOTOR_CALIBRATION_CURRENT             3
#endif

#ifdef MOTORPROFILE_TMOTOR_MN4004_300KV
#define MOTOR_POLE_PAIRS                      12
#define MOTOR_TORQUE_CONSTANT                 0.03183098861f // Kt = 60/(2π * KV), KV = 300
#define MOTOR_PHASE_RESISTANCE                0.452f         // Internal Resistance of specs
#define MOTOR_PHASE_INDUCTANCE                2.703e-04f     // it measured with LRC meter in 1kHz
#define MOTOR_CALIBRATION_CURRENT             3              // if it's not given, usually is 10-30% of peak current (here is 9A)
#endif

#endif /* INC_MOTOR_PROFILES_H_ */
