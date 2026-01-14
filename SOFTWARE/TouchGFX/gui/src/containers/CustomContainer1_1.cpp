#include <gui/containers/CustomContainer1_1.hpp>

CustomContainer1_1::CustomContainer1_1()
{

}

void CustomContainer1_1::initialize()
{
    CustomContainer1_1Base::initialize();
}

void CustomContainer1_1::setText(const char* string_text)
{
	Unicode::fromUTF8((const uint8_t*)string_text, textArea1Buffer, TEXTAREA1_SIZE);
	textArea1.invalidate();
}

