#ifndef SIMHUBSCREENVIEW_HPP
#define SIMHUBSCREENVIEW_HPP

#include <gui_generated/simhubscreen_screen/SimhubScreenViewBase.hpp>
#include <gui/simhubscreen_screen/SimhubScreenPresenter.hpp>
#include <gui/swipeExitContainer/SwipeExitHandler.hpp>


class SimhubScreenView : public SimhubScreenViewBase
{
public:
    SimhubScreenView();
    virtual ~SimhubScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void setVlueTestDisplaySimHub(int* testBuffer);
    void updateDisplayValues(int* arrayOfValues, int count);

protected:
    virtual void handleDragEvent(const touchgfx::DragEvent& evt) override;
	virtual void handleClickEvent(const touchgfx::ClickEvent& evt) override;
private:
    void onExitSwipeTriggered();
    SwipeExitHandler swipeExitHandler;
    touchgfx::Callback<SimhubScreenView> onExitCallback;
    Unicode::UnicodeChar valueBuffers[8][10];
};

#endif // SIMHUBSCREENVIEW_HPP
