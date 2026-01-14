#ifndef EXITSCREENPRESENTER_HPP
#define EXITSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class exitScreenView;

class exitScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    exitScreenPresenter(exitScreenView& v);

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

    virtual ~exitScreenPresenter() {}

private:
    exitScreenPresenter();

    exitScreenView& view;
};

#endif // EXITSCREENPRESENTER_HPP
