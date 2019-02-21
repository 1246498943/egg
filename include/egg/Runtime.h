#ifndef _EGG_RUNTIME_H_
#define _EGG_RUNTIME_H_

#include <egg/Export.h>

#include <map>
#include <string>

#include <egg/Singleton.h>
#include <egg/Module.h>
//#include <eggRuntime/ref_ptr.h>


namespace egg
{

    class EGG_RUNTIME_EXPORT Runtime
        :public Object
    {
    public:
        Runtime();
        ~Runtime();

        DECLARE_SINGLETON(Runtime)

        /*
        * �������ģ��
        * ������name�ظ���ģ�顣
        * postfix��Ϊ�ظ�ģ�����ʾ��Ϣ��������Ȼ���ظ���
        */
        Module* requestModule(std::string name, std::string postfix);

        /*
        * ��ȡģ��
        * һ��nameֻ���ܴ���һ�������ص�ģ�顣
        */
        Module* getModule(std::string name);

        /*
        * �������Ͳ�ѯģ��
        * һ������ֻ���ܱ�����һ�Ρ�
        */
        template<class T>
        Module* getModule()
        {
            return 0;
        }

    private:

        std::map<std::string, egg::ref_ptr<Module> > mModuleMap;

        Module* loadLibrary(std::string name, std::string postfix);

    };

}//namespace egg

#endif // !_EGG_RUNTIME_H_
