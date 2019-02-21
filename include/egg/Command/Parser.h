#ifndef _CLR_Parser_H_
#define _CLR_Parser_H_

#include <egg/Export.h>

//#include <fstream>
//#include <string>
//#include <vector>
//#include <map>
//
//#include <egg/Reflection/ValueHandle.h>
//#include <egg/Reflection/ObjectHandle.h>

#include <egg/Command/Command.h>

namespace egg
{
   

    //����������ַ�������
    //  ����֧��һ��Ĭ�ϵ��ı���¼��ʽ �Ժ�ͨ���̳�����չ�ŵķ�ʽ
    class EGG_RUNTIME_EXPORT Parser
    {
    public:
        Parser() {}
        ~Parser() {}

        /*
        *   �ļ���ʽ
        *   cmd p0 p1 p2 p3 p4 p5 "a b c d";
        *
        *   �ո�Ϊ�ָ��� ����""�ַ�����
        *   �ֺ�Ϊ�߼��н���
        *   # Ϊ������ע��
        *
        *   cmd Ϊ��������
        *   p0-5 ���������͵Ĳ���
        *   ���� "" �������еĿո�
        *
        */

        virtual CommandDataList pushData(const char* data, int size)
        {
            CommandDataList ret;

            //��¼��������
            mBuffer.insert(mBuffer.end(), data, data + size);

            //���շֺŲ��
            std::vector < std::string > strlist;
            std::string str;
            int endindex = -1;
            for (size_t i = 0; i < mBuffer.size(); i++)
            {
                char ch = mBuffer[i];
                if (ch == '#')
                {
                    //ע��ֱ�ӵ���ĩβ
                    while ((i < mBuffer.size()) && (mBuffer[i] != '\n'))
                    {
                        i++;
                    }
                    continue;
                }
                else if (ch == ';')
                {
                    if (str.size() > 0)strlist.push_back(str);
                    str.clear();
                    endindex = i;
                }
                else
                {
                    str.push_back(ch);
                    endindex = i;
                }
            }

            //ɾ����Ч����
            std::vector<char> mBuffer2;
            int copysize = mBuffer.size() - 1 - endindex;
            if (copysize > 0)
            {
                mBuffer2.resize(copysize);
                memcpy(&mBuffer2[0], &mBuffer[0], copysize);
                mBuffer = mBuffer2;
            }
            else
            {
                mBuffer.clear();
            }

            //�����ű�
            //CommandDataList cmds;
            bool string_state = false;
            ret.resize(strlist.size());
            for (size_t i = 0; i < strlist.size(); i++)
            {
                //cmds[i].resize(strlist[i].size());
                std::string str;
                for (size_t j = 0; j < strlist[i].size(); j++)
                {
                    char ch = strlist[i][j];
                    //if (ch == '#')
                    //{
                    //    //ע��ֱ�ӵ���ĩβ
                    //    while ((strlist[i][j] != '\n') && (j < strlist[i].size()))
                    //    {
                    //        j++;
                    //    }
                    //    continue;
                    //}
                    //��ת�ַ���״̬
                    if (ch == '\"')
                    {
                        string_state = !string_state;
                    }
                    //��������ַ����ڲ�
                    if (!string_state)
                    {
                        if (ch == ' ')
                        {
                            if (str.size() > 0)ret[i].push_back(str);
                            str.clear();
                        }
                        else
                        {
                            str.push_back(ch);
                        }
                    }
                    else
                    {
                        str.push_back(ch);
                    }
                }//for

                //׷�����һ���ַ���
                if (str.size() > 0)
                {
                    if (str.size() > 0)ret[i].push_back(str);
                    str.clear();
                }
            }//for

            return ret;
        }

        virtual std::string toData(Command* command, ParameterList pl)
        {
            std::string ret;
            ret += command->getName();
            for (size_t i = 0; i < pl.size(); i++)
            {
                ret += " ";
                ret += pl[i];
                //if (i + 1 < pl.size())ret += " ";
            }
            if (ret.size() > 0)ret += ";\n";
            return ret;
        }

        virtual std::string toData(CommandDataList cdl)
        {
            std::string ret;
            for (size_t i = 0; i < cdl.size(); i++)
            {
                for (size_t j = 0; j < cdl[i].size(); j++)
                {
                    ret += cdl[i][j];
                    if (j + 1 < cdl[i].size())ret += " ";
                }
                if (cdl[i].size() > 0)ret += ";\n";
            }
            return ret;
        }

        void clear()
        {
            mBuffer.clear();
        }

    private:

        std::vector<char> mBuffer;
    };


}//namespace egg

#endif // !_CLR_Parser_H_
