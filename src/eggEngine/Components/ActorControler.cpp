
#include <eggEngine/Components/ActorControler.h>

using namespace egg;



void ActorControler::onUpdate(nv_update* nv)
{
    //printf("a");
    float dt = 0.1f; //����ʱ��

    //��ǰ����ʱ��
    static double time = 0;
    time += dt;

    //���� �ӵ�ǰ��animation�еõ�
    //�������Ӧ�����ƶ�����

    //�����������ص�
    //nbSG::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<nbSG::SkeletonActorControler>();
    //if(!sac) return;

    ////��ȡjoint��ɫ�ռ����(�������������ڽڵ�Ŀռ�)
    ////   ������̿������÷ǳ���Ľ����߼���ʵ�ָ��Ӷ�������
    //std::vector<float> joint_matrix;
    //joint_matrix.resize(sac->mSkeletonMapping.size()*16);
    //float idm[16] = {1,0,0,0  ,0,1,0,0  ,0,0,1,0  ,0,0,0,1};
    //for (unsigned int i = 0; i < joint_matrix.size()/16; i++)
    //{
    //    memcpy(&joint_matrix[i*16],idm,sizeof(float)*16);//�ָ���λ����
    //}

    ////��������ڵ��local����
    //nbSG::Animation* anim = sac->getAnimation();
    //if(anim)
    //{
    //    //�������ʱ��
    //    int fps = 25;  //���貥�Ŷ�����fps
    //    int frame_num = anim->getFrameNum(); //����֡��
    //    float count = float(frame_num-1)/float(fps);//��������ʱ��s
    //    float current = fmodf(time,count); //��ǰ����ѭ���е�ʱ��
    //    float current_frame = current*fps; //��ǰ��ֵ֡��
    //    //printf("    %f\n",current_frame);

    //    for (auto it = anim->getIndex().begin(); it != anim->getIndex().end(); it++)
    //    {
    //        std::string name = it->first;

    //        //���ֵ
    //        vmml::mat4f r = anim->interpolation( name , current_frame );

    //        //��ֵlocal
    //        int id = nbSG::SkeletonActorControler::getSkeletonIndex(name,sac->mSkeletonMapping);
    //        if(id >= 0)
    //        {
    //            float* ra = r;
    //            memcpy(&joint_matrix[id*16],ra,sizeof(float)*16);
    //        }
    //    }
    //}

    ////����world���� ��¼��joint_matrix��
    //computeWorldMatrix( -1 , sac->mSkeletonMapping , joint_matrix);


    //applyMatrix( "" , vmml::mat4f mat );
}

#if 0
void ActorControler::applyMatrix( std::string , vmml::mat4f mat )
{


}
#endif
