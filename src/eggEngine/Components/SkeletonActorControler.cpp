#include <eggEngine/Components/SkeletonActorControler.h>

#include <eggEngine/Material.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Components/MeshRender.h>

using namespace egg;

class nv_find_material
    :public NodeVisitor
{
public:
    nv_find_material() {}
    virtual ~nv_find_material() {}
    virtual void apply(Node& node) override
    {
        SceneNode* sn = dynamic_cast<SceneNode*>(&node);
        if (sn)
        {
            MeshRender* meshrender = sn->getComponent<MeshRender>();
            if (meshrender)
            {
                for (unsigned int i = 0; i < meshrender->getNumMaterial(); i++)
                {
                    Material* m = meshrender->getMaterial(i);
                    if (m)
                    {
                        mMaterialList.insert(m);
                    }
                }

            }
        }
        traverse(node);
    }

    std::set< ref_ptr< Material > > mMaterialList;
};



void SkeletonActorControler::computeWorldMatrix(int index, const std::map< std::string, JointMapping >& sm, std::vector<float>& joint_matrix)
{
    //�����Լ��ľ���
    if (index < int(joint_matrix.size() / 16) && index >= 0)
    {
        int pid = SkeletonActorControler::getSkeletonParent(index, sm);
        if (pid >= 0)
        {
            //������
            //vmml::mat4f pm;
            //pm.set(&joint_matrix[pid * 16], &joint_matrix[pid * 16 + 15], false);
            Eigen::Matrix4f pm(&joint_matrix[pid * 16]);

            //��ǰlocal����
            //vmml::mat4f lm;
            //lm.set(&joint_matrix[index * 16], &joint_matrix[index * 16 + 15], false);
            Eigen::Matrix4f lm(&joint_matrix[index * 16]);

            //���ǵ�ǰ����Ϊworld
            Eigen::Matrix4f gm = pm * lm;
            float* ga = gm.data();
            memcpy(&joint_matrix[index * 16], ga, sizeof(float) * 16);
        }
    }

    //�����ӽڵ�
    std::vector<int> ch = SkeletonActorControler::getSkeletonChild(index, sm);
    for (unsigned int i = 0; i < ch.size(); i++)
    {
        computeWorldMatrix(ch[i], sm, joint_matrix);
    }
}

void SkeletonActorControler::onUpdate(nv_update* nv)
{
    float dt = nv->dt;//0.01; //����ʱ��

    //��ǰ����ʱ��
    //if( start_time<0 ) start_time = dt;
    current_time += dt;

    //printf("dt = %f time = %f\n",dt,current_time);


    //���� �ӵ�ǰ��animation�еõ�
    //�������Ӧ�����ƶ�����

    //�����������ص�
    //nbSG::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<nbSG::SkeletonActorControler>();
    //if(!sac) return;

    //��ȡjoint��ɫ�ռ����(�������������ڽڵ�Ŀռ�)
    //   ������̿������÷ǳ���Ľ����߼���ʵ�ָ��Ӷ�������
    std::vector<float> joint_matrix;
    joint_matrix.resize(this->mSkeletonMapping.size() * 16);
    float idm[16] = { 1,0,0,0  ,0,1,0,0  ,0,0,1,0  ,0,0,0,1 };
    for (unsigned int i = 0; i < joint_matrix.size() / 16; i++)
    {
        memcpy(&joint_matrix[i * 16], idm, sizeof(float) * 16);//�ָ���λ����
    }

    //��������ڵ��local����
    Animation* anim = this->getAnimation();
    if (anim)
    {
        //�������ʱ��
        int fps = 25;  //���貥�Ŷ�����fps
        int frame_num = anim->getFrameNum(); //����֡��
        float count = float(frame_num - 1) / float(fps);//��������ʱ��s
        float current = fmodf(current_time, count); //��ǰ����ѭ���е�ʱ��
        float current_frame = current*fps; //��ǰ��ֵ֡��
                                           //printf("    %f\n",current_frame);

        for (auto it = anim->getIndex().begin(); it != anim->getIndex().end(); it++)
        {
            std::string name = it->first;

            //���ֵ
            Eigen::Matrix4f r = anim->interpolation(name, current_frame);

            //��ֵlocal
            int id = SkeletonActorControler::getSkeletonIndex(name, this->mSkeletonMapping);
            if (id >= 0)
            {
                float* ra = r.data();
                memcpy(&joint_matrix[id * 16], ra, sizeof(float) * 16);
            }
        }
    }

    //����world���� ��¼��joint_matrix��
    computeWorldMatrix(-1, this->mSkeletonMapping, joint_matrix);


    //�����ӽڵ��ϵ�ȫ��material����
    ref_ptr<nv_find_material> find_material = new nv_find_material();
    Node* parent = this->getNode();
    if (parent)
    {
        //�������µ�ȫ������
        parent->accept(*find_material);

        //�����������
        for (unsigned int i = 0; i < mSkeletonMapping.size(); i++)
        {
            //���¹�����uniform  ����bindpose inv matrix * bone world matrix
            //osg::Matrix bone(&joint_matrix[i*16]);
            //osg::Matrix inv  = vmml2osg(sac->mBindPoseMatrixInv[i]);
            //u->setElement(i,inv * bone);

            //vmml::mat4f bone;
            //bone.set(&joint_matrix[i * 16], &joint_matrix[i * 16 + 15], false);
            Eigen::Matrix4f bone(&joint_matrix[i * 16]);
            bone = bone * mBindPoseMatrixInv[i];

            float* b = bone.data();
            memcpy(&joint_matrix[i * 16], b, sizeof(float) * 16);
        }

        //ѭ��ȫ������
        for (auto it = find_material->mMaterialList.begin(); it != find_material->mMaterialList.end(); it++)
        {
            Pass* pass = (*it)->getTech((*it)->getCurrentTech())->mPasses[0].get();

            //��������
            pass->setUniform("boneMatrices", UniformBase::UniformDataType::UDT_MAT4, &joint_matrix[0], joint_matrix.size(), "");
        }
    }//if(parent)

    return;
}


