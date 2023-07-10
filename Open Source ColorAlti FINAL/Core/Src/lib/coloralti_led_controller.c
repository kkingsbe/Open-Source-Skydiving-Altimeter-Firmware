/*
 * coloralti_led_controller.c
 *
 *  Created on: Apr 30, 2023
 *      Author: kking
 */

#include "coloralti_led_controller.h"

uint32_t standbyLastFlash = 0;
uint32_t lastLedTurnonTime = 0;

void ColorAlti_displayLeds(enum ColorAltiState state, uint16_t step, struct ColorAltiConfig* config, uint16_t alt)
{
	//Periodically flashes ot show that the device is on
	if(state == COLORALTI_STANDBY)
	{
		uint32_t standbyFlashLength = config->standbyFlashOnLength + config->standbyFlashOffLength;
		Set_Brightness(config->standbyBrightness);

		//If board has just been turned on
		if(standbyLastFlash == 0 && HAL_GetTick() < 3000)
		{
			uint8_t on = (step) % 2 == 0;
			uint8_t leds = on * config->numLeds;
			Strip_Progress_Bar_Single_Color(leds, PURPLE);
		}
		else {
			//If the current time is greater than the last flash time + the flash length, we are onto a new flash now.
			if(HAL_GetTick() > standbyLastFlash + standbyFlashLength) {
				standbyLastFlash = HAL_GetTick();
			}
			else
			{
				//If the current time is greater than the last flash time + the flash, we are in the on portion of the flash.
				if(standbyLastFlash + config->standbyFlashOffLength < HAL_GetTick())
				{
					Strip_Progress_Bar_Single_Color(config->numLeds, GREEN);
				}
				else
				{
					Strip_Clear();
				}
			}
		}
	}

	//Led bar with top led flashing. Full when at gear check altitude
	if(state == COLORALTI_ASCENT)
	{
		Set_Brightness(config->brightness);
		uint8_t flash = (step / 5) % 2 == 0;
		uint8_t i = (config->numLeds * (alt / (double)config->gearCheckAlt)) + 1;
		if(flash) i++;
		Strip_Progress_Bar_Single_Color(i, GREEN);
	}

	//Flashes green for a few secs
	if(state == COLORALTI_GEARCHECK)
	{
		Set_Brightness(config->brightness);
		uint8_t flash = (step / 7) % 2 == 0;
		Strip_Progress_Bar_Single_Color(flash * config->numLeds, GREEN);
	}

	//Solid blue while awaiting jump
	if(state == COLORALTI_DETECT_FREEFALL_START)
	{
		Set_Brightness(config->brightness);
		Strip_Progress_Bar_Single_Color(config->numLeds, SKYBLUE);
	}

	//LED bar fills up approaching breakoff altitude + 1500ft
	if(state == COLORALTI_FREEFALL)
	{
		Set_Brightness(config->brightness);
		uint8_t leds = (config->numLeds) * (1-(float)(alt - (config->breakoff + 1500)) / (config->exit - (config->breakoff + 1500)));
		Strip_Progress_Bar_Single_Color(leds + 1, SKYBLUE);
	}

	//LED bar fills up approaching breakoff altitude
	if(state == COLORALTI_APPROACHING_BREAKOFF)
	{
		uint8_t leds = (config->numLeds + 1) * (1-(float)(alt - config->breakoff) / ((config->breakoff + 1500) - config->breakoff));
		Strip_Progress_Bar_Single_Color(leds, GREEN);
	}

	//Flashes green for a few secs
	if(state == COLORALTI_BREAKOFF)
	{
		Set_Brightness(config->brightness);
		uint8_t on = (step) % 2 == 0;
		uint8_t leds = on * config->numLeds;
		Strip_Progress_Bar_Single_Color(leds, GREEN);
	}

	//LED bar fills up approaching deployment altitude
	if(state == COLORALTI_TRACK)
	{
		Set_Brightness(config->brightness);
		uint8_t leds = (config->numLeds + 1) * (1-(float)(alt - config->deploy) / ((config->deploy + 500) - config->deploy));
		Strip_Progress_Bar_Single_Color(leds, RED);
	}

	//Flashes red until detects canopy deployment
	if(state == COLORALTI_DEPLOY)
	{
		Set_Brightness(config->brightness);
		uint8_t on = (step) % 2 == 0;
		uint8_t leds = on * config->numLeds;
		Strip_Progress_Bar_Single_Color(leds, RED);
	}

	//Flashes purple
	if(state == COLORALTI_CANOPY)
	{
		Set_Brightness(config->brightness);
		uint8_t on = (step/10) % 2 == 0;
		uint8_t leds = on * config->numLeds;
		Strip_Progress_Bar_Single_Color(leds, PURPLE);
	}

	Strip_Send();
}
