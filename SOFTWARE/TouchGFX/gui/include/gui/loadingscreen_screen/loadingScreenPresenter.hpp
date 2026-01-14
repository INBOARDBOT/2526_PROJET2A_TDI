#ifndef LOADINGSCREENPRESENTER_HPP
#define LOADINGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class loadingScreenView;

class loadingScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    loadingScreenPresenter(loadingScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();
    virtual void rotatingCornCat(float angle) override;
    virtual void setReadToInit(bool state);
    virtual void animateTextLoading(int state);
    virtual ~loadingScreenPresenter() {}

private:
    loadingScreenPresenter();

    loadingScreenView& view;
};

#endif // LOADINGSCREENPRESENTER_HPP
