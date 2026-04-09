#ifndef ANALOG_CONFIG_H
#define ANALOG_CONFIG_H

#include "arm_math.h"


#define ADC_RESOLUTION    4095.0f
#define VREF              3.3f
#define SHUNT_RESISTOR    0.007f
#define VMID              1.65f

#define INTERNAL_PGA_GAIN 1.0f
#define EXTERNAL_GAIN     10.0f
#define TOTAL_GAIN        (INTERNAL_PGA_GAIN * EXTERNAL_GAIN)

#define CONVERSION_VBUS   0.01928887712f



typedef struct {
	uint32_t sampling_frequency;

	float32_t Ia;
	float32_t Ib;
	float32_t Ic;

	__IO float32_t vbus;

}ANG_HandleTypeDef;


#endif
