#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/SwipeContainer.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/Callback.hpp>

#ifndef SWIPEEXITCONTAINER_HPP
#define SWIPEEXITCONTAINER_HPP

#include <touchgfx/containers/SwipeContainer.hpp>
#include <touchgfx/Callback.hpp>

class SwipeContainerWithExit : public touchgfx::SwipeContainer
{
public:
    SwipeContainerWithExit();

    void setExitThreshold(int16_t px) { exitThreshold = px; }
    void setOnExitSwipeCallback(touchgfx::GenericCallback<>* cb) { exitCallback = cb; }

    virtual void handleDragEvent(const touchgfx::DragEvent& evt) override;
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt) override;

private:
    int16_t dragStartX;
    int16_t maxLeftDelta;
    int16_t exitThreshold;
    touchgfx::GenericCallback<>* exitCallback;
};

#endif // SWIPECONTAINERWITHEXIT_HPP

