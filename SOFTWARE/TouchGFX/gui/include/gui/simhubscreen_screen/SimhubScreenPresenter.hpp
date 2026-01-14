#ifndef SIMHUBSCREENPRESENTER_HPP
#define SIMHUBSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SimhubScreenView;

class SimhubScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SimhubScreenPresenter(SimhubScreenView& v);

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

    virtual ~SimhubScreenPresenter() {}

    virtual void setVlueTestDisplaySimHub(int* testBuffer);

private:
    SimhubScreenPresenter();

    SimhubScreenView& view;
};

#endif // SIMHUBSCREENPRESENTER_HPP
