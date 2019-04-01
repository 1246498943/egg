
#ifndef _NSG_CORE_OBJECT_H_
#define _NSG_CORE_OBJECT_H_

#include <egg/Export.h>

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include <egg/ref_ptr.h>
#include <egg/observer_ptr.h>

#include <egg/Path.h>
#include <egg/Serializer.h>
#include <egg/Factory.h>

#include <egg/Event.h>


namespace egg
{

    class ObjectManager;

    /*
    *  ����������
    *  �����л��Ļ�����
    */
    class EGG_RUNTIME_EXPORT Object
        :public Referenced
    {
    public:

        Object(std::string name = "object")
        {
            //std::string name = getUniformName("object");
            setName(name);
            addObject(this);
        }
        virtual ~Object()
        {
            delObject(this);
        }

        virtual std::string ObjectClassName(){return "object";}

        //����
        void setName(std::string name)
        { 
            name = getUniformName(this, name);
            mName = name;
        }
        std::string getName() { return mName; }


        //���л�����֧��
        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const
        {
            ser->writeString(os, mName); //����
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser 
            , ObjectManager* om , Referenced* data , bool load_ext )
        {
            ser->readString(is, mName); //����
        }

        //ͳһ���ƹ��ܽӿ�
        static std::string getUniformName(Object* self,std::string name);
        static void addObject(Object* object);
        static void delObject(Object* object);
        //static void cleanup();

        //�������Ӻͼ���֧��
        //���ȴ���ӹ��ܣ�


        //listenerģʽ
        //���ȴ���ӹ��ܣ�
        void addListender(Object* listender)
        {
            mListenders.insert(listender);
        }
        void removeListender(Object* listender)
        {
            auto it = mListenders.find(listender);
            if (it != mListenders.end())
            {
                mListenders.erase(it);
            }
        }
        void fireEvent(Event* event)
        {
            //for (size_t i = 0; i < mListenders.size(); i++)
            for (auto it = mListenders.begin(); it != mListenders.end(); it++)
            {
                //������Ϣ
                ref_ptr<Object> obj = (*it).get();
                obj->onEvent(this, obj.get(), event);

                //Ϊ�˱�֤��ָ�벻���ͷ�
                obj.release();
            }
        }
        virtual void onEvent(Object* from, Object* to, Event* event) {}

    private:

        //����ͳ��ȫ��Object��ʵ��
        // ��Ϊ���캯����װ���� ���������������ָ��
        //static std::set< ref_ptr<Object> > Objects;
        static std::set< Object* > Objects;

    private:

        std::string mName;

        std::set< observer_ptr<Object> > mListenders;

    };

    //���󹤳�
    class ObjectFactory
        :public Referenced
    {
    public:
        ObjectFactory(){}
        virtual ~ObjectFactory(){}
        virtual Object* newObject() = 0;
    };

    //���󹤳� ģ����
    template<class T>
    class ObjectFactoryT
        :public ObjectFactory
    {
    public:
        ObjectFactoryT(){}
        virtual ~ObjectFactoryT(){}
        Object* newObject()
        {
            return new T();
        }
    };

    //���������
    class EGG_RUNTIME_EXPORT ObjectManager
        :public Object
    {
    public:
        ObjectManager() { registerDefaultObjectClass(); }
        virtual ~ObjectManager(){}

        //manager(ObjectManager)
        DECLARE_SINGLETON(ObjectManager)

        template<class T>
        void registerObjectClass( const std::string& classname )
        {
            //ǿ��ע��Сд����
            std::string a = classname;
            std::transform(a.begin(), a.end(), a.begin(), ::tolower);

            mObjectFactory[a] = new ObjectFactoryT<T>();
        }

        Object* newObject( const std::string& classname )
        {
            //auto it = mObjectFactory.find(classname);
            //if(it != mObjectFactory.end() )
            //{
            //    return it->second->newObject();
            //}

            //���Դ�Сд ǿ��ʹ��Сд
            for (auto it = mObjectFactory.begin(); it != mObjectFactory.end(); it++)
            {
                std::string a = classname;
                std::string b = it->first;

                std::transform(a.begin(), a.end(), a.begin(), ::tolower);
                std::transform(b.begin(), b.end(), b.begin(), ::tolower);

                if( a == b )
                {
                    return it->second->newObject();
                }
            }
            return nullptr;
        }

        void registerDefaultObjectClass();

    private:

        std::map< std::string , ref_ptr< ObjectFactory > > mObjectFactory;
    };


}//namespace nbSG


#endif //_NSG_CORE_OBJECT_H_
