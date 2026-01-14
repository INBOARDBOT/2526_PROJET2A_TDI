#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Application.hpp>

#include <cstring>

#ifndef SIMULATOR

#include "main.h"
#include "mutexRTOSDOFs.h"
#include "stdlib.h"
int testBuffer[8];

#endif



Model::Model() : modelListener(0)
{
#ifndef SIMULATOR

	counter        = 0;
	tickCounter    = 0;
	stateofInit    = true;
	angleofCornCat = 0.0f;
	bckgrndCounter = 0;
	bufferImgIndex = 0;


#endif
}

void Model::tick()
{
#ifndef SIMULATOR

	updateAngle();
    updateLoadingCounter();
    notifyPresenter();
    testDisplayinSimhub();

#endif

}


void Model::updateAngle()
{
	angleofCornCat -= 0.125f;
    if (angleofCornCat > 360.0f)
    	angleofCornCat -= 360.0f;
}

void Model::updateLoadingCounter()
{
    tickCounter++;
    if (tickCounter >= 20)
    {
        tickCounter = 0;
        counter = (counter + 1) % 3;
    }
}

void Model::notifyPresenter()
{
    if (!modelListener) return;

    modelListener->rotatingCornCat(angleofCornCat);
    modelListener->setReadToInit(stateofInit);
    modelListener->animateTextLoading(counter);
}

void Model::testDisplayinSimhub()
{

	// Make sure mutex is initialized
	if (g_axesMutex == NULL)
	{
		InitAxesMutex();
	}

	if (osMutexAcquire(g_axesMutex, 10) == osOK)
	{
		for(int i = 0; i < 8; i++)
		{
			// FIX: Use g_axes[i] instead of g_axes
			testBuffer[i] = (int)g_axes[i];
		}
		osMutexRelease(g_axesMutex);

		// Only notify if data was successfully acquired
		if (modelListener)
		{
			modelListener->setVlueTestDisplaySimHub(testBuffer);
		}
	}
	else
	{
		// Handle mutex acquisition failure
		// You might want to set error values in testBuffer or retry
	}
}




