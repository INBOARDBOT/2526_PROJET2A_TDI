#include <peripherals/ADC/analog_utils.h>

void analog_init(
		ANG_HandleTypeDef *current_sensor,
		ADC_HandleTypeDef *adc1,
		ADC_HandleTypeDef *adc2){

	HAL_ADCEx_Calibration_Start(adc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(adc2, ADC_SINGLE_ENDED);

	HAL_ADCEx_InjectedStart_IT(adc1);
	HAL_ADCEx_InjectedStart_IT(adc2);
}


void update_current_sensor(
		ANG_HandleTypeDef *current_sensor,
		const ADC_HandleTypeDef *adc1,
		const ADC_HandleTypeDef *adc2
		){

	uint32_t raw_a_value = adc1->Instance->JDR1;
	uint32_t raw_c_value = adc2->Instance->JDR2;
	uint32_t raw_b_value = adc2->Instance->JDR1;

	float32_t amp_a_value = (float32_t)raw_a_value * VREF / (ADC_RESOLUTION * TOTAL_GAIN); // (vout_a - VMID) / (TOTAL_GAIN * SHUNT_RESISTOR);
	float32_t amp_c_value = (float32_t)raw_c_value * VREF / (ADC_RESOLUTION * TOTAL_GAIN); // (vout_c - VMID) / (TOTAL_GAIN * SHUNT_RESISTOR);
	float32_t amp_b_value = (float32_t)raw_b_value * VREF / (ADC_RESOLUTION * TOTAL_GAIN); // (vout_c - VMID) / (TOTAL_GAIN * SHUNT_RESISTOR);

	current_sensor->Ia = (float32_t)raw_a_value;
	current_sensor->Ic = (float32_t)raw_c_value;

	current_sensor->Ib = (float32_t)raw_b_value; // -(current_sensor->Ia + current_sensor->Ic);
}

void update_vbus_voltage(ANG_HandleTypeDef *current_sensor, ADC_HandleTypeDef *adc1)
{
	uint32_t raw_vbus = adc1->Instance->JDR2;
	current_sensor->vbus = (float32_t)raw_vbus * CONVERSION_VBUS;  // HAL_ADCEx_InjectedGetValue(adc1, ADC_INJECTED_RANK_2) * CONVERSION_VBUS;

}
