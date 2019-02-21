
#ifndef _NEZHA_CMDobject_H_
#define _NEZHA_CMDobject_H_

#include <egg/Command/Command.h>
#include <egg/Command/Context.h>
//#include <Runtime/Runtime.h>

/*
�Ժ��ƶ��� CLR ��

//����ɾ��
new ��������
del ɾ������

//�����ջ����
select    ѡ��ǰ���� ͨ��ȫ������
push      ��ǰ������ջ
pop       ��ջ����

//��Ե�ǰ��ջ�Ĳ���
rename    ������

*/

class CMDnew
    :public egg::Command
{
public:
    CMDnew(const char* name = "new") :egg::Command(name) {}
    virtual ~CMDnew() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("new");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //�������
        if (pl.size() < 1) return "";

        //��������
        egg::ObjectManager* om = egg::ObjectManager::getInstance();
        egg::Object* obj = om->newObject(pl[0].c_str());

        //��������
        //Runtime* runtime = Runtime::getInstance();
        //std::string name = CLR::UniformNamingObject::getInstance()->getUniformName(pl[0]);
        //egg::ObjectHandle::handle(obj)->getAttr("name")->setDataString(obj, pl[0]);
        obj->setName(pl[0]);

        //����Ϊ��ǰѡ�����
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.setCurrent(obj);

        //��������
        return obj->getName();
    }
};

class CMDdel
    :public egg::Command
{
public:
    CMDdel(const char* name = "del") :egg::Command(name) {}
    virtual ~CMDdel() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("del");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");


        //��������
        return "";
    }
};


class CMDselect
    :public egg::Command
{
public:
    CMDselect(const char* name = "select") :egg::Command(name) {}
    virtual ~CMDselect() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("select");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //�������
        if (pl.size() < 1) return "";

        //��������
        //Runtime* runtime = Runtime::getInstance();
        auto it = context->getGloble(pl[0].c_str());
        if (!it.valid()) return "";

        //����Ϊ��ǰѡ����� ʹ��ȫ��·��
        context->mResultStack.setCurrent(it.get());

        //��������
        return "";
    }
};


class CMDpush
    :public egg::Command
{
public:
    CMDpush(const char* name = "push") :egg::Command(name) {}
    virtual ~CMDpush() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("push");
        printf("\n");

        //��ջ
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.push();

        //��������
        return "";
    }
};

class CMDpop
    :public egg::Command
{
public:
    CMDpop(const char* name = "pop") :egg::Command(name) {}
    virtual ~CMDpop() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("pop");
        printf("\n");

        //��ջ
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.pop();

        //��������
        return "";
    }
};


class CMDrename
    :public egg::Command
{
public:
    CMDrename(const char* name = "rename") :egg::Command(name) {}
    virtual ~CMDrename() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("rename");
        printf("\n");


        //��������
        return "";
    }
};

#endif //_NEZHA_CMDobject_H_
