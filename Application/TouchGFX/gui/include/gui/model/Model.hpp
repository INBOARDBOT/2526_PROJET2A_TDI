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

    virtual void updateDOFs(uint16_t* locAxes) {};
//    virtual void updateLEDs(bool* testLED) {};

    void tick();
protected:
    ModelListener* modelListener;
    uint16_t localAxes[4];
//    bool testLEDs[3];
};

#endif // MODEL_HPP
