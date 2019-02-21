
#include "Export.h"

#include <iostream>
#include <map>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Components/SkeletonActorControler.h>


using namespace fbxsdk;



//���������ҵ��Ƿ����skin�ı�����
//    ˳���ռ�skeleton��ӳ���ϵ��Ϣ
//    ���û�� ˵������������SkeletonActorControler������
void getSkeleonMapping(FbxNode* pNode, bool& has_skin_deformer
    , std::map< std::string, egg::JointMapping >& sm
    , int& this_id, int parent_id = -1)
{
    //������ڵ�С��0 ˵����ǰid��û�п�ʼ����
    //if( parent_id < 0 ) this_id = -1;

    //��ѯjoint����
    if (pNode->GetSkeleton())
    {
        //����������
        this_id++;

        printf(" -- joint : %s parent_id %d this_id %d \n", pNode->GetName(), parent_id, this_id);

        egg::JointMapping jm;
        jm.name = pNode->GetName();
        jm.parent_id = parent_id;
        jm.this_id = this_id;

        //�������
        auto it = sm.find(jm.name);
        if (it != sm.end())
        {
            printf("error: joint has same name : %s \n", jm.name.c_str());
        }

        //������ʱ��֧���������� ���������ֱ�Ӿ͸����� �Ӷ����´���
        sm[jm.name] = jm;

        //���ӽڵ������ȷ��id
        parent_id = this_id;
    }

    if (pNode->GetMesh())
    {
        int deformerCount = pNode->GetMesh()->GetDeformerCount();
        if (deformerCount > 0)  has_skin_deformer = true;
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        getSkeleonMapping(pNode->GetChild(j), has_skin_deformer, sm, this_id, parent_id);
    }
}


//���䲻���ڵĹ���bindpose
void buildBindPose(FbxNode* pNode
    , std::map< FbxNode*, FbxMatrix >& bind
    , int& this_id, int parent_id = -1)
{
    //��ѯjoint����
    if (pNode->GetSkeleton())
    {
        //����������
        this_id++;
        printf(" -- joint : %s parent_id %d this_id %d \n", pNode->GetName(), parent_id, this_id);

        auto it = bind.find(pNode);
        if (it != bind.end())
        {
        }
        else
        {
            //FbxMatrix m = pNode->EvaluateGlobalTransform(0);
            vmml::mat4f current_mesh_matrix = getWorldMatrix(pNode);
            float* vm = current_mesh_matrix;
            FbxMatrix m(
                vm[0], vm[1], vm[2], vm[3]
                , vm[4], vm[5], vm[6], vm[7]
                , vm[8], vm[9], vm[10], vm[11]
                , vm[12], vm[13], vm[14], vm[15]
            );
            bind[pNode] = m;
        }

        //���ӽڵ������ȷ��id
        parent_id = this_id;
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        buildBindPose(pNode->GetChild(j), bind, this_id, parent_id);
    }
}

//�ռ�������bindpose���� �������
std::map< FbxNode*, FbxMatrix > getBindPose(FbxScene* scene)
{
    std::map< FbxNode*, FbxMatrix > ret;

    unsigned int num_pose = scene->GetPoseCount();
    for (unsigned int i = 0; i < num_pose; i++)
    {
        FbxPose* pose = scene->GetPose(i);

        //�������bindpose������
        if (!pose->IsBindPose()) continue;

        //������ǰpose�ڵ�ȫ��node
        unsigned int num_node = pose->GetCount();
        for (unsigned int j = 0; j < num_node; j++)
        {
            FbxNode* node = pose->GetNode(j);

            //����õ��Ķ���bind���������
            FbxMatrix m = pose->GetMatrix(j);
            ret[node] = m;

            printf(" -- bindpose %s\n", node->GetName());
        }
    }

    return ret;
}

//�����������������й���Ϣ ������ھͷ��� SkeletonActorControler
egg::SkeletonActorControler* LoadSkeletonActorControler(FbxScene* scene)
{
    //�ж��Ƿ����skin�ı����� ���û�� ��˵��û����
    std::map< std::string, egg::JointMapping > skeleton_mapping;
    bool has_skin_deferoer = false;
    int this_id = -1; //mast be -1
    getSkeleonMapping(scene->GetRootNode(), has_skin_deferoer, skeleton_mapping, this_id);
    if (has_skin_deferoer)
    {
        printf(" -- has skin deferoer\n");
    }
    else
    {
        printf(" -- has no skin deferoer\n");
        return nullptr;
    }

    //������������������
    egg::SkeletonActorControler* actor = new egg::SkeletonActorControler();
    actor->mSkeletonMapping = skeleton_mapping;

    //�ռ�bindpose �������
    std::map< FbxNode*, FbxMatrix > bindpose = getBindPose(scene);

    //����Ҳ���bind��Ϣ ������CS���������ȱʧ����
    //    �����������ȫ������ �õ�һ֡����bindpose��
    //    ǰ����CS�����ĵ�һ֡���ú�bindpose�غ�
    this_id = -1; //mast be -1
    buildBindPose(scene->GetRootNode(), bindpose, this_id);


    //���bind��Ϣ
    actor->mBindPoseMatrix.resize(skeleton_mapping.size());
    actor->mBindPoseMatrixInv.resize(skeleton_mapping.size());
    for (auto it = bindpose.begin(); it != bindpose.end(); it++)
    {
        if (it->first->GetSkeleton())
        {
            //��ǰ����
            double* m = it->second;
            float mf[16] = { 0 };
            for (int i = 0; i < 16; i++) { mf[i] = (float)m[i]; }

            //�����
            FbxMatrix inv = it->second.Inverse();
            double* m_inv = inv;
            float mf_inv[16] = { 0 };
            for (int i = 0; i < 16; i++) { mf_inv[i] = (float)m_inv[i]; }

            //����ӳ�������
            int ske_index = egg::SkeletonActorControler::getSkeletonIndex(it->first->GetName(), skeleton_mapping);
            if (ske_index < 0) continue;

            //��¼bind���������� ��������
            actor->mBindPoseMatrix[ske_index].set(&mf[0], &mf[15], false);
            actor->mBindPoseMatrixInv[ske_index].set(&mf_inv[0], &mf_inv[15], false);
        }
    }


    return actor;
}
