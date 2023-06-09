/*
 * coloralti_led_controller.h
 *
 *  Created on: Apr 30, 2023
 *      Author: kking
 */

#ifndef INC_ALTIMEX_LED_CONTROLLER_H_
#define INC_ALTIMEX_LED_CONTROLLER_H_
#include <altimex_state_controller.h>
#include "altimex_config.h"
#include "led_strip.h"

void Altimex_displayLeds(enum AltimexState state, uint16_t step, struct AltimexConfig* config, uint16_t alt);

#endif /* INC_ALTIMEX_LED_CONTROLLER_H_ */
