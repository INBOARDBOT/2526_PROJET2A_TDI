#include <gui/containers/CustomContainer1.hpp>

CustomContainer1::CustomContainer1()
{

}

void CustomContainer1::initialize()
{
    CustomContainer1Base::initialize();
}


void CustomContainer1::setText(const char* string_text)
{
	Unicode::fromUTF8((const uint8_t*)string_text, textArea1Buffer, TEXTAREA1_SIZE);
	textArea1.invalidate();
}
