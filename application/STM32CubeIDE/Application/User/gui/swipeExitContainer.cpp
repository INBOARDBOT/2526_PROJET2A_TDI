#include <gui/swipeExitContainer/swipeExitContainer.hpp>
#include <touchgfx/hal/Types.hpp> // for types

using namespace touchgfx;

SwipeContainerWithExit::SwipeContainerWithExit()
    : dragStartX(0),
      maxLeftDelta(0),
      exitThreshold(80), // default ~80px
      exitCallback(nullptr)
{
}

void SwipeContainerWithExit::handleDragEvent(const DragEvent& evt)
{
    // Normal SwipeContainer behaviour
    SwipeContainer::handleDragEvent(evt);

    // Only detect overswipe when on first page
    if (getSelectedPage() != 0)
    {
        dragStartX = 0;
        maxLeftDelta = 0;
        return;
    }

    if (evt.getType() == DragEvent::DRAGGED)
    {
        if (dragStartX == 0)
        {
            dragStartX = evt.getOldX();
            maxLeftDelta = 0;
        }

        int16_t delta = evt.getNewX() - dragStartX;
        if (delta < maxLeftDelta)
            maxLeftDelta = delta;
    }
//    else if (evt.getType() == DragEvent::CANCELLED)
//    {
//        dragStartX = 0;
//        maxLeftDelta = 0;
//    }
}

void SwipeContainerWithExit::handleClickEvent(const ClickEvent& evt)
{
    // Let SwipeContainer handle normal page taps first
    SwipeContainer::handleClickEvent(evt);

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Check overswipe gesture
        if (getSelectedPage() == 0 && maxLeftDelta <= -exitThreshold)
        {
            if (exitCallback && exitCallback->isValid())
                exitCallback->execute();
        }

        dragStartX = 0;
        maxLeftDelta = 0;
    }
}
