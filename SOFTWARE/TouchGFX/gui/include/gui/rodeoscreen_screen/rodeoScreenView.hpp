#ifndef RODEOSCREENVIEW_HPP
#define RODEOSCREENVIEW_HPP

#include <gui_generated/rodeoscreen_screen/rodeoScreenViewBase.hpp>
#include <gui/rodeoscreen_screen/rodeoScreenPresenter.hpp>

class rodeoScreenView : public rodeoScreenViewBase
{
public:
    rodeoScreenView();
    virtual ~rodeoScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // RODEOSCREENVIEW_HPP
