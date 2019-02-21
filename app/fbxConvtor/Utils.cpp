
#include "Utils.h"


std::vector<char> readfile(std::string filename)
{
    std::vector<char> ret;
    std::ifstream fl(filename, std::ios::binary);
    if (fl.good())
    {
        fl.seekg(0, std::ios::end);
        size_t len = fl.tellg();
        ret.resize(len);
        fl.seekg(0, std::ios::beg);
        fl.read(&ret[0], len);
        fl.close();
    }
    else
    {
        printf("read file failed : %s\n", filename.c_str());
    }
    return ret;
}


//�ϲ���ǰnode��local����
vmml::mat4f getLocalMatrix(FbxNode* node)
{
    vmml::mat4f mat;
    vmml::identity(mat);

#if 0
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();

    FbxDouble3 ro = node->RotationOffset.Get();
    FbxDouble3 rp = node->RotationPivot.Get();

    FbxDouble3 so = node->ScalingOffset.Get();
    FbxDouble3 sp = node->ScalingPivot.Get();

    FbxDouble3 pr = node->PreRotation.Get();   //��maya����Ա�ʾjoint orient
    FbxDouble3 or = node->PostRotation.Get();

    //pivot
    FbxDouble3 gt = node->GeometricTranslation.Get();
    FbxDouble3 gr = node->GeometricRotation.Get();
    FbxDouble3 gs = node->GeometricScaling.Get();


    //���㵱ǰ���� �����˳��ܹ���

    //λ��
    mat *= makeTrans(t[0], t[1], t[2]);

    //������ת��
    if (node->GetSkeleton())
    {
        mat *= makeRotate(pr[2] / 180.0*M_PI, 0, 0, 1);
        mat *= makeRotate(pr[1] / 180.0*M_PI, 0, 1, 0);
        mat *= makeRotate(pr[0] / 180.0*M_PI, 1, 0, 0);
    }

    //��ת
    mat *= makeRotate(r[2] / 180.0*M_PI, 0, 0, 1);
    mat *= makeRotate(r[1] / 180.0*M_PI, 0, 1, 0);
    mat *= makeRotate(r[0] / 180.0*M_PI, 1, 0, 0);

    //����
    mat *= makeScale((float)s[0], (float)s[1], (float)s[2]);


    //pivot SRT
    mat *= makeTrans(gt[0], gt[1], gt[2]);
    mat *= makeRotate(gr[0] / 180.0*M_PI, 1, 0, 0);
    mat *= makeRotate(gr[1] / 180.0*M_PI, 0, 1, 0);
    mat *= makeRotate(gr[2] / 180.0*M_PI, 0, 0, 1);
    mat *= makeScale((float)gs[0], (float)gs[1], (float)gs[2]);
#endif

    //ʹ��FBX������ʽ ����������
    FbxMatrix m = node->EvaluateLocalTransform(0);
    double* dm = m;
    float fm[16];
    for (int i = 0; i < 16; i++)
    {
        fm[i] = dm[i];
    }
    mat.set(&fm[0], &fm[15], false);

    //
    return mat;
}

vmml::mat4f getWorldMatrix(FbxNode* node)
{
    vmml::mat4f mat;
    vmml::identity(mat);

    //ʹ��FBX������ʽ ����������
    FbxMatrix m = node->EvaluateGlobalTransform(0);
    double* dm = m;
    float fm[16];
    for (int i = 0; i < 16; i++)
    {
        fm[i] = dm[i];
    }
    mat.set(&fm[0], &fm[15], false);

    //
    return mat;
}



bool hasSkin(FbxMesh* pMesh)
{
    int deformerCount = pMesh->GetDeformerCount();
    FbxDeformer* pFBXDeformer;
    FbxSkin*     pFBXSkin;

    for (int i = 0; i < deformerCount; ++i)
    {
        //��ȡ������
        pFBXDeformer = pMesh->GetDeformer(i);
        if (pFBXDeformer == NULL) { continue; }

        //��ȡskin����
        if (pFBXDeformer->GetDeformerType() != FbxDeformer::eSkin) { continue; }

        //��ȡskin����
        pFBXSkin = (FbxSkin*)(pFBXDeformer);
        if (pFBXSkin == NULL) { continue; }

        return true;
    }

    return false;
}

void getCustomNodeProperty(FbxNode* pNode)
{
    FbxProperty p;

    p = pNode->FindProperty("UDP3DSMAX", false);
    if (p.IsValid())
    {
        std::string nodeName = p.GetName();
        std::cout << "found property: " << nodeName << std::endl;
    }
}


//������������joint��bindposeλ��
void getPose(FbxScene* fbx_scene)
{
    unsigned int num_pose = fbx_scene->GetPoseCount();
    for (unsigned int i = 0; i < num_pose; i++)
    {
        FbxPose* pose = fbx_scene->GetPose(i);
        //pose->SetIsBindPose(true);
        if (!pose->IsBindPose()) continue;

        unsigned int num_node = pose->GetCount();
        for (unsigned int j = 0; j < num_node; j++)
        {
            FbxNode* node = pose->GetNode(j);

            //����õ��Ķ���bind���������
            FbxMatrix m = pose->GetMatrix(j);

            printf("");
        }
    }
}




