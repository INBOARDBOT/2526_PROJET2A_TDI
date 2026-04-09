#ifndef LOADINGSCREENVIEW_HPP
#define LOADINGSCREENVIEW_HPP

#include <gui_generated/loadingscreen_screen/loadingScreenViewBase.hpp>
#include <gui/loadingscreen_screen/loadingScreenPresenter.hpp>

class loadingScreenView : public loadingScreenViewBase
{
public:
    loadingScreenView();
    virtual ~loadingScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void updateCornCatRotation(float angle);
    void updateStateofInitButton(bool state);
    void updateAnimationTextLoading(int state);
protected:
};

#endif // LOADINGSCREENVIEW_HPP
