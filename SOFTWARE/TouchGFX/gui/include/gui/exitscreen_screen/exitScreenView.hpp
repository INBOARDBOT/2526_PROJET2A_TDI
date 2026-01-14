#ifndef EXITSCREENVIEW_HPP
#define EXITSCREENVIEW_HPP

#include <gui_generated/exitscreen_screen/exitScreenViewBase.hpp>
#include <gui/exitscreen_screen/exitScreenPresenter.hpp>

class exitScreenView : public exitScreenViewBase
{
public:
    exitScreenView();
    virtual ~exitScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EXITSCREENVIEW_HPP
