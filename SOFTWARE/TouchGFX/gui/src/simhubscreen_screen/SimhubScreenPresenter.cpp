#include <gui/simhubscreen_screen/SimhubScreenView.hpp>
#include <gui/simhubscreen_screen/SimhubScreenPresenter.hpp>

SimhubScreenPresenter::SimhubScreenPresenter(SimhubScreenView& v)
    : view(v)
{

}

void SimhubScreenPresenter::activate()
{

}

void SimhubScreenPresenter::deactivate()
{

}


void SimhubScreenPresenter::setVlueTestDisplaySimHub(int* testBuffer)
{
	view.setVlueTestDisplaySimHub(testBuffer);
}
