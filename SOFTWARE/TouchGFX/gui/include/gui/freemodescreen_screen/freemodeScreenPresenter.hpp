#ifndef FREEMODESCREENPRESENTER_HPP
#define FREEMODESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class freemodeScreenView;

class freemodeScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    freemodeScreenPresenter(freemodeScreenView& v);

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

    virtual ~freemodeScreenPresenter() {}

private:
    freemodeScreenPresenter();

    freemodeScreenView& view;
};

#endif // FREEMODESCREENPRESENTER_HPP
