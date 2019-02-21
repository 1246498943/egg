
#include <egg/Object.h>



//#include <nbSG/Node.h>
//#include <nbSG/SceneNode.h>
//
//#include <nbSG/Geometry.h>
//#include <nbSG/Image.h>
//#include <nbSG/Material.h>
//#include <nbSG/Program.h>
//#include <nbSG/Scene.h>
//#include <nbSG/Shader.h>
//#include <nbSG/Texture.h>
//#include <nbSG/Animation.h>
//
//
//#include <nbSG/MeshRender.h>
//#include <nbSG/ActorControler.h>
//#include <nbSG/SkeletonActorControler.h>


using namespace egg;

IMPLEMENT_SINGLETON(ObjectManager)

#define ROC(CLASS) registerObjectClass<CLASS>(#CLASS);

void ObjectManager::registerDefaultObjectClass()
{
    //registerObjectClass<Object>("Object");
    ROC(Object)
    
#if 0
    //��Դ���
    ROC(Geometry)
    ROC(Image)
    ROC(Material)
    ROC(Program)
    ROC(Scene)
    ROC(Shader)
    ROC(Texture)
    ROC(Animation)

    //�������
    ROC(Node)
    ROC(SceneNode)

    //��Ϊ���
    ROC(Transfrom3D)
    ROC(Joint3D)
    ROC(MeshRender)
    ROC(ActorControler)
    ROC(SkeletonActorControler)
    ROC(Light)
    ROC(Camera)
    //ROC(Line)
#endif

}


std::set< observer_ptr<Object> > Object::Objects;

std::string Object::getUniformName(Object* self, std::string name)
{
    std::string ret = name;

    //�ռ���������
    std::set< std::string > name_list;
    for (auto it = Objects.begin(); it != Objects.end(); it++)
    {
        ref_ptr<Object> s = (*it).get();

        std::string n = s->getName();
        if (s.get() != self) name_list.insert(n);

        //��ֹ��ָ�뱻�ͷ�
        s.release();
    }

    //������ھ�һֱѭ��
    while (name_list.find(ret) != name_list.end())
    {
        //////////////////////////////////////////////////////////////////////////
        //���ӵķ�ʽ  �ж�ĩβ������ ����1
        //////////////////////////////////////////////////////////////////////////

        //ȷ�����һ����Ч���ֵĿ�ʼindex
        int shuzu_index = ret.size();//���������ʾ�������ҵ�����
        for (int i = ret.size() - 1; i >= 0; i--)
        {
            if (ret[i] >= '0' &&  ret[i] <= '9')
            {
                shuzu_index = i;
            }
            else
            {
                break;
            }
        }

        //�ж��Ƿ�û��������
        if (shuzu_index == ret.size())
        {
            ret += "1";
        }

        //�ҵ�������
        else
        {
            //�õ���������
            std::string shuzi = ret.substr(shuzu_index, ret.size() - shuzu_index);
            int int_shuzi = atoi(shuzi.c_str()) + 1;

            //������
            char bu[128] = {};
            sprintf(bu, "%d", int_shuzi);
            ret = ret.substr(0, shuzu_index) + bu;
        }
    }

    return ret;
}

void Object::addObject(Object* object)
{
    Objects.insert(object);
}

void Object::delObject(Object* object)
{
    auto it = Objects.find(object);
    if (it != Objects.end())
    {
        Objects.erase(it);
    }
}

//void Object::cleanup()
//{
//    //for (auto it = Objects.begin(); it != Objects.end(); it++)
//    //{
//    //    if(it->release)
//    //}
//
//}

