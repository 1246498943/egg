#ifndef NIUBI_SG_SCENENODE_H
#define NIUBI_SG_SCENENODE_H

//#include <nbBase/Referenced>
#include <eggRuntime/Node.h>
#include <eggEngine/Component.h>

namespace egg
{

    /*
    * �����ڵ�
    */

    class EGG_ENGINE_EXPORT SceneNode
        :public Node
    {
    public:
        SceneNode(const std::string& name = "scenenode")
            :Node(name) {}
        virtual ~SceneNode() {}

        virtual std::string ObjectClassName() { return "scenenode"; }

        //��ֹComponent�����
        virtual void addChild(Node* child) override
        {
            if (!dynamic_cast<ComponentBase*>(child))
            {
                Node::addChild(child);
            }
        }

        //��Ϣ�ּ�
        virtual void traverse(NodeVisitor& nv);

        //�����Ϊ���
        //template<class T>
        bool addComponent(ComponentBase* component);

        //��ѯ��Ϊ���
        //  ������ComponentBase�̳���
        //  ���������� ֻ���ص�һ�� ������������ʾ
        template<class T>
        T* getComponent()
        {
            //����ȫ���ӽڵ� �ҵ���һ������Ҫ��ľͷ���
            for (auto it = mComponents.begin(); it != mComponents.end(); it++)
            {
                T* v = dynamic_cast<T*>((*it).get());
                if (v) return v;
            }
            return nullptr;
        }

        //ɾ����ͬ���͵���Ϊ���
        template<class T>
        void removeComponent(T* component)
        {
            for (auto it = mComponents.begin(); it != mComponents.end(); it++)
            {
                T* v = dynamic_cast<T*>((*it).get());
                if (v)
                {
                    (*it)->onDetach();
                    mComponents.erase(it);
                    return;
                }
            }
        }

        //��ѯ��Ϊ���������
        unsigned int getComponentCount() { return mComponents.size(); }

        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;

        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext)override;

        //��ʱ��ȡ����б�  �Ժ����޸ĵ����ŵ�
        std::vector< ref_ptr< ComponentBase > > getComponentList();

    private:

        //������ֱ�
        std::set< ref_ptr< ComponentBase > > mComponents;
    };

}//namespace egg

#endif//NIUBI_SG_SCENENODE_H
