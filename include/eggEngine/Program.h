#ifndef NIUBI_SG_PROGRAM_H
#define NIUBI_SG_PROGRAM_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/DataDefine.h>
#include <eggEngine/Resource.h>
#include <eggEngine/ResourceManager.h>
#include <eggEngine/Shader.h>
#include <eggEngine/ProgramDefine.h>
#include <eggEngine/XML.h>
#include <eggEngine/Renderer.h>

#include <egg/DataType.h>


#if 0
//attrbute��������
#define NB_ATTRIBUTE_position "nb_position"     //ֻ֧��vec4
#define NB_ATTRIBUTE_normal "nb_normal"         //vec3
#define NB_ATTRIBUTE_color0 "nb_color0"         //vec4
#define NB_ATTRIBUTE_color1 "nb_color1"
#define NB_ATTRIBUTE_color2 "nb_color2"
#define NB_ATTRIBUTE_color3 "nb_color3"
#define NB_ATTRIBUTE_uv0 "nb_uv0"               //vec2
#define NB_ATTRIBUTE_uv1 "nb_uv1"
#define NB_ATTRIBUTE_uv2 "nb_uv2"
#define NB_ATTRIBUTE_uv3 "nb_uv3"
#define NB_ATTRIBUTE_tangent0 "nb_tangent0"     //vec3
#define NB_ATTRIBUTE_tangent1 "nb_tangent1"
#define NB_ATTRIBUTE_tangent2 "nb_tangent2"
#define NB_ATTRIBUTE_tangent3 "nb_tangent3"


//uniform��������
#define NB_UNIFORM_VIEWMATRIX "nb_view_matrix"  //mat4
#define NB_UNIFORM_PROJ "nb_proj_matrix"        //mat4
#define NB_UNIFORM_LIGHT0 "nb_light0"           //vec4
#define NB_UNIFORM_LIGHT1 "nb_light1"
#define NB_UNIFORM_LIGHT2 "nb_light2"
#define NB_UNIFORM_LIGHT3 "nb_light3"
#define NB_UNIFORM_TEXTURE0 "nb_tex0"           //int
#define NB_UNIFORM_TEXTURE1 "nb_tex1"
#define NB_UNIFORM_TEXTURE2 "nb_tex2"
#define NB_UNIFORM_TEXTURE3 "nb_tex3"


//shader���ͣ���Ҫ�ǹ涨����������Ժ�uniform�����͹�ϵ Ŀǰ��û�ж������ ��Ϊ��֧��MRT��
// #define NB_SHADER_TYPE_UNDEFINED "nb_mat_undefined"                 //δ����
#define NB_SHADER_TYPE_NETWORK "nb_mat_network"                     //��������
#define NB_SHADER_TYPE_ONLY_VERTEX "nb_mat_only_vertex"             //pos
#define NB_SHADER_TYPE_NOLIGHT_COLOR "nb_mat_nolight_color"         //pos + color
#define NB_SHADER_TYPE_LIGHT_COLOR "nb_mat_light_color"             //pos + color         //light0
#define NB_SHADER_TYPE_NOLIGHT_TEXTURE "nb_mat_nolight_textrue"     //pos + uv0           //tex0
#define NB_SHADER_TYPE_LIGHT_TEXTURE "nb_mat_light_textrue"         //pos + uv0           //tex0 + light0
#define NB_SHADER_TYPE_LIGHTMAP "nb_mat_lightmap"                   //pos + uv0 + uv1     //tex0 + tex1
#define NB_SHADER_TYPE_NORMALMAP "nb_mat_normalmap"                  //pos + uv0           //tex0 + tex1 + tex2
#define NB_SHADER_TYPE_NORMALMAP_LIGHTMAP "nb_mat_normalmap_lightmap"         //pos + uv0 + uv1     //tex0 + tex1 + tex2 + tex3

#endif

namespace egg
{
    class RenderState;

    //��¼һ��uniform����
    //  ����ʹ��name��Ϊkey�������Ⱦ�ӿ�
    //  ���������Ǹ�DCCT��ƥ���
    class EGG_ENGINE_EXPORT UniformBase
        :public Referenced
    {
    public:

        //uniform��������
        enum UniformDataType
        {
            UDT_VOID = 0 //�������������Ч���� ��Ϊ�������uniform���Բ�����void
            , UDT_BOOL
            , UDT_INT
            , UDT_FLOAT
            , UDT_FLOAT_VEC2
            , UDT_FLOAT_VEC3
            , UDT_FLOAT_VEC4
            , UDT_BOOL_VEC2
            , UDT_BOOL_VEC3
            , UDT_BOOL_VEC4
            , UDT_INT_VEC2
            , UDT_INT_VEC3
            , UDT_INT_VEC4
            , UDT_MAT2
            , UDT_MAT3
            , UDT_MAT4
            , UDT_SAMPLER_2D
            , UDT_SAMPLER_CUBE
            , UDT_COUNT //���һ��
        };

        UniformBase() :mType(UDT_VOID), mCount(0){}
        virtual ~UniformBase(){}

        std::string getName(){ return mName; }
        UniformDataType getType(){ return mType; }
        unsigned int getCount(){ return mCount; }
        std::string getURL(){ return mUrl; }
        std::string getSemantics(){ return maya_semantics_name; }

        virtual std::string toString() = 0;

        virtual std::string getDataTypeName() const = 0;

        virtual void save( egg::Path filename , std::ostream& os , Serializer* ser ) const 
        {
            ser->writeString(os,mName);
            ser->writeNumber(os,mType);
            ser->writeNumber(os,mCount);
            ser->writeString(os,mUrl);
            ser->writeString(os,maya_semantics_name);
        }

        virtual void load(egg::Path filename , std::istream& is , Serializer* ser )
        {
            ser->readString(is,mName);
            ser->readNumber(is,(int&)mType);
            ser->readNumber(is,mCount);
            ser->readString(is,mUrl);
            ser->readString(is,maya_semantics_name);
        }

    protected:
        std::string mName;
        UniformDataType mType;  //�������� �ο�shader�ĵ�
        unsigned int mCount;    //�������� std::vector< T > data; �ĳ���
        std::string mUrl;       //�������ݵ�ַ����ID �����������Ͳ�ͬ����ͬ
        std::string maya_semantics_name; //maya���ݰ����� ���ݺ궨��
    };

    template <class T>
    class /*NIUBI_SG_EXPORT*/ Uniform
        :public UniformBase
    {
    public:

        Uniform(){}
        Uniform(const std::string& name, UniformDataType t, T v , const std::string& semantics){ set(name, t, v , semantics); }                                //����
        Uniform(const std::string& name, UniformDataType t, T x, T y , const std::string& semantics){ set(name, t, x, y , semantics); }                        //������
        Uniform(const std::string& name, UniformDataType t, T x, T y, T z , const std::string& semantics){ set(name, t, x, y, z , semantics); }                //������
        Uniform(const std::string& name, UniformDataType t, T x, T y, T z, T w , const std::string& semantics){ set(name, t, x, y, z, w , semantics); }        //�ķ���
        Uniform(const std::string& name, UniformDataType t, unsigned int count, T* v , const std::string& semantics){ set(name, t, count, v , semantics); }    //�����
        //Uniform(const std::string& name, unsigned int id, const std::string& filename , const std::string& semantics){ set(name, id, filename , semantics); }  //��ͼ����ͨ��

        void get(const std::string& name, T& v , std::string& semantics)
        {
            name = mName;
            v = data[0];
            semantics = this->maya_semantics_name;
        }
        void get(const std::string& name, T& x, T& y , std::string& semantics)
        {
            name = mName;
            x = data[0]; y = data[1];
            semantics = this->maya_semantics_name;
        }
        void get(const std::string& name, T& x, T& y, T& z , std::string& semantics)
        {
            name = mName;
            x = data[0]; y = data[1]; z = data[2];
            semantics = this->maya_semantics_name;
        }
        void get(const std::string& name, T& x, T& y, T& z, T& w , std::string& semantics)
        {
            name = mName;
            x = data[0]; y = data[1]; z = data[2]; w = data[3];
            semantics = this->maya_semantics_name;
        }
        void get(const std::string& name, unsigned int count, T* v , std::string& semantics)
        {
            name = mName;
            for (unsigned int i = 0; i < count; i++)
            {
                v[i] = data[i];
            }
            semantics = this->maya_semantics_name;
        }
        //void get(const std::string& name, unsigned int id, std::string& filename , std::string& semantics)
        //{
        //    id = data[0];
        //    filename = this->mUrl;
        //    semantics = this->maya_semantics_name;
        //}

        void set(const std::string& name, UniformDataType t, T v , const std::string& semantics)
        {
            mName = name;
            mType = t;
            mCount = 1;
            data.resize(1); data[0] = v;
            this->maya_semantics_name = semantics;
        }
        void set(const std::string& name, UniformDataType t, T x, T y , const std::string& semantics)
        {
            mName = name;
            mType = t;
            mCount = 2;
            data.resize(2); data[0] = x; data[1] = y;
            this->maya_semantics_name = semantics;
        }
        void set(const std::string& name, UniformDataType t, T x, T y, T z , const std::string& semantics)
        {
            mName = name;
            mType = t;
            mCount = 3;
            data.resize(3); data[0] = x; data[1] = y; data[2] = z;
            this->maya_semantics_name = semantics;
        }
        void set(const std::string& name, UniformDataType t, T x, T y, T z, T w , const std::string& semantics)
        {
            mName = name;
            mType = t;
            mCount = 4;
            data.resize(4); data[0] = x; data[1] = y; data[2] = z; data[3] = w;
            this->maya_semantics_name = semantics;
        }
        void set(const std::string& name, UniformDataType t, unsigned int count, T* v , const std::string& semantics)
        {
            mName = name;
            mType = t;
            mCount = count;
            data.resize(count);
            for (unsigned int i = 0; i < count; i++)
            {
                data[i] = v[i];
            }
            this->maya_semantics_name = semantics;
        }
        //void set(const std::string& name, unsigned int id, const std::string& filename , const std::string& semantics)
        //{
        //    mType = UDT_SAMPLER_2D; //Ŀǰ�������ֻ�Ǳ������Ǹ���ͼ
        //    mCount = 1;
        //    data.resize(1); data[0] = id;
        //    this->mUrl = filename;
        //    this->maya_semantics_name = semantics;
        //}

        virtual std::string toString() override
        {
            std::ostringstream oss;
            for (unsigned int i = 0; i < data.size(); i++)
            {
                oss << data[i] << " ";
            }
            return oss.str();
        }

        T getData(unsigned int i){ return data[i]; }
        T* getDataPtr(){ return &data[0]; }

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            UniformBase::save(filename ,os,ser);

            //ser->writeString(os,getDataTypeName()); //��������

            unsigned int data_num = data.size();
            ser->writeNumber(os,data_num);      //����
            for (unsigned int i = 0; i < data_num; i++)
            {
                ser->writeNumber(os,data[i]);   //����
            }
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser ) override
        {
            UniformBase::load(filename ,is,ser);

            //std::string type_name;
            //ser->readString(is,type_name); //��������

            unsigned int data_num = 0;
            ser->readNumber(is,data_num);      //����
            data.resize(data_num);
            for (unsigned int i = 0; i < data_num; i++)
            {
                ser->readNumber(is,data[i]);   //����
            }
        }

        std::string getDataTypeName() const
        {
            //if(typeid(T) == typeid(bool)) return "bool";
            //if(typeid(T) == typeid(char)) return "char";
            //if(typeid(T) == typeid(unsigned char)) return "uchar";
            if(typeid(T) == typeid(short)) return "short";
            if(typeid(T) == typeid(unsigned short)) return "ushort";
            if(typeid(T) == typeid(int)) return "int";
            if(typeid(T) == typeid(unsigned int)) return "uint";
            if(typeid(T) == typeid(long)) return "long";
            if(typeid(T) == typeid(unsigned long)) return "ulong";
            if(typeid(T) == typeid(long long)) return "longlong";
            if(typeid(T) == typeid(unsigned long)) return "ulonglong";
            if(typeid(T) == typeid(float)) return "float";
            if(typeid(T) == typeid(double)) return "double";
            if(typeid(T) == typeid(std::string)) return "string";
            return "void";
        }

        //static UniformBase* newUniform( const std::string& type ,const std::string& name, unsigned int id, const std::string& filename , const std::string& semantics)
        //{
        //    if(type == "string") return new Uniform<std::string>(name, id , filename , semantics);
        //    return nullptr;
        //}

    protected:

        std::vector< T > data; //��ֵ�洢
    };

    inline UniformBase* newUniform( const std::string& type 
        //, const std::string& name, UniformBase::UniformDataType t, unsigned int count, void* v , const std::string& semantics
        //, nbBase::Path filename , std::istream& is , Serializer* ser
        )
    {
        //if(type == "bool") return new Uniform<bool>(/*name, t , count, v , semantics*/);
        //if(type == "char") return new Uniform<char>(/*name, t , count, v , semantics*/);
        //if(type == "uchar") return new Uniform<unsigned char>(/*name, t , count, v , semantics*/);
        if(type == "short") return new Uniform<short>(/*name, t , count, v , semantics*/);
        if(type == "ushort") return new Uniform<unsigned short>(/*name, t , count, v , semantics*/);
        if(type == "int") return new Uniform<int>(/*name, t , count, v , semantics*/);
        if(type == "uint") return new Uniform<unsigned int>(/*name, t , count, v , semantics*/);
        if(type == "long") return new Uniform<long>(/*name, t , count, v , semantics*/);
        if(type == "ulong") return new Uniform<unsigned long>(/*name, t , count, v , semantics*/);
        if(type == "longlong") return new Uniform<long long>(/*name, t , count, v , semantics*/);
        if(type == "ulonglong") return new Uniform<unsigned long long>(/*name, t , count, v , semantics*/);
        if(type == "float") return new Uniform<float>(/*name, t , count, v , semantics*/);
        if(type == "double") return new Uniform<double>(/*name, t , count, v , semantics*/);
        return nullptr;
    }

#if 0
    //��������˵��
    //  ����ʹ��name��Ϊkey�������Ⱦ�ӿ�
    //  ���������Ǹ�DCCT��ƥ���
    class NIUBI_SG_EXPORT VectexAttribute
        :public Object
    {
    public:
        //VectexAttribute(unsigned int id = 0, unsigned int count = 0, unsigned int type = 0)
        //    :mID(id), mCount(count), mType(type)
        //{
        //}

        //������������
        //GL_BYTE,GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT,GL_FIXED, or GL_FLOAT(default)
        enum VectexAttributeDataType
        {
            VADT_BYTE
            , VADT_UNSIGNED_BYTE
            , VADT_SHORT
            , VADT_UNSIGNED_SHORT
            //, VADT_FIXED
            , VADT_FLOAT
            , VADT_DOUBLE
            , VADT_COUNT //���һ��
        };

        VectexAttribute(const std::string& name = "", unsigned int index = 0
            , unsigned int noc = 3, VectexAttributeDataType type = VADT_FLOAT ,bool normalized = false
            , unsigned int stride = 0 , const std::string& semantics = "" )
        {
            this->name = name;
            this->index = index;
            this->noc = noc;
            this->type = type;
            this->normalized = normalized;
            this->stride = stride;
            this->maya_semantics_name = semantics;
        }

        virtual ~VectexAttribute()
        {
        }

        //unsigned int mID;     //ʵ��id
        //unsigned int mCount;  //������
        //unsigned int mType;   //��������

        //���� ͨ���� ������ ��������
        std::string name;
        unsigned int index; //ͨ��
        unsigned int noc;    /*Number of components*/ //1 2 3 4(default)
        VectexAttributeDataType type; //GL_BYTE,GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT,GL_FIXED, or GL_FLOAT(default)
        bool normalized;     // defualt false
        unsigned int stride; // 0(default)
        std::string maya_semantics_name; //maya���ݰ����� ���ݺ궨��
    };
#endif 

    /*
    *  Program�Ĳ���ģ�嶨�����
    *     ������Ϊһ�����ݼ��ϱ���Ƚ�����
    *     load����Ҳ���Ա��ıȽϼ���
    */
    class EGG_ENGINE_EXPORT ProgramParameterTemplate
    {
    public:
        void load(/*std::istream& is*/EGG_XML_NODE *element);

        //�궨���б�
        //std::map<std::string, std::string > mDefines;

        //uniform�б�
        std::map<std::string, egg::ref_ptr<UniformBase> > mUniforms;

        //attributeλ�ð󶨹�ϵ
        //std::map<std::string, std::string > mAttributes;

        //texture�б�
        std::map<std::string, std::string > mTextures;

    };



    /*
    * Program����
    */
    class EGG_ENGINE_EXPORT Program
        :public Resource
    {
    public:

        Program(){}
        virtual ~Program(){}
        
        //virtual std::string getExt(){return "program";}
        virtual std::string ObjectClassName() override {return "program";}

#if 1
        //ֻ��Ҫ������Ϣ������ɳ�ʼ��
        void bind(HANDLE_TYPE contextID, Renderer* renderer
            //, vmml::mat4f& mv, vmml::mat4f& proj
            //, std::map<std::string, nbBase::ref_ptr<VectexAttribute> > attributes/*Material* material*/
            //, std::vector< nbBase::ref_ptr<VectexAttribute> > attributes
            );
        void unBind();
#endif

        //�ռ��Ѿ����ص���Դ����
        virtual void collectionResourceObject( std::map< egg::Path , egg::ref_ptr< Resource > >& result ) override
        {
            //Shader
            for (auto it = mShader.begin(); it != mShader.end(); it++)
            {
                if(it->second->isLoaded())
                {
                    egg::Path p = it->second->getPath();
                    if( p.size() > 0 )
                    {
                        result[ p ] = it->second->getResource();
                    }
                    else
                    {
                        printf("Shader path is null\n");
                    }
                }
            }
        }

        //�ռ���δ���ص���Դ·��
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result) override
        {
            //Shader
            for (auto it = mShader.begin(); it != mShader.end(); it++)
            {
                if(!it->second->isLoaded())
                {
                    result.push_back( it->second.get() );
                }
            }
        }

        virtual void save(egg::Path filename , std::ostream& os , Serializer* ser ) const override
        {
            //program�Ǹ�ֻ������ ���ﲻ��Ҫsave����
#if 0
            Resource::save(filename ,os,ser);

            //Shader
            std::vector< Shader::ShaderType > shader_keys = getKeysShader();
            ser->writeNumber( os , (int)shader_keys.size() );
            for (unsigned int i = 0; i < getNumShader(); i++)
            {
                auto it = mShader.find(shader_keys[i]);

                //type
                ser->writeNumber( os , shader_keys[i] );

                //res
                saveResourceLink<Shader>( it->second.get() , this->uniform_path , os , ser );
            }

            //mVANameMapping
            ser->writeNumber( os , (int)mVANameMapping.size() ); //����
            for (auto it = mVANameMapping.begin(); it != mVANameMapping.end(); it ++)
            {
                ser->writeString( os , it->first );   //va����
                ser->writeNumber( os , it->second );  //vaͨ��
            }
#endif
        }

        virtual void load(egg::Path filename , std::istream& is , Serializer* ser , ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override
        {
            //�����xml��ȡ��Ϣ ֱ������Ĭ�ϵ����л�����

            EGG_XML_DOCUMENT dom;

            //ƴװ������
            std::vector<char> buffer;
            buffer = std::vector<char>(
                (std::istreambuf_iterator<char>(is))
                , std::istreambuf_iterator<char>()
                );
            buffer.push_back('\0');

            std::cout << "parse xml begin \n";
            try
            {
                dom.parse<0>(&(buffer[0]));
            }
            catch (...)
            {
                std::cout << "parse xml catch\n";
                return;
            }
            std::cout << "parse xml end \n";

            //�ҵ���һ���Ϸ���library
            EGG_XML_NODE* program = 0;
            for (EGG_XML_NODE *node = dom.first_node(); node; node = node->next_sibling())
            {
                if (strcmp(node->name(), "program") == 0)
                {
                    program = node;
                }
            }
            if (program == 0)return;

            //���������ӽڵ�
            for (EGG_XML_NODE *element = program->first_node(); element; element = element->next_sibling())
            {
                if (compareElementName(element, "template"))
                {
                    mProgramParameterTemplate.load(element);
                }
                if (compareElementName(element, "shader_define"))
                {
                    std::string define_string = getContent(element);
                    if (define_string.size() > 0)
                    {
                        mShaderDefines.push_back(define_string);
                    }
                }
                if (compareElementName(element, "shader_link"))
                {
                    Shader::ShaderType st;
                    std::string type = getElementProperties(element, "type");
                    if (type == "vs") st = Shader::ShaderType::NB_VERTEX_SHADER;
                    if (type == "fs") st = Shader::ShaderType::NB_FRAGMENT_SHADER;

                    std::string path = getElementProperties(element, "path");

                    ResourceLinkT< Shader >* shader_link = new ResourceLinkT< Shader >();
                    
                    //loadResourceLink<Shader>(filename, shader_link, is, ser);
                    
                    //ȡ��loadResourceLink�Ĳ���
                    egg::Path res_filename = filename;
                    res_filename.pop();
                    res_filename.push(path);

                    //ֱ������·��
                    //shader_link->setPath(res_filename);
                    //mShader[st] = shader_link;

                    //ֱ�Ӽ�����Դ
                    ResourceManager* rm = ResourceManager::getInstance();
                    Resource* resource = rm->readResource(res_filename, false);
                    Shader* shader = dynamic_cast<Shader*>(resource);
                    shader->setShaderType(st);
                    this->setShader(st, shader, egg::ResourceLinkType::RLT_Absolute);
                    
                }
                if (compareElementName(element, "attribute"))
                {
                    std::string name = getElementProperties(element, "name");
                    std::string id = getElementProperties(element, "id");
                    int int_id = atoi(id.c_str());

                    mVANameMapping[name] = int_id;
                }

            }

#if 0
            Resource::load(filename ,is,ser,om,rmg,load_ext);

            //Shader
            unsigned int shader_num = 0;
            ser->readNumber( is , shader_num );
            for (unsigned int i = 0; i < shader_num; i++)
            {
                //type
                Shader::ShaderType st;
                ser->readNumber( is , (int&)st );

                //res
                ResourceLinkT< Shader >* shader_link = new ResourceLinkT< Shader >();
                loadResourceLink<Shader>( filename ,shader_link , is , ser );

                //
                mShader[st] = shader_link;
            }


            //mVANameMapping
            int va_count = 0;
            ser->readNumber( is , va_count );  //����
            for (int i = 0; i < va_count; i++)
            {
                std::string va_string;
                ser->readString( is , va_string );  //va����

                unsigned int va_id = 0;
                ser->readNumber( is , va_id );      //vaͨ��

                mVANameMapping[va_string] = va_id;
            }
#endif
        }

        ProgramParameterTemplate mProgramParameterTemplate;

        RendererResourceSet<handle_program>& getProgram(){return mProgram;}

    //private:

        RESOURCE_LINK_MAP(Shader::ShaderType,Shader)

    public:
        //GL���
        //    �ھ�����Ⱦ��ʱ��������
        RendererResourceSet<handle_program> mProgram;
        //bool dirty = true;

    //public:
        //VA�;���ID�İ󶨹�ϵ
        //    �����Ϣ��link��ʱ���Զ�����
        std::map<std::string , unsigned int> mVANameMapping;

        std::vector<std::string > mShaderDefines;
    };
    


}//namespace nbSG

#endif//NIUBI_SG_PROGRAM_H
