#include "CameraController.h"


//Initalize the Camera Structure
void CameraController_InitStruct(struct sCameraController *pCameraController, struct sDAC_Channel *pDAC)
{
    pCameraController->pDAC = pDAC;
}

//Trigger the Camera
//TODO This will need to Be Updated at some point in the Future
void CameraController_TriggerCamera(struct sCameraController *pCameraController)
{
    //Set the Ouput high for some amount of time then low
    return;
}
