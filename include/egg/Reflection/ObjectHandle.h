#ifndef _CLR_ObjectHandle_H_
#define _CLR_ObjectHandle_H_

#include <egg/Export.h>

#include <vector>
#include <string>
#include <map>
#include <set>

#include <egg/ref_ptr.h>
#include <egg/Object.h>
#include <egg/Reflection/ValueHandle.h>


namespace egg
{

    //���������
    class EGG_RUNTIME_EXPORT ObjectHandle
        :public Referenced
    {
    public:
        ObjectHandle(const char* className = "");
    public:
        virtual ~ObjectHandle();

        ///////////////////////////////////////////////
        // ��̬����
        ///////////////////////////////////////////////

        //�����Ѿ����ڵĶ���
        static ref_ptr<ObjectHandle> handle(Object* object);

        //template <class T>
        //static CLRPTR<ObjectHandle> handle(T* object)
        //{
        //    for (auto it = mObjectHandle.begin(); it != mObjectHandle.end(); it++)
        //    {
        //        std::string typeID = typeid(*object).name();

        //        if (it->first == typeID)
        //        {
        //            return it->second;
        //        }
        //    }
        //    return 0;
        //}

        //���������handle
        //static Object* create(const char* className);

        //ע��handle
        static void registorHandle(ObjectHandle* objectHandle);

        //����handle
        static ref_ptr<ObjectHandle> getHandle(const char* className);
        static ref_ptr<ObjectHandle> getHandle(Object* objectHandle);

        //��̬��������
        //  ��Ϊֻ�ж�̬���Ϳ���ֱ��ȷ����ǰ�������ʵclass
        static std::map<std::string, ref_ptr<ObjectHandle> > mObjectHandle;

        ///////////////////////////////////////////////
        // ö�����Ͳ���
        ///////////////////////////////////////////////

        //ö�ٲ���
        void addEnum(std::string enum_name, std::string value, int i)
        {
            mEnumMapping[enum_name][value] = i;
        }
        int getEnum(std::string enum_name, std::string value)
        {
            auto it = mEnumMapping.find(enum_name);
            if (it != mEnumMapping.end())
            {
                auto it2 = it->second.find(value);
                if (it2 != it->second.end())
                {
                    return it2->second;
                }
            }
            return -1;
        }
        std::string getEnum(std::string enum_name, int i)
        {
            for (auto it = mEnumMapping.begin(); it != mEnumMapping.end(); it++)
            {
                for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    
                        if (it2->second == i) return it2->first;
                    
                }
            }
            return "";
        }
        std::map<std::string, std::map<std::string, int> > mEnumMapping;



        ///////////////////////////////////////////////
        // �̳������
        ///////////////////////////////////////////////

    public:
        std::string getClassName() { return mClassName; }
        std::string getClassID() { return mClassID; }
        //virtual Object* create() { return 0; }
        //virtual Object* getObject() { return 0; }

    protected:
        std::string mClassName;
        std::string mClassID;
        std::vector< ref_ptr<ObjectHandle> > mParentObjectHandle;

    //private:
        //CLROBJECT* mObject;

        ///////////////////////////////////////////////
        // ���Բ���
        ///////////////////////////////////////////////
    public:
        std::vector<std::string> getAttrNames();

        ref_ptr<ValueHandle> getAttr(const char* name);

    protected:
        void registorAttr(ValueHandle* valueHandle);
        std::map<std::string, ref_ptr<ValueHandle> > mAttrs;

    private:

    };


#if 0
    //ͳһ��������
    class CLR_EXPORT UniformNamingObject
    {
    public:

        static UniformNamingObject* getInstance();
        static void delInstance();
        static UniformNamingObject* Instance;

    protected:

        UniformNamingObject() {}
        ~UniformNamingObject() {}

    public:
        void addObject(CLROBJECT* object)
        {
            mStack.insert(object);
        }

        void delObject(CLROBJECT* object)
        {
            auto it = mStack.find(object);
            if (it != mStack.end())
            {
                mStack.erase(it);
            }
        }

        //��������������� �Զ���Դһ���������Ľ��
        std::string getUniformName(std::string name)
        {
            return name;
        }

    private:

        //������Ͱ���name
        std::set<CLROBJECT*> mStack;

        //��ѯ���ж������Ƿ��������
        bool exist(std::string name)
        {
            return false;
        }
    };
#endif


}//namespace egg

#endif // !_CLR_ObjectHandle_H_
