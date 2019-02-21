
#ifndef _NSG_CORE_NODE_VISITOR_H_
#define _NSG_CORE_NODE_VISITOR_H_

#include <egg/Node.h>
#include <egg/NodePath.h>

namespace egg
{

    class EGG_RUNTIME_EXPORT NodeVisitor
        :public Object
    {
    public:
        NodeVisitor() :isUP(false){}
        virtual ~NodeVisitor(){}

        //�����ͼ̳нӿ�
        virtual void apply(Node& node)
        {
            traverse(node);
        }

        //Ĭ�ϱ�������
        inline void traverse(Node& node)
        {
            if (isUP)
            {
                node.ascend(*this);
            }
            else
            {
                node.traverse(*this);
            }
        }

        //���Ͳ���
        template<class T>
        T* as()
        {
            T* v = dynamic_cast<T*>(this);
            return v;
        }


        bool isUP; //�������� true���� false����

        NodePath np;
    };

}//namespace nbSG

#endif //_NSG_CORE_NODE_VISITOR_H_
