#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <cstring>

#ifndef SIMULATOR

#include "main.h"
#include "mutexRTOSDOFs.h"
#include "stdlib.h"

#endif

Model::Model() : modelListener(0), localAxes{2047, 2047, 2047, 2047}
{

}

void Model::tick()
{
#ifndef SIMULATOR

	osMutexAcquire(g_axesMutex, osWaitForever);
	memcpy(localAxes, (const void*)g_axes, sizeof(localAxes));
	osMutexRelease(g_axesMutex);


	modelListener->updateDOFs(localAxes);

#endif
}
