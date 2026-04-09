#include <gui/simhubscreen_screen/SimhubScreenView.hpp>

SimhubScreenView::SimhubScreenView()
	: onExitCallback(this, &SimhubScreenView::onExitSwipeTriggered)
{

}

void SimhubScreenView::setupScreen()
{
    SimhubScreenViewBase::setupScreen();

    swipeExitHandler.attachTo(&swipeContainer1);
    swipeExitHandler.setDirection(SwipeExitHandler::RIGHT_FROM_LAST_PAGE);
    swipeExitHandler.setExitThreshold(150);
    swipeExitHandler.setOnExitCallback(&onExitCallback);

}

void SimhubScreenView::tearDownScreen()
{
    SimhubScreenViewBase::tearDownScreen();
}


void SimhubScreenView::handleDragEvent(const DragEvent& evt)
{
    swipeExitHandler.handleDragEvent(evt);
}

void SimhubScreenView::handleClickEvent(const ClickEvent& evt)
{
    swipeExitHandler.handleClickEvent(evt);
}

void SimhubScreenView::onExitSwipeTriggered()
{
    application().gotomenuScreenScreenNoTransition();
}

void SimhubScreenView::setVlueTestDisplaySimHub(int* testBuffer)
{

	// day
	Unicode::snprintf(textCurrentProgressM1Buffer, TEXTCURRENTPROGRESSM1_SIZE, "%d", (int)testBuffer[0]);
	textCurrentProgressM1.invalidate();

	Unicode::snprintf(textCurrentProgressM2Buffer, TEXTCURRENTPROGRESSM2_SIZE, "%d", (int)testBuffer[1]);
	textCurrentProgressM2.invalidate();

	// month
	Unicode::snprintf(textCurrentProgressM3Buffer, TEXTCURRENTPROGRESSM3_SIZE, "%d", (int)testBuffer[2]);
	textCurrentProgressM3.invalidate();

	Unicode::snprintf(textCurrentProgressM4Buffer, TEXTCURRENTPROGRESSM4_SIZE, "%d", (int)testBuffer[3]);
	textCurrentProgressM4.invalidate();

	//year
	Unicode::snprintf(textAngleProgressM1Buffer, TEXTANGLEPROGRESSM1_SIZE, "%d", (int)testBuffer[4]);
	textAngleProgressM1.invalidate();

	Unicode::snprintf(textAngleProgressM2Buffer, TEXTANGLEPROGRESSM2_SIZE, "%d", (int)testBuffer[5]);
	textAngleProgressM2.invalidate();

	Unicode::snprintf(textAngleProgressM3Buffer, TEXTANGLEPROGRESSM3_SIZE, "%d", (int)testBuffer[6]);
	textAngleProgressM3.invalidate();

	Unicode::snprintf(textAngleProgressM4Buffer, TEXTANGLEPROGRESSM4_SIZE, "%d", (int)testBuffer[7]);
	textAngleProgressM4.invalidate();
}
