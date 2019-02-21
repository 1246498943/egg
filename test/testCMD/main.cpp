
#include <egg/Reflection/ObjectHandle.h>
#include <egg/Command/Console.h>
#include <string>


class CMD_test
    :public egg::Command
{
public:
    CMD_test(const char* name = "test") :egg::Command(name) {}
    virtual ~CMD_test() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("test");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        return "return string";
    }
};



void test_cmd()
{
    //��������

    //ִ��������Ҫ��һ��context����
    egg::Context* context = new egg::Context();

    //ע���������
    context->registorConmmand(new CMD_test());

    //ִ������ ������Լ�����
    egg::CommandData cmdd;
    cmdd.push_back("test");
    cmdd.push_back("p0");
    cmdd.push_back("p1");
    cmdd.push_back("p2");
    cmdd.push_back("p3");
    context->runConmmand(cmdd);

}


void test_parser()
{
    //ִ��������Ҫ��һ��context����
    egg::Context* context = new egg::Context();

    //ע���������
    context->registorConmmand(new CMD_test());

    //���������
    egg::Parser* parser = new egg::Parser();

    std::string cmd;

    //������������
    cmd = "test q0 q1 q2 q3;test q5;";
    egg::CommandDataList cmds0 = parser->pushData(cmd.c_str(), cmd.size());
    for (size_t i = 0; i < cmds0.size(); i++)
    {
        context->runConmmand(cmds0[i]);
    }

    //����ע��
    cmd = "#test x0 x1 x2 x3;\ntest x5;";
    egg::CommandDataList cmds = parser->pushData(cmd.c_str(), cmd.size());
    for (size_t i = 0; i < cmds.size(); i++)
    {
        context->runConmmand(cmds[i]);
    }

    //������������
    std::vector<std::string> cmdlist;
    cmdlist.push_back("test ");
    cmdlist.push_back("ww ");
    cmdlist.push_back("; ");
    for (size_t i = 0; i < cmdlist.size(); i++)
    {
        egg::CommandDataList cmds = parser->pushData(cmdlist[i].c_str(), cmdlist[i].size());

        for (size_t i = 0; i < cmds.size(); i++)
        {
            context->runConmmand(cmds[i]);
        }
    }

}

void test_console()
{
    //���Կ���̨
    egg::Context* context = new egg::Context();
    context->registorConmmand(new CMD_test());
    egg::Parser* parser = new egg::Parser();
    egg::Console* console = new egg::Console();


    //�����ڴ�ű�
    std::string script = {
        "test 111 222 333;"
        "test aaa bbb ccc;"
        " test  AAA  BBB  CCC ; "
    };
    console->runScript(parser, context, script.c_str(), script.size());


    //д������ļ�
    std::string filename = "testCLRScript.txt";
    std::ofstream ofs(filename);
    ofs.write(script.c_str(), script.size());
    ofs.close();

    //�����ļ��ű�
    console->runScriptFile(parser, context, filename.c_str());


}


int main(int argc, char *argv[])
{
    test_cmd();
    test_parser();
    test_console();

}

