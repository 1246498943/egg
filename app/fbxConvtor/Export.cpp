
#include "Export.h"


egg::ComponentBase* exportAttribute(FbxNode* pNode, FbxNodeAttribute* pAttribute
    , egg::SkeletonActorControler* sac, std::map<std::string, std::string>& copy_image_list, bool world_position)
{
    if (!pAttribute) return nullptr;

    //FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    fbxsdk::FbxNodeAttribute::EType type = pAttribute->GetAttributeType();
    FbxString attrName = pAttribute->GetName();

    switch (type)
    {
    case fbxsdk::FbxNodeAttribute::eUnknown:
        std::cout << "  eUnknown : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eNull:
        std::cout << "  eNull : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eMarker:
        std::cout << "  eMarker : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eSkeleton:
        std::cout << "  eSkeleton : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eMesh:
        std::cout << "  eMesh : " << attrName.Buffer() << std::endl;
        return exportMesh(pNode, (FbxMesh*)pAttribute, sac, copy_image_list, world_position);
        break;
    case fbxsdk::FbxNodeAttribute::eNurbs:
        std::cout << "  eNurbs : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::ePatch:
        std::cout << "  ePatch : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eCamera:
        std::cout << "  eCamera : " << attrName.Buffer() << std::endl;
        return exportCamera(pNode, (FbxCamera*)pAttribute);
        break;
    case fbxsdk::FbxNodeAttribute::eCameraStereo:
        std::cout << "  eCameraStereo : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
        std::cout << "  eCameraSwitcher : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eLight:
        std::cout << "  eLight : " << attrName.Buffer() << std::endl;
        return exportLight(pNode, (FbxLight*)pAttribute);
        break;
    case fbxsdk::FbxNodeAttribute::eOpticalReference:
        std::cout << "  eOpticalReference : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eOpticalMarker:
        std::cout << "  eOpticalMarker : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eNurbsCurve:
        std::cout << "  eNurbsCurve : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
        std::cout << "  eTrimNurbsSurface : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eBoundary:
        std::cout << "  eBoundary : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eNurbsSurface:
        std::cout << "  eNurbsSurface : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eShape:
        std::cout << "  eShape : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eLODGroup:
        std::cout << "  eLODGroup : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eSubDiv:
        std::cout << "  eSubDiv : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eCachedEffect:
        std::cout << "  eCachedEffect : " << attrName.Buffer() << std::endl;
        break;
    case fbxsdk::FbxNodeAttribute::eLine:
        std::cout << "  eLine : " << attrName.Buffer() << std::endl;
        return exportLine(pNode, (FbxLine*)pAttribute);
        break;
    default:
        break;
    }

    return nullptr;
}


//���������FBX�ڵ� ������Ӧ��DOM SceneNode
egg::SceneNode* exportNode(FbxNode* pNode, egg::SkeletonActorControler* sac
    , std::map<std::string, std::string>& copy_image_list
    , bool collapse_mesh
    , bool skinning_mode)
{
    getCustomNodeProperty(pNode);

    const char* nodeName = pNode->GetName();

    egg::SceneNode* sn = new egg::SceneNode();
    sn->setName(nodeName);
    std::cout << "Node : " << nodeName << std::endl;

    //��ȡ��ǰlocal
    vmml::mat4f mat = getLocalMatrix(pNode);

    //FbxTime keyTimer;
    //keyTimer.SetSecondDouble( 0.0 );
    //FbxAMatrix curveKeyLocalMatrix  = pNode->EvaluateLocalTransform(/*keyTimer*/);
    ////FbxAMatrix curveKeyGlobalMatrix = pNode->EvaluateGlobalTransform(/*keyTimer*/);
    //double* m = curveKeyLocalMatrix;
    //vmml::mat4d mat;
    //mat.set(&m[0],&m[15],false);

    //�������ǰmesh�Ƿ���Ҫִ������
    bool local_collapse_mesh = false;

    //������Ҫ���� (��node��������ص�����)
    //   transform joint
    if (pNode->GetSkeleton())
    {
        //����joint��Ϊ���
        egg::Joint3D* joint = new egg::Joint3D();
        sn->addComponent(joint);
        joint->setLocalMatrix(mat);
    }
    else
    {
        //����transform��Ϊ���
        egg::Transfrom3D* transfrom = new egg::Transfrom3D();
        sn->addComponent(transfrom);

        //�Ƿ�����mesh�����¼������
        //  ��ǰ�����skinningģʽ
        //      �����mesh����skin           ����ѡ������collapse_meshֱ��Ϊfalse
        //      ���mesh����skin������       ����ѡ������collapse_meshֱ��Ϊture
        //  ��ǰ�������skinningģʽ
        //      �����mesh����skin           ��ѡ����collapse_mesh
        //      ���mesh����skin������       ����ѡ������collapse_meshֱ��Ϊtrue
        if (skinning_mode)
        {
            if (pNode->GetMesh() && hasSkin(pNode->GetMesh()))
            {
                //ʲôҲ��������ԭ��
                local_collapse_mesh = true;
            }
            else
            {
                transfrom->setLocalMatrix(mat);
            }
        }
        else
        {
            if ((pNode->GetMesh() && !hasSkin(pNode->GetMesh()) && collapse_mesh)
                || (pNode->GetMesh() && hasSkin(pNode->GetMesh()))
                )
            {
                //ʲôҲ��������ԭ��
                local_collapse_mesh = true;
            }
            else
            {
                transfrom->setLocalMatrix(mat);
            }
        }
    }


    //������������
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
    {
        egg::ComponentBase* component = exportAttribute(pNode, pNode->GetNodeAttributeByIndex(i), sac, copy_image_list, local_collapse_mesh);
        if (component)
        {
            sn->addComponent(component);
        }
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        egg::SceneNode* child = exportNode(pNode->GetChild(j), sac, copy_image_list, collapse_mesh, skinning_mode);
        sn->addChild(child);
    }

    return sn;
}

FbxScene* importFBXScene(FbxManager* lSdkManager, const char* lFilename)
{
    // Change the following filename to a suitable filename value.
    //const char* lFilename = "file.fbx";

    // Initialize the SDK manager. This object handles all our memory management.
    //FbxManager* lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
    {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        //exit(-1); 
        return nullptr;
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    lImporter->Destroy();

    //ǿ�����ǻ�
    FbxGeometryConverter lConverter(lSdkManager);
    lConverter.Triangulate(lScene, true);

    return lScene;
}

