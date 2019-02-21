#ifndef NIUBI_SG_RESOURCE_H
#define NIUBI_SG_RESOURCE_H

#include <eggEngine/Export.h>

#include <map>
#include <vector>

//#include <eggBase/Public.h>
//#include <eggRuntime/Object.h>
#include <eggRuntime/Node.h>
#include <eggRuntime/NodeVisitor.h>

namespace egg
{

    /*
    * ��Դ���������
    */

//��ֵ��Դ����
#define RESOURCE_LINK(TYPE) \
public:\
    void set##TYPE( TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE()const{return m##TYPE.valid()?m##TYPE->getResource():nullptr;}\
    ResourceLinkType getLinkType()const{return m##TYPE.valid()?m##TYPE->getLinkType():RLT_Null;}\
private:\
    egg::ref_ptr< ResourceLinkT< TYPE > > m##TYPE;


//std::vector ��Դ����   /*�����Խ��Ĳ���ֻ����Ϊ��׷��*/
#define RESOURCE_LINK_VECTOR(TYPE) \
public:\
    void set##TYPE( unsigned int index , TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        if( index >= m##TYPE.size() ){ m##TYPE.resize(index+1); }\
        m##TYPE[index] = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE( unsigned int index )const\
    {\
        return ( (index<m##TYPE.size()) && (m##TYPE[index].valid()) )?m##TYPE[index]->getResource():nullptr;\
    }\
    void add##TYPE( TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE.push_back( new ResourceLinkT< TYPE >( r , t ) );\
    }\
    void remove##TYPE( unsigned int index )\
    {\
        if( index < m##TYPE.size()) m##TYPE.erase(m##TYPE.begin()+index);\
    }\
    ResourceLinkType getLinkType( unsigned int index )const{return (index<m##TYPE.size()&&m##TYPE[index].valid())?m##TYPE[index]->getLinkType():RLT_Null;}\
    unsigned int getNum##TYPE()const{ return m##TYPE.size(); }\
private:\
    std::vector< egg::ref_ptr< ResourceLinkT< TYPE > > > m##TYPE;


//std::map ��Դ����
#define RESOURCE_LINK_MAP(KEY,TYPE) \
public:\
    void set##TYPE( KEY key , TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE[key] = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE( KEY key ) const\
    {\
        auto it = m##TYPE.find(key);\
        if(it != m##TYPE.end()) return it->second.valid()?it->second->getResource():nullptr;\
        else return nullptr;\
    }\
    ResourceLinkType getLinkType( KEY key ) const\
    {\
        auto it = m##TYPE.find(key);\
        if(it != m##TYPE.end()) return it->second.valid()?it->second->getLinkType():RLT_Null;\
        else return RLT_Null;\
    }\
    unsigned int getNum##TYPE() const { return m##TYPE.size(); }\
    std::vector<KEY> getKeys##TYPE() const\
    {\
        std::vector<KEY> ret;\
        for( auto it = m##TYPE.begin() ; it != m##TYPE.end(); it++)\
        {\
            ret.push_back(it->first);\
        }\
        return ret;\
    }\
private:\
    std::map< KEY , egg::ref_ptr< ResourceLinkT< TYPE > > > m##TYPE;




    class ResourceLink;

    class EGG_ENGINE_EXPORT Resource
        //:public nbBase::Referenced
        :public Object
    {
    public:
        Resource()
        {
            uniform_path.setAbsolutePath(true); //ͳһ·���ڰ����Ǿ���·��
            uniform_path.set("null"); //Ϊ�˱�����Դmap��ʶ����� Ĭ�ϸ���Դһ������
        }
        virtual ~Resource(){}

        //�洢���ļ���׺ �����CLASS����һ��
        //virtual std::string getExt(){return "resource";}
        virtual std::string ObjectClassName() override {return "resource";}

        //�ռ��Ѿ����ص���Դ����
        virtual void collectionResourceObject( std::map< egg::Path , egg::ref_ptr< Resource > >& result){}

        //�ռ���δ���ص���Դ·��
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result){}

        virtual void save(egg::Path filename , std::ostream& os , Serializer* ser ) const override;

        virtual void load(egg::Path filename , std::istream& is , Serializer* ser 
            , ObjectManager* om , egg::Referenced* rmg, bool load_ext) override;

        //��ȡͳһ·��
        egg::Path getUniformPath()const { return uniform_path; }

        //����ͳһ·�� �����β����class��׺�Ͳ�����ȥ
        void setUniformPath(const std::string& path);
        void setUniformPath(egg::Path path);

    protected:

        //����·��  
        egg::Path uniform_path; 
    };


    enum ResourceLinkType
    {
        RLT_Null             //��ǰ��¼Ϊ��Ч
        , RLT_Absolute         //��¼Ϊ����·��
        , RLT_Parent_Relative //��¼Ϊ���������·��
        , RLT_Local_Relative  //��¼Ϊ��ǰĿ¼֮�µ����·��
    };


    //��Դ���ӻ�����
    class ResourceLink
        :public egg::Referenced
    {
    public:
        ResourceLink(){}
        virtual ~ResourceLink(){}

        virtual egg::Path getPath() = 0;
        virtual void setPath( egg::Path p ) = 0;
        //virtual ResourceLinkType getLinkType() const  = 0;
        //virtual void setLinkType( ResourceLinkType t ) = 0;
        virtual void setRes( Resource* r ) = 0;
        virtual bool isLoaded() const  = 0;
    };


    //��Դ���Ӿ�������
    template< class T >
    class ResourceLinkT
        :public ResourceLink
    {
    public:
        ResourceLinkT( T* r = NULL , ResourceLinkType t = RLT_Local_Relative )
        {
            setResource( r , t );
        }
        ResourceLinkT(egg::Path p , ResourceLinkType t = RLT_Local_Relative )
        {
            setPath( p , t );
        }

        bool isLoaded() const override {return res.valid();}

        T* getResource() const { return res.get(); }
        void setResource( T* r = NULL , ResourceLinkType t = RLT_Local_Relative  )
        {
            res = r; type = t; 
            if(r) path = r->getUniformPath();
        }
        
        virtual void setRes( Resource* r ) override
        {
            //setResource( r , type )
            res = dynamic_cast<T*>(r);
            if(r) path = r->getUniformPath();
        }

        egg::Path getPath() override
        {
            if( res.valid() )
            {
                path = res->getUniformPath();
            }
            return path;
        }
        void setPath(egg::Path p ) override
        {
            if( res.valid() )
            {
                res->setUniformPath(p);
            }
            path = p;
        }

        ResourceLinkType getLinkType() const {return type;}
        void setLinkType( ResourceLinkType t ) { type = t; }

    private:
        /*
            ��Ϊ��Դ�������ǻ���ת���������Ĺ�ϵ 
            ���Բ������κ���ֱ�ӷ�����Щ���� ��ȫ����setget������
        */
        egg::ref_ptr< T > res;       //ʵ�����Ӷ���
        ResourceLinkType type;          //��¼����
        egg::Path path;              //·��
    };


    //������Դ���ӱ���
    //   filename ������������ӵ��ļ�·����Ҳ���ǰ������ӵ��ļ���ַ��
    //   resource_link ��Դ�����ڴ����
    template< class T >
    void loadResourceLink(egg::Path filename , ResourceLinkT<T>* resource_link
        , std::istream& is , Serializer* ser )
    {
        //����������
        int t = 0;
        ser->readNumber( is , t);
        resource_link->setLinkType((ResourceLinkType)t);

        //���Ϊ��Ч �ͷ�������Ĳ���
        if( resource_link->getLinkType() == RLT_Null ) return;

        //������·��
        std::string name;
        ser->readString(is,name);

        //������Դ��Թ�ϵ
        filename.pop();
        filename.push(name);

        //����·��
        resource_link->setPath(filename);
    }

    template< class T >
    //void saveResourceLink( const ResourceLink<T>* resource_link , std::ostream& os , Serializer* ser )/* const*/
    void saveResourceLink( const ResourceLinkT<T>* resource_link , egg::Path this_path
        , std::ostream& os , Serializer* ser )
    {
        //д��������
        if( resource_link )
        {
            ser->writeNumber(os,(int)resource_link->getLinkType());
        }
        else
        {
            ser->writeNumber(os,(int)RLT_Null); //д����Ч���� ֱ���˳�
            return;
        }

        //���Ϊ��Ч �ͷ�������Ĳ���
        if( resource_link->getLinkType() == RLT_Null) return;

        //����������Դ��·�� ���res�����ھ���path�ֶ�
        ResourceLinkT<T>* rl = const_cast<ResourceLinkT<T>*>(resource_link); //��ΪgetPath����const ����ǿ��ת��
        egg::Path path = rl->getPath();
        if (resource_link->getLinkType() == egg::RLT_Parent_Relative
            || resource_link->getLinkType() == egg::RLT_Local_Relative )
        {
            path = egg::Path::relative(
                //this->uniform_path.getPathMel(false,true).c_str() 
                this_path.getPathMel(false,true).c_str() 
                , path.getPathMel(false,true).c_str());
        }
        ser->writeString(os,path.getPathMel());
    }


    //�ռ���������Դ
    class nv_collectionResourceObject
        :public NodeVisitor
    {
    public:
        nv_collectionResourceObject(std::map< egg::Path , egg::ref_ptr< Resource > >& r):result(r){}
        virtual ~nv_collectionResourceObject(){}
        virtual void apply(egg::Node& node) override{ traverse(node); }

        std::map< egg::Path , egg::ref_ptr< Resource > >& result;
    };

    //�ռ���������Դ����
    class nv_collectionResourceLink
        :public NodeVisitor
    {
    public:
        nv_collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& r):result(r){}
        virtual ~nv_collectionResourceLink(){}
        virtual void apply(egg::Node& node) override{ traverse(node); }

        std::vector< egg::ref_ptr< ResourceLink > >& result;
    };


}//namespace egg

#endif//NIUBI_SG_RESOURCE_H