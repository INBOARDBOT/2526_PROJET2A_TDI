#ifndef SWIPEEXITHANDLER_HPP
#define SWIPEEXITHANDLER_HPP

#include <touchgfx/containers/SwipeContainer.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/Callback.hpp>

class SwipeExitHandler
{
public:
    enum Direction
    {
        LEFT_FROM_FIRST_PAGE,
        RIGHT_FROM_LAST_PAGE
    };

    SwipeExitHandler();

    void attachTo(touchgfx::SwipeContainer* container);
    void setExitThreshold(int16_t px);
    void setOnExitCallback(touchgfx::GenericCallback<>* cb);
    void setDirection(Direction dir);

    void handleDragEvent(const touchgfx::DragEvent& evt);
    void handleClickEvent(const touchgfx::ClickEvent& evt);

private:
    touchgfx::SwipeContainer* swipeContainer;
    touchgfx::GenericCallback<>* exitCallback;
    int16_t dragStartX;
    int16_t dragDelta;
    int16_t exitThreshold;
    Direction direction;
};

#endif // SWIPEEXITHANDLER_HPP
