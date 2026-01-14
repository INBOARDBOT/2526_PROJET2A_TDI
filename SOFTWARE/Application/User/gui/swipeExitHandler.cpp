#include <gui/swipeExitContainer/SwipeExitHandler.hpp>


using namespace touchgfx;

SwipeExitHandler::SwipeExitHandler()
    : swipeContainer(nullptr),
      exitCallback(nullptr),
      dragStartX(0),
      dragDelta(0),
      exitThreshold(80),
      direction(LEFT_FROM_FIRST_PAGE)
{}

void SwipeExitHandler::attachTo(SwipeContainer* container)
{
    swipeContainer = container;
}

void SwipeExitHandler::setExitThreshold(int16_t px)
{
    exitThreshold = px;
}

void SwipeExitHandler::setOnExitCallback(GenericCallback<>* cb)
{
    exitCallback = cb;
}

void SwipeExitHandler::setDirection(Direction dir)
{
    direction = dir;
}

void SwipeExitHandler::handleDragEvent(const DragEvent& evt)
{
    if (!swipeContainer)
        return;

    // Let SwipeContainer handle its own normal drag
    swipeContainer->handleDragEvent(evt);

    // Only track overswipe on relevant edge
    if ((direction == LEFT_FROM_FIRST_PAGE && swipeContainer->getSelectedPage() != 0) ||
        (direction == RIGHT_FROM_LAST_PAGE && swipeContainer->getSelectedPage() != swipeContainer->getNumberOfPages() - 1))
    {
        dragStartX = 0;
        dragDelta = 0;
        return;
    }

    if (evt.getType() == DragEvent::DRAGGED)
    {
        if (dragStartX == 0)
            dragStartX = evt.getOldX();

        dragDelta = evt.getNewX() - dragStartX;
    }
}

void SwipeExitHandler::handleClickEvent(const ClickEvent& evt)
{
    if (!swipeContainer)
        return;

    swipeContainer->handleClickEvent(evt);

    if (evt.getType() == ClickEvent::RELEASED && exitCallback && exitCallback->isValid())
    {
        if ((direction == LEFT_FROM_FIRST_PAGE && swipeContainer->getSelectedPage() == 0 && dragDelta <= -exitThreshold) ||
            (direction == RIGHT_FROM_LAST_PAGE && swipeContainer->getSelectedPage() == swipeContainer->getNumberOfPages() - 1 && dragDelta >= exitThreshold))
        {
            exitCallback->execute();
        }
    }

    dragStartX = 0;
    dragDelta = 0;
}
