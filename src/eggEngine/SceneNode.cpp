
#include <eggEngine/SceneNode.h>

#include <eggEngine/RenderWindow.h>

using namespace egg;

void SceneNode::traverse(NodeVisitor& nv)
{
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        (*it)->accept(nv);
    }

    //����
    Node::traverse(nv);

    if (nv.as<nv_update>())
    {
        //������SceneNode������ɵ�ʱ���
    }
}

bool SceneNode::addComponent(ComponentBase* component)
{
    //�������ظ�����
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        std::string a = (*it)->ObjectClassName();
        std::string b = component->ObjectClassName();

        if (a == b)
        {
            return false;
        }
    }

    //���û�и��Ӹ���Ľڵ� ���������
    if (!component->mAttachNode.get().valid())
    {
        component->mAttachNode = this;
        mComponents.insert(component);
        component->onAttach();
        return true;
    }

    return false;
}

void SceneNode::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Node::save(filename, os, ser);

    //д��ȫ���������
    ser->writeNumber(os, (unsigned int)mComponents.size());   //����
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        ser->writeString(os, (*it)->ObjectClassName());//����

        (*it)->save(filename, os, ser);  //����
    }
}

void SceneNode::load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Node::load(filename, is, ser, om, rmg, load_ext);

    //��ȡ���й������
    unsigned int com_num = 0;
    ser->readNumber(is, com_num); //������
    for (unsigned int i = 0; i < com_num; i++)
    {
        std::string com_type_name;
        ser->readString(is, com_type_name);//����

        Object* obj = om->newObject(com_type_name);
        obj->load(filename, is, ser, om, rmg, load_ext);      //����

        ComponentBase* cb = dynamic_cast<ComponentBase*>(obj);
        //mComponents.insert(cb);
        addComponent(cb);
    }
}

//��ʱ��ȡ����б�  �Ժ����޸ĵ����ŵ�
std::vector< ref_ptr< ComponentBase > > SceneNode::getComponentList()
{
    std::vector< ref_ptr< ComponentBase > > ret;
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        ret.push_back(*it);
    }
    return ret;
}


