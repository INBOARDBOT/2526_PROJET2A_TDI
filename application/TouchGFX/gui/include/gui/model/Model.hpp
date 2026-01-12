#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void goToExitScreen();

#ifndef SIMULATOR
    void updateAngle();
    void updateLoadingCounter();
    void notifyPresenter();
    void testDisplayinSimhub();
#endif

protected:
    ModelListener* modelListener;

#ifndef SIMULATOR

    int counter;
    int tickCounter;
    bool stateofInit;
    float angleofCornCat;
    int bckgrndCounter;
    int bufferImgIndex;

#endif
};

#endif // MODEL_HPP
