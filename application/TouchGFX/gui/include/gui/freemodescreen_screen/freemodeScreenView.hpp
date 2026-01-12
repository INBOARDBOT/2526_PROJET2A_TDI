#ifndef FREEMODESCREENVIEW_HPP
#define FREEMODESCREENVIEW_HPP

#include <gui_generated/freemodescreen_screen/freemodeScreenViewBase.hpp>
#include <gui/freemodescreen_screen/freemodeScreenPresenter.hpp>

class freemodeScreenView : public freemodeScreenViewBase
{
public:
    freemodeScreenView();
    virtual ~freemodeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // FREEMODESCREENVIEW_HPP
