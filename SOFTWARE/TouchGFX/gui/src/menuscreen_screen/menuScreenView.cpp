#include <gui/menuscreen_screen/menuScreenView.hpp>
#include <gui_generated/menuscreen_screen/menuScreenViewBase.hpp>

const char* menuScreenView::textArrayScrollList[9] = {
    "Mode Rodeo", "Mode Libre", "Mode jsp",
    "Mode Jsp", "Mode Jsp", "Mode Jsp",
    "Mode Jsp", "Mode Jsp", "Mode Jsp"
};

menuScreenView::menuScreenView()
    : fadeCallback(this, &menuScreenView::onFadeEnded)
{

}

void menuScreenView::setupScreen()
{
    menuScreenViewBase::setupScreen();

    for (int i = 0; i < MENUBCKGRNDSIZE; i++)
    {
        menuBckgrndList[i]->setFadeAnimationEndedAction(fadeCallback);
    }
}

void menuScreenView::tearDownScreen()
{
    menuScreenViewBase::tearDownScreen();
}

void menuScreenView::buttonMenuPlayisTriggered()
{
    if (alreadyTriggered) return;
    alreadyTriggered = true;

    playMenuButton.setTouchable(false);
    playMenuButton.setAlpha(128);
    playMenuButton.invalidate();

    startMyTask();
    waitingForAction = true;
}

void menuScreenView::handleTickEvent()
{
    menuScreenViewBase::handleTickEvent();

    if (waitingForAction && myBoolIsReady())
    {
        waitingForAction = false;
        application().gotoSimhubScreenScreenNoTransition();
    }

    if ((menuModeIndex = scrollWheel1.getSelectedItem()) != lastmenuModeIndex)
    {
        lastmenuModeIndex = menuModeIndex;
        changeMenuModeBckgrnd(menuModeIndex % 2);
    }
}

void menuScreenView::changeMenuModeBckgrnd(int menuModeIndex)
{
    for (int i = 0; i < MENUMODEBCKGRNDSIZE; i++)
    {
        menuModeBckgrndList[i]->setVisible(i == menuModeIndex);
        menuModeBckgrndList[i]->invalidate();
    }
}

void menuScreenView::startMyTask()
{
    for (int i = 0; i < 10000; i++) {}
}

bool menuScreenView::myBoolIsReady()
{
    return true;
}

void menuScreenView::animateBckgrndMenuSimHub(int index)
{
    if (swipeContainer1.getSelectedPage() != 0) return;

    for (int i = 0; i < MENUBCKGRNDSIZE; i++)
    {
        if (i == index)
        {
            menuBckgrndList[i]->setVisible(true);
            menuBckgrndList[i]->startFadeAnimation(255, 20, EasingEquations::linearEaseInOut);
        }
        else if (menuBckgrndList[i]->isVisible())
        {
            menuBckgrndList[i]->startFadeAnimation(0, 20, EasingEquations::linearEaseInOut);
        }
    }
}

void menuScreenView::onFadeEnded(const FadeAnimator<Image>& img)
{
    if (img.getAlpha() == 0)
    {
        const_cast<FadeAnimator<Image>&>(img).setVisible(false);
    }
}

void menuScreenView::scrollWheel1UpdateItem(CustomContainer1_1& item, int16_t itemIndex)
{
    item.setText(textArrayScrollList[itemIndex % 9]);
}

void menuScreenView::scrollWheel1UpdateCenterItem(CustomContainer1& item, int16_t itemIndex)
{
    item.setText(textArrayScrollList[itemIndex % 9]);
}
