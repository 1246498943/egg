
#ifndef _NEZHA_CMDsystem_H_
#define _NEZHA_CMDsystem_H_

#include <egg/Command/Command.h>

/*
����ϵͳ���������

version ���汾����
date    ��������
info    �����ļ�������Ϣ

*/

class CMDversion
    :public egg::Command
{
public:
    CMDversion(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDversion() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};

class CMDdate
    :public egg::Command
{
public:
    CMDdate(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDdate() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};

class CMDinfo
    :public egg::Command
{
public:
    CMDinfo(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDinfo() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};


#endif //_NEZHA_CMDsystem_H_
