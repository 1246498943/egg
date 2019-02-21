
#ifndef _CLR_ResultStack_H_
#define _CLR_ResultStack_H_

#include <egg/Export.h>

#include <stack>

#include <egg/Object.h>


namespace egg
{

    //ѡ���ջ
    //�洢���������洢��ʱ����
    class EGG_RUNTIME_EXPORT ResultStack
    {
    public:
        ResultStack() {}
        ~ResultStack() {}

        void setCurrent(Object* object) { mCurrentObject = object; }
        Object* getCurrent() { return mCurrentObject; }

        //��ȡ��ͬͨ����topֵ
        Object* top(int channel = 0)
        {
            return mStack.top();
        }

        //��ջ��ǰֵ
        void push(int channel = 0)
        {
            mStack.push(mCurrentObject);
        }

        //��ջ��ǰֵ
        void pop(int channel = 0)
        {
            mCurrentObject = mStack.top();
            mStack.pop();
        }

        //����
        void clear()
        {
            //for (size_t i = 0; i < mStack.size(); i++)
            {
                while (mStack.size() > 0)
                {
                    mStack.pop();
                }
            }
        }

        void setCurrentAttr(Object* object, std::string name)
        {
            mSelectAttrObject = object;
            mSelectAttrName = name;
        }
        Object* getCurrentAttrObject() { return mSelectAttrObject; }
        std::string getSelectAttrName() { return mSelectAttrName; }

    private:

        //��ǰobject����
        Object* mCurrentObject = 0;

        //object��ջ
        //std::vector< std::stack<CLROBJECT*> > mStack;
        std::stack<Object*> mStack;

        //��ǰѡ������
        Object* mSelectAttrObject = 0;
        std::string mSelectAttrName;

    };

}//namespace egg

#endif // !_CLR_ResultStack_H_

