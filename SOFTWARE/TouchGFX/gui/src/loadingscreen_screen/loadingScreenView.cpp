#include <gui/loadingscreen_screen/loadingScreenView.hpp>

loadingScreenView::loadingScreenView()
{

}

void loadingScreenView::setupScreen()
{
    loadingScreenViewBase::setupScreen();
}

void loadingScreenView::tearDownScreen()
{
    loadingScreenViewBase::tearDownScreen();
}

void loadingScreenView::updateCornCatRotation(float angle)
{
	float currentAngle = angle;
	imagemapCorncat.setAngles(0.0f, currentAngle, 0.0f);
	imagemapCorncat.invalidate();
}


void loadingScreenView::updateStateofInitButton(bool state)
{
	InitButton.setVisible(state);
	InitButton.setTouchable(state);
	textInitSystem.setVisible(state);

	textInitSystem.invalidate();
	InitButton.invalidate();
}

void loadingScreenView::updateAnimationTextLoading(int state)
{
	bool Text1 = (state == 0) ? true : false;
	bool Text2 = (state == 1) ? true : false;
	bool Text3 = (state == 2) ? true : false;

	textL3.setVisible(Text3);
	textL2.setVisible(Text2);
	textL1.setVisible(Text1);

	textL3.invalidate();
	textL2.invalidate();
	textL1.invalidate();
}
