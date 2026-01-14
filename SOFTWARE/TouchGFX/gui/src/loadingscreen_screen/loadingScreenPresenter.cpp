#include <gui/loadingscreen_screen/loadingScreenView.hpp>
#include <gui/loadingscreen_screen/loadingScreenPresenter.hpp>

loadingScreenPresenter::loadingScreenPresenter(loadingScreenView& v)
    : view(v)
{

}

void loadingScreenPresenter::activate()
{

}

void loadingScreenPresenter::deactivate()
{

}


void loadingScreenPresenter::rotatingCornCat(float angle)
{
	view.updateCornCatRotation(angle);
}

void loadingScreenPresenter::setReadToInit(bool state)
{
	view.updateStateofInitButton(state);
}


void loadingScreenPresenter::animateTextLoading(int state)
{
	view.updateAnimationTextLoading(state);
}
