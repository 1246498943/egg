#ifndef _CLR_CONSOLE_H_
#define _CLR_CONSOLE_H_

#include <egg/Export.h>

#include <vector>
#include <string>
#include <fstream>
#include <map>

#include <egg/Command/Command.h>
#include <egg/Command/Parser.h>
#include <egg/Command/Context.h>


namespace egg
{

    //����̨
    //  Console Parser Context �����ǲ��й�ϵ
    //  Parser ����ͬʱ���ڶ�� ����������ѡ�����ʹ����һ��
    //  Context ��ʾ�������Ե������� Ҳ����ʵ��ִ�����Ե�����
    //      ��������ע��� Command ���� Command ���������κ�״̬��Ϣ
    //      ����������ʷ��¼ ��ʷ��¼���� Command ָ�� ִ�в��� ����ֵ
    //      Command ������������ Parameter Ϊ������Ϣ
    //      Command ������˵����Ϣ
    //  Console ģ��һ������̨���߼�
    //  
    class EGG_RUNTIME_EXPORT Console
    {
    public:
        Console() {}
        ~Console() {}

        //ͨ���̳�ʵ�ֲ�ͬ�������ʽ
        class Output
        {
        public:
            Output() {}
            virtual ~Output() {}

            virtual void info(std::string str) = 0;
            virtual void warning(std::string str) = 0;
            virtual void error(std::string str) = 0;

        private:

        };

        //std::string toString(Parser* parser, Command* command)
        //{
        //    return parser->toString(command);
        //}

        //void runConmmand(Parser* parser, const char* command, int size)
        //{
        //}

        void saveHistroyScriptFile(Parser* parser, Context* context, const char* filename)
        {
            context->exportHistory(parser, filename);
        }
        void saveObjectScriptFile(Parser* parser, Context* context, const char* filename)
        {
            context->exportObject(parser, filename);
        }

        void runScript(Parser* parser, Context* context, const char* script, int size)
        {
            //��������� ������Ч�� CommandDataList
            CommandDataList cdl = parser->pushData(script, size);
            for (size_t i = 0; i < cdl.size(); i++)
            {
                //��context�����нű�
                context->runConmmand(/*context, */cdl[i]);
            }
        }

        void runScriptFile(Parser* parser, Context* context, const char* filename)
        {
            // buffer_size ֻ��һ���ݽ���ȡ�Ĳ������� �����Ƕ��ٶ�û�й�ϵ
            const int buffer_size = 1024;
            //char buffer[buffer_size] = {0};

            std::ifstream ifs(filename, std::ios::binary);
            int length = 0;
            if (ifs.good())
            {
                ifs.seekg(0, ifs.end);
                length = ifs.tellg();
                ifs.seekg(0, ifs.beg);
            }
            while (ifs.good())
            {
                char buffer[buffer_size] = { 0 };

                //�����������ļ�������ѭ��
                //if (ifs.eof()) { break; }

                //��ȡ�ļ���ĩβ
                ifs.read(buffer, buffer_size);
                int read_size = buffer_size;
                if (length < buffer_size)
                {
                    read_size = length;
                }
                else
                {
                    length -= buffer_size;
                }

                //����ʽ��������
                runScript(parser, context, buffer, buffer_size);
            }
            ifs.close();
        }

    };

}//namespace egg

#endif // !_CLR_CONSOLE_H_
