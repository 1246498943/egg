#ifndef _EGG_EntityManager_H_
#define _EGG_EntityManager_H_

#include <egg/Runtime.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>
#include <eggEngine/Animation.h>


namespace egg
{
    //�����Զ���
    //  ֧��default����
    //  ֧��dirty״̬
    //  ֧�ּ����л�
    class EGG_ENGINE_EXPORT Property
    {
    public:
        Property() {}
        ~Property() {}
    };

    //ģ��
    template<typename T>
    class PropertyT
        :public Property
    {
    public:
        PropertyT() {}
        ~PropertyT() {}

        void set(T v) { mValue = v; }
        T get() { return mIsSet ? mValue : mDefaultValue; }

        bool isSet() { return mIsSet; }
        void setDefault() { mValue = mDefaultValue; }
        void clear() { mIsSet = false; }

    private:

        bool mIsSet = false;
        T mDefaultValue;
        T mValue;
    };

    //�������ݶ���
    typedef PropertyT< int > PropertyInt;
    typedef PropertyT< float > PropertyFloat;
    typedef PropertyT< double > PropertyDouble;


    //ʵ�����
    class EGG_ENGINE_EXPORT Entity
        :public Object
    {
    public:
        Entity(unsigned int UID = 0)
            :mUID(UID)
            , mSpawn(false)
            , mBind(false)
        {}
        virtual ~Entity() {}

        //������Ϣ
        void setUID(unsigned int uid) { mUID = uid; }
        unsigned int getUID() { return mUID; }
        //virtual std::string getClassName() = 0;

        //���ӹ�ϵ
        void addChild(Entity* child);
        int getNumChildren();
        Entity* getChild(int index);
        void removeChild(Entity* entity);
        void removeChild(int index);

        //new�����Ϣ
        virtual void onCreate() {};

        //delete֮ǰ����Ϣ
        virtual void onDestroy() {};

        //ֱ�Ӳ���
        //  ��ʵ�帺����SN���ݣ�Ҳ�����ǿյġ�
        virtual void onSpawn(SceneNode* parent, Eigen::Vector3f pos, Eigen::Vector3f rot, Eigen::Vector3f scale) {};

        //��
        //  SN�Ѿ�����,������ʼ��һ��entity����Ҳ�����ǿ�sn��
        virtual void onBind(Entity* parent, SceneNode* self) {};

        //����
        //  ����ʵ����󡣲��һ�����entity�������Դ��
        virtual void onUnSpawn(SceneNode* parent) {};

        //�������ڵ�
        //  ������ڵ㷢���仯�����繤�߽�����ͬ���ˡ�
        virtual void onChangeParent(Entity* parent) {};

        //�޸�����
        //  ĳ�����Ա仯�󣬸���һ��������ʾ��
        virtual void onPropertyChange(Property old_property, Property new_property) {};

        //���Ա��޸�
        virtual void onPropertyDirty(Property property) {};

        //��Ⱦ������Ϣ
        virtual void onBegin() {};
        virtual void onUpdate() {};
        virtual void onCull() {};
        virtual void onDraw() {};
        virtual void onSwapBuffer() {};

        //ͨ����Ϣ
        virtual void onMessage(Entity* from, Entity* to, const std::string& message) {};

        //�߼���Ϣ
        virtual void onEnter() {}; /*��������*/
        virtual void onExit() {};  /*�뿪����*/
        virtual void onHit() {};   /*������*/
        virtual void onKill() {};  /*ǿ������*/
        virtual void onStateChange() {}; /*״̬�ı�*/


        //���Բ���
        //   �������������ԣ��������ԣ������ԡ�
        //   ���ǲ���Ҫ������ô���ӵ�֧�֡�
        //   ������Ҫһ����Ĭ��ֵ�����Լ�¼dirty�����ԡ�ֻ��Ҫ��¼�򵥵�����������������Ҫ���ӵĶ���
        //   ���Բ���Ҫ���䡣ֻҪ֧�ּ򵥵����л����ɡ�����Ҫ�̳���Object��
        Property* getProperty(const std::string& name) { return 0; }
        void setProperty(const std::string& name, Property* p) {}

    protected:

    private:

        //ȫ��UID
        unsigned int mUID;

        //�Ƿ��Ѿ�������
        bool mSpawn;

        //�Ƿ��ǰ󶨶���
        bool mBind;

        //��Entity
        std::vector< ref_ptr<Entity> > mChildren;
    };


    //ȫ��ʵ�幤��
    class EGG_ENGINE_EXPORT EntityFactory
        :public Factory<Entity>
    {
    public:
        EntityFactory() {}
        ~EntityFactory() {}
        DECLARE_SINGLETON(EntityFactory)

        Entity* createEntity(const std::string& type_name);
    };


}//namespace egg

#endif //_EGG_EntityManager_H_

