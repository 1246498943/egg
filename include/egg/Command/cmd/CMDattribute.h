
#ifndef _NEZHA_CMDattr_H_
#define _NEZHA_CMDattr_H_

#include <egg/Command/Command.h>
#include <egg/Command/Context.h>
//#include <Runtime/Runtime.h>

/*
�Ժ��ƶ��� CLR ��

//��Ե�ǰobject�ĵ����Բ���
typeattr ��ȡ��������
setattr  ��������ֵ
getattr  ��ѯ����ֵ
addattr  ׷����չ����Ԫ�أ��б�Ҫ��
delattr  ɾ����չ����Ԫ�أ��б�Ҫ��

//���Զ�ջ
selectattr ѡ��ǰ��������Ե�������

//ref����
setref      ���ö�ջtop����ǰ����
addref      ׷�Ӷ�ջtop����ǰ����
insertref   ׷�Ӷ�ջtop����ǰ����

*/

class CMDsetattr
    :public egg::Command
{
public:
    CMDsetattr(const char* name = "setattr") :egg::Command(name) {}
    virtual ~CMDsetattr() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("setattr");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //�������
        if (pl.size() < 2) return "";

        //get��ǰ����
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrent();
        if (obj == 0) return "";

        //��������
        egg::ObjectHandle::handle(obj)->getAttr(pl[0].c_str())->setDataString(obj, pl[1].c_str());

        return "";
    }
};

class CMDselectattr
    :public egg::Command
{
public:
    CMDselectattr(const char* name = "selectattr") :egg::Command(name) {}
    virtual ~CMDselectattr() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("selectattr");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //�������
        if (pl.size() < 1) return "";

        //get��ǰ����
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrent();
        if (obj == 0) return "";

        //���õ�ǰ����
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.setCurrentAttr(obj, pl[0].c_str());

        return "";
    }
};


class CMDsetref
    :public egg::Command
{
public:
    CMDsetref(const char* name = "setref") :egg::Command(name) {}
    virtual ~CMDsetref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("setref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //���õ�ǰ����
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //��ǰ����
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //��������ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->setObject(obj, obj_cur);

        return "";
    }
};

class CMDaddref
    :public egg::Command
{
public:
    CMDaddref(const char* name = "addref") :egg::Command(name) {}
    virtual ~CMDaddref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("addref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //���õ�ǰ����
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //��ǰ����
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //��������ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->pushObject(obj, 0, obj_cur);

        return "";
    }
};

class CMDinsertref
    :public egg::Command
{
public:
    CMDinsertref(const char* name = "insertref") :egg::Command(name) {}
    virtual ~CMDinsertref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("insertref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //�������
        if (pl.size() < 1) return "";

        //���õ�ǰ����
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //��ǰ����
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //��������ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->insertObject(obj, 0, pl[0].c_str(), obj_cur);

        return "";
    }
};


#endif //_NEZHA_CMDattr_H_
