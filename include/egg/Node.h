
#ifndef _NSG_CORE_NODE_H_
#define _NSG_CORE_NODE_H_

/*
* ����visitor���ģʽ ������ڵ�·���йص���Ʒ���
*       ֧�ֶ�child ��parent
*       ֧��node path
*       ֧��node�Զ����Լ��ı������̷������ʽ traverse
*
*
*             Node                     visitor
*   push path  |                          |
*   accept->   |                          |
*              |      apply(node type)    |
*              |      ----------->        | ---|
*              |                          | <--| traverse ��������ѡ�������½�
*              |      <-------------      |
*              |       traverse ascend    |
*   pop path   |        ���������Ϊ       |
*
*   ������OSG�е�ģʽ ��ΪOSG���ǵ�node�ǲ��ϱ��̳е� ����apply��traverse �ǿ������ص�
*   �����������������Node��û�м̳���� ��������������has��ϵ ������visitorֻ��Ҫһ��apply�ӿ�ȥ֪ͨ��Ӧ��capability
*/

//#include <vector>
#include <string>
#include <set>

#include <egg/Object.h>

namespace egg
{

    class NodeVisitor;

    class EGG_RUNTIME_EXPORT Node
        :public Object
    {
    public:
        Node(const std::string& name = "node")
            :Object(name){ /*mName = name;*/ }
        virtual ~Node(){}

        virtual std::string ObjectClassName(){return "node";}

        //void setName(const std::string& name){ mName = name; }
        //std::string getName(){ return mName; }

        virtual void addChild(Node* child)
        {
            mChildern.insert(child);
            child->addParent(this);
        }
        void removeChild(Node* child)
        {
            auto it = mChildern.find(child);
            if (it != mChildern.end())
            {
                (*it)->removeParent(this);
                mChildern.erase(it);
            }
        }
        Node* getChild(unsigned int i)
        {
            unsigned int t = 0;
            for (auto it = mChildern.begin(); it != mChildern.end(); ++it)
            {
                if (t == i) { return (*it).get(); }
                t++;
            }
            return 0;
        }
        unsigned int getNumChild() const { return static_cast<unsigned int>(mChildern.size()); }

        Node* getParent(unsigned int i)
        {
            unsigned int t = 0;
            for (auto it = mParents.begin(); it != mParents.end(); ++it)
            {
                if (t == i) { return (*it).get().get(); }
                t++;
            }
            return 0;
        }
        unsigned int getNumParents() const { return static_cast<unsigned int>(mParents.size()); }


        void accept(NodeVisitor& nv);   //���Լ���ʼ
        void ascend(NodeVisitor& nv);   //����
        virtual void traverse(NodeVisitor& nv); //����


        //���Ͳ���
        template<class T>
        T* as()
        {
            T* v = dynamic_cast<T*>(this);
            return v;
        }

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            Object::save(filename,os,ser);

            //ser->writeString(os,mName); //����

            ser->writeNumber( os ,(unsigned int)mChildern.size() ); //����
            for (auto it = mChildern.begin(); it != mChildern.end(); it++)
            {
                ser->writeString( os , (*it)->ObjectClassName()); //����

                (*it)->save( filename , os , ser );  //�ݹ��ӽڵ�
            }
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om 
            , Referenced* rmg, bool load_ext) override
        {
            Object::load(filename,is,ser,om,rmg,load_ext);

            //ser->readString(is,mName); //����

            unsigned int count = 0;
            ser->readNumber(is,count);  //����

            for (unsigned int i = 0; i < count; i++)
            {
                std::string classname;
                ser->readString( is , classname); //����

                Object* obj = om->newObject(classname);
                Node* node = dynamic_cast<Node*>(obj);
                node->load(filename,is,ser,om,rmg,load_ext);   //���ӽڵ�

                mChildern.insert(node);
            }
        }

    private:

        void addParent(Node* parent)
        {
            observer_ptr<Node> wp(parent);
            mParents.insert(wp);
        }
        void removeParent(Node* parent)
        {
            auto it = mParents.find(parent);
            if (it != mParents.end())
            {
                mParents.erase(it);
            }
        }

        //std::string mName;
        std::set< observer_ptr<Node> > mParents;
        std::set< ref_ptr<Node> > mChildern;
        //std::vector< Capability* > mCapability;

    };

}//namespace nbSG

#endif //_NSG_CORE_NODE_H_
