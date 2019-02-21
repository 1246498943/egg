
#include <egg/Command/Context.h>

#include <egg/Command/cmd/CMDsystem.h>
#include <egg/Command/cmd/CMDobject.h>
#include <egg/Command/cmd/CMDattribute.h>

using namespace egg;


//�Ժ���ڶ���λ��
class ObjectHandleObject
    :public egg::ObjectHandle
{
public:
    ObjectHandleObject(const char* className = "Object")
        :egg::ObjectHandle(className)
    {
        std::string typeID = typeid(Object).name();
        mClassID = typeID;

        //mInheritance.push_back();

        {
            egg::ValueHandle* vh_name = new egg::ValueHandle("std::string", "name", egg::VHVT_Value, egg::VHST_Single);
            vh_name->getDataString = [=](egg::Object* object) {return (((Object*)object)->getName()); };
            vh_name->setDataString = [=](egg::Object* object, std::string str) {((Object*)object)->setName(str); };
            this->registorAttr(vh_name);
        }
    }
    virtual ~ObjectHandleObject() {}
};


void Context::registerDefault()
{
    egg::ObjectManager* om = egg::ObjectManager::getInstance();

    //ע��������� Object
    om->registerObjectClass<Object>("Object");
    egg::ObjectHandle::registorHandle(new ObjectHandleObject());


    //ϵͳ����
    registorConmmand(new CMDversion());
    registorConmmand(new CMDdate());
    registorConmmand(new CMDinfo());

    //��������
    registorConmmand(new CMDnew());
    registorConmmand(new CMDdel());
    registorConmmand(new CMDselect());
    registorConmmand(new CMDpush());
    registorConmmand(new CMDpop());
    registorConmmand(new CMDrename());

    //��������
    registorConmmand(new CMDsetattr());
    registorConmmand(new CMDselectattr());
    registorConmmand(new CMDsetref());
    registorConmmand(new CMDaddref());
    registorConmmand(new CMDinsertref());

}


//Context::Context()
//{
//}
//
//Context::~Context()
//{
//}

