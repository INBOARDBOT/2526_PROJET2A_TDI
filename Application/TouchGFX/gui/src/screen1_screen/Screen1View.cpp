#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateDOFs(uint16_t* locAxes)
{

	Unicode::snprintf(textArea1_3Buffer, TEXTAREA1_3_SIZE, "%d", locAxes[3]);
	textArea1_3.invalidate();

	Unicode::snprintf(textArea1_2Buffer, TEXTAREA1_2_SIZE, "%d", locAxes[2]);
	textArea1_2.invalidate();

	Unicode::snprintf(textArea1_1Buffer, TEXTAREA1_1_SIZE, "%d", locAxes[1]);
	textArea1_1.invalidate();

	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", locAxes[0]);
	textArea1.invalidate();
}


