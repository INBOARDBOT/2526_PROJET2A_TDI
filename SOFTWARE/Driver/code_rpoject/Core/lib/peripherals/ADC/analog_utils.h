#ifndef ANALOG_UTILS_H
#define ANALOG_UTILS_H

#include <FOC/foc_config.h>

void update_current_sensor(
		ANG_HandleTypeDef *current_sensor,
		const ADC_HandleTypeDef *adc1,
		const ADC_HandleTypeDef *adc2);

void update_vbus_voltage(ANG_HandleTypeDef *current_sensor, ADC_HandleTypeDef *adc1);

void analog_init(
		ANG_HandleTypeDef *current_sensor,
		ADC_HandleTypeDef *adc1,
		ADC_HandleTypeDef *adc2);


#endif
