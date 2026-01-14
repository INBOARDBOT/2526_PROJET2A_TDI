#ifndef MENUSCREENVIEW_HPP
#define MENUSCREENVIEW_HPP

#include <gui_generated/menuscreen_screen/menuScreenViewBase.hpp>
#include <gui/menuscreen_screen/menuScreenPresenter.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

#define MENUBCKGRNDSIZE 8
#define MENUMODEBCKGRNDSIZE 2

class menuScreenView : public menuScreenViewBase
{
public:
    /**
     * @brief Constructs the menu screen view.
     *
     * Initializes member variables and any required setup
     * for the screen before use.
     */
    menuScreenView();

    /**
     * @brief Virtual destructor.
     *
     * Cleans up any resources used by the screen.
     */
    virtual ~menuScreenView() {}

    /**
     * @brief Called when the screen is initialized.
     *
     * This method sets up the screen before it is displayed,
     * by initializing UI elements, animations, and background images.
     */
    virtual void setupScreen() override;

    /**
     * @brief Called when the screen is about to be removed.
     *
     * This method performs cleanup before navigating away from the screen,
     * used to stop animations or free resources to avoid memory leaks.
     */
    virtual void tearDownScreen() override;

    /**
     * @brief Periodically called by the framework.
     *
     * Called on every tick event, used to update UI components
     * or run periodic checks in the view.
     *
     * Update dynamic UI components such as :
     * 	- Dynamic change of simhub's backgrounds
     * 	- Dynamic change of mode's backgrounds
     *
     */
    virtual void handleTickEvent() override;

    /**
     * @brief Called when the "Play" menu button is triggered.
     *
     * Overriden method from menuScreenViewBase, triggered when the button event occurs
     *
     */
    virtual void buttonMenuPlayisTriggered() override;

    /**
     * @brief Starts a custom background task.
     *
     * This runs a certain task asynchronously or in sequence
     * and checks readiness state through myBoolIsReady().
     */
    void startMyTask();

    /**
     * @brief Checks if a task or process is ready.
     *
     * @return true if the task is completed.
     * @return false if the task is still in progress.
     */
    bool myBoolIsReady();

    /**
     * @brief Updates non-centered items in the scroll wheel.
     *
     * @param item The item container to be updated.
     * @param itemIndex The index of the item in the list.
     */
    virtual void scrollWheel1UpdateItem(CustomContainer1_1& item, int16_t itemIndex) override;

    /**
     * @brief Updates the centered item in the scroll wheel.
     *
     * @param item The centered item container to be updated.
     * @param itemIndex The index of the centered item in the list.
     */
    virtual void scrollWheel1UpdateCenterItem(CustomContainer1& item, int16_t itemIndex) override;

protected:
    /**
     * @brief Text contents for the scroll list items.
     */
    static const char* textArrayScrollList[9];

    /**
     * @brief Called when fade animation for an image has ended.
     *
     * @param img The reference to the Image whose fade animation ended.
     */
    virtual void onFadeEnded(const FadeAnimator<Image>& img);

    /**
     * @brief Callback for fade animations.
     */
    touchgfx::Callback<menuScreenView, const touchgfx::FadeAnimator<touchgfx::Image>&> fadeCallback;

private:
    bool waitingForAction = false;       ///< Flag indicating wait state
    bool alreadyTriggered = false;       ///< Ensures action triggers only once
    int menuModeIndex = 0;               ///< Current menu mode index
    int lastmenuModeIndex = 0;           ///< Previous menu mode index

    /**
     * @brief Initial setup for the scroll-like containers.
     */
    void setupScrollContainer();

    /**
     * @brief Animates the background through a sequence.
     *
     * @param index The index of the background image to animate.
     */
    void animateBckgrndMenuSimHub(int index);

    /**
     * @brief Changes the background based on current menu mode.
     *
     * @param menuModeIndex Index determining which background image to display.
     */
    void changeMenuModeBckgrnd(int menuModeIndex);

    /**
     * @brief List of background images used for menu modes.
     */
    touchgfx::Image* menuModeBckgrndList[MENUMODEBCKGRNDSIZE] = {
        &image2, &imageRodeo
    };

    /**
     * @brief List of background images animated in SimHub mode.
     */
    FadeAnimator<Image>* menuBckgrndList[MENUBCKGRNDSIZE] = {
        &imageSH1, &imageSH2, &imageSH3, &imageSH4,
        &imageSH5, &imageSH6, &imageSH7, &imageSH8
    };
};

#endif // MENUSCREENVIEW_HPP
