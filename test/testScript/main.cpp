
#include <egg/Reflection/ObjectHandle.h>
#include <egg/Command/Console.h>
#include <string>

//#include <Runtime/Runtime.h>

//���runtime����Ĳ���
#define FILENAME_HISTORY "testScriptHistory.nezha"
#define FILENAME_OBJECTS "testScriptObjects.nezha"

/*
* �����з�������ʽ
*   1 ��������Լ�¼ Ҳ���Ǵ洢�õ���ʽ
*   2 ������ʽ ��¼��ʷ ����ֱ�����о�������
*   ��¼��ʽֻ�����޵�������ɣ�������¼�߼����
*   ��Ϊ����ϵͳ�Ƿ���ġ�
*   ��¼��ʽֻ�ж�����������Բ��������а汾��Ϣ��
*/

int main(int argc, char *argv[])
{
    //Runtime* runtime = Runtime::getInstance();
    egg::Context* context = new egg::Context();
    egg::Console* console = new egg::Console();
    egg::Parser* parser = new egg::Parser();

    //��ȡ��ǰ����Ŀ
    auto project = context->getGloble("project");

    //ͨ�������б༭project����
    std::string script = {
        //"test 111 222 333;"
        //"test aaa bbb ccc;"
        //" test  AAA  BBB  CCC ; "

        //�������ƶ�����Ҫ���� ����Ҳ�����пո�
        //ֻ����������������

        "select project;"
        "setattr name \"��������ս\";"

        "selattr actor;"
        "push;"

        "newobject actor;"
        "addref;"

        "pop;"
    };

    console->runScript(
        parser, context, script.c_str(), script.size());

    //����������ʷ��¼���ı�
    console->saveHistroyScriptFile(
        parser, context, FILENAME_HISTORY);

    //������Ŀ���ļ� ������������� ���ɶ�Ӧ������
    console->saveObjectScriptFile(
        parser, context, FILENAME_OBJECTS);

#if 0
    //�½���Ŀ
    runtime->mContext->setGloble("project", new Project());

    //����Ŀ�ļ� ֱ�����нű��ļ�
    runtime->mConsole->runScriptFile(runtime->mParser, runtime->mContext, FILENAME_HISTORY);



    //�½���Ŀ
    runtime->mContext->setGloble("project", new Project());

    //����Ŀ�ļ� ֱ�����нű��ļ�
    runtime->mConsole->runScriptFile(runtime->mParser, runtime->mContext, FILENAME_OBJECTS);
#endif

}
