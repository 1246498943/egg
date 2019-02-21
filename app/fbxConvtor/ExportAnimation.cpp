
#include "Export.h"

#include <iostream>
#include <fstream>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Animation.h>


using namespace fbxsdk;

//�ݹ���䶯����Ϣ
//    Ҫ�󶯻��ڵ㲻������
//    ֻ�����֡��Ϣ ��ָ����֡������ �����0��ʼ������
void getAnimation(FbxNode* pNode, FbxTime start_time, FbxTime end_time, egg::Animation* anim, egg::Animation* anim_global)
{
    //����
    std::string name = pNode->GetName();

    //�ж�node�Ķ�����Ϣ�Ƿ���Ч
    FbxAnimCurveNode* acn0 = pNode->LclTranslation.GetCurveNode();
    FbxAnimCurveNode* acn1 = pNode->LclRotation.GetCurveNode();
    FbxAnimCurveNode* acn2 = pNode->LclScaling.GetCurveNode();
    FbxAnimCurveNode* acn3 = pNode->PreRotation.GetCurveNode();
    if (acn0 || acn1 || acn2 || acn3)
    {
        //��ÿ�����ֶ�Ӧһ��float16������ ��ʾlocal����

        //��ʼλ�úͲ�������
        double s = start_time.GetSecondDouble();
        double e = end_time.GetSecondDouble();
        FbxLongLong sn = start_time.GetFrameCount();
        FbxLongLong en = end_time.GetFrameCount();
        double step = (end_time - start_time).GetSecondDouble() / (en - sn);

        //����һ���¶���ͨ��
        int size = anim->getIndex().size();
        anim->getIndex()[name] = size;
        anim->getData().resize(size + 1);
        anim->getData()[size].resize((en - sn + 1) * 16);

        size = anim_global->getIndex().size();
        anim_global->getIndex()[name] = size;
        anim_global->getData().resize(size + 1);
        anim_global->getData()[size].resize((en - sn + 1) * 16);


        //printf(" ");
        printf("anim %d %s ", size, name.c_str());
        for (FbxLongLong i = 0; i <= en - sn; i++)
        {
            printf(" %lld", (sn + i));

            //���㵱ǰ֡��ʱ��
            FbxTime keyTimer;
            keyTimer.SetSecondDouble(s + i*step);
            FbxString str = keyTimer.GetTimeString();

            //��������
            FbxAMatrix curveKeyLocalMatrix = pNode->EvaluateLocalTransform(keyTimer);
            FbxAMatrix curveKeyGlobalMatrix = pNode->EvaluateGlobalTransform(keyTimer);
            double* lm = curveKeyLocalMatrix;
            double* gm = curveKeyGlobalMatrix;

            //�����
            for (int m = 0; m < 16; m++)
            {
                anim->getData()[size][i * 16 + m] = lm[m];
                anim_global->getData()[size][i * 16 + m] = gm[m];
            }

            //printf("  a %s\n",name.c_str() );

        }

        printf("\n");
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        getAnimation(pNode->GetChild(j), start_time, end_time, anim, anim_global);
    }
}


void exportDefaultAnimationCode(egg::Animation* anim)
{
    std::ofstream ofs_pose("pose.inc");
    std::ofstream ofs_anim("anim.inc");

    ofs_pose << "#define pose_channal " << anim->getIndex().size() << "\n";
    ofs_pose << "#define pose_frame " << 1 << "\n";
    ofs_pose << "struct pose{std::string name;float data[pose_frame*16];};\n";
    ofs_pose << "pose _pose[pose_channal] = {\n";

    ofs_anim << "#define anim_channal " << anim->getIndex().size() << "\n";
    ofs_anim << "#define anim_frame " << anim->getFrameNum() << "\n";
    ofs_anim << "struct anim{std::string name;float data[anim_frame*16];};\n";
    ofs_anim << "anim _anim[anim_channal] = {\n";

    std::map< std::string, unsigned int > index = anim->getIndex();
    for (auto it = index.begin(); it != index.end(); it++)
    {
        if (it != index.begin())
        {
            ofs_pose << ",";
            ofs_anim << ",";
        }

        ofs_pose << "\"" << it->first.c_str() << "\" , {";
        ofs_anim << "\"" << it->first.c_str() << "\" , {";
        for (unsigned int i = 0; i < anim->getData()[it->second].size(); i++)
        {
            if (i < 16)
            {
                if (i != 0)
                {
                    ofs_pose << ",";
                }
                //ofs_pose.precision(1);
                ofs_pose << std::fixed << anim->getData()[it->second][i] << "f";
            }

            if (i != 0)
            {
                ofs_anim << ",";
            }
            ofs_anim << std::fixed << anim->getData()[it->second][i] << "f";
        }
        ofs_pose << "}\n";
        ofs_anim << "}\n";
    }


    ofs_pose << "};\n";
    ofs_pose.close();
    ofs_anim << "};\n";
    ofs_anim.close();
}

//�������� ����һ��Animation
//    _frame_begin ֡��ʼλ�� -1Ϊ��ǰ���������֡��
//    _frame_count ֡����     -1Ϊ�Զ�ʶ�����֡��
egg::Animation* LoadAnimation(FbxScene* scene, int _frame_begin, int _frame_count)
{
    //����anim����
    egg::ref_ptr<egg::Animation> anim = new egg::Animation();
    egg::ref_ptr<egg::Animation> anim_global = new egg::Animation();
    anim->setUniformPath("scene");
    anim_global->setUniformPath("scene");

    //��ѯĬ�϶�������
    FbxAnimStack* stack = scene->GetCurrentAnimationStack();
    FbxTimeSpan localtimespan = stack->GetLocalTimeSpan();
    FbxTimeSpan reftimespan = stack->GetReferenceTimeSpan();

    //Ĭ�ϳ���
    FbxTime start_time = reftimespan.GetStart();
    FbxTime end_time = reftimespan.GetStop();
    //FbxTime step_time  = reftimespan.GetSignedDuration();

    //������������޸Ŀ�ʼ��ֹλ��
    if (_frame_begin > 0)
    {
        //FbxString aa = start_time.GetTimeString();
        //start_time.SetTimeString();
    }
    if (_frame_count > 0)
    {
    }

    //��ȡ���ܴ��ڵĶ�����Ϣ
    getAnimation(scene->GetRootNode(), start_time, end_time, anim.get(), anim_global.get());
    if (anim->getIndex().size() == 0) return nullptr; //���������ֱ�ӷ���null

    //��������Ĵ���
    exportDefaultAnimationCode(anim_global.get());

    //������ȷ�Ķ��� �����ͷ��ڴ����
    return anim.release();
}

