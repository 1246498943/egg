
#include "Export.h"

#include <iostream>
#include <list>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Components/Camera.h>
#include <eggEngine/Components/Light.h>


using namespace fbxsdk;



/*
*   ���camera�����Ϣ
*/
egg::ComponentBase* exportCamera(FbxNode* pNode, FbxCamera* camera)
{
    egg::Camera* nb_cam = new egg::Camera();


    return nb_cam;
}


/*
*   ���light�����Ϣ
*/
egg::ComponentBase* exportLight(FbxNode* pNode, FbxLight* ligbt)
{

    egg::Light* nb_light = new egg::Light();


    return nb_light;
}
