#ifndef NIUBI_SG_SHADER_H
#define NIUBI_SG_SHADER_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/DataDefine.h>
#include <eggEngine/Resource.h>
//#include <eggEngine/RenderState.h>
#include <egg/DataType.h>
#include <eggEngine/Renderer.h>

namespace egg
{

    //class Renderer;
    //class GraphicsContext;
    class RenderState;
    class Program;

    /*
    * Shader����
    */
    class EGG_ENGINE_EXPORT Shader
        :public Resource
    {
    public:

        enum ShaderType
        {
            NB_NULL_SHADER
            , NB_VERTEX_SHADER
            , NB_TESSELLATION_CONTROL_SHADER
            , NB_TESSELLATION_EVALUATION_SHADER
            , NB_GEOMETRY_SHADER
            , NB_FRAGMENT_SHADER
            , NB_COMPUTE_SHADER
            , NB_COUNT //���һ��
        };

#if 0
        enum ShaderProfileType
        {
            SPT_BULL
            , SPT_GL_1            //��֧��
            , SPT_GL_2            //֧��    ��������ģʽ
            , SPT_GL_CORE_330     //֧��    GL3.3 #version 330
            , SPT_GLES_1          //��֧��
            , SPT_GLES_2          //֧�� 
            , SPT_GLES_310        //֧��
            , SPT_GLES_320        //֧��
            , SPT_DX_11           //��ֲ�Ա�֤
            , SPT_COUNT           //���һ��
        };
#endif

        Shader():mShaderType(NB_NULL_SHADER){}
        virtual ~Shader(){}
        
        //virtual std::string getExt(){return "shader";}
        virtual std::string ObjectClassName() override {return "shader";}

#if 1
        void bind(HANDLE_TYPE contextID, Renderer* renderer, Program* program);
        void unBind();
#endif

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            //ֱ��д���ַ������ļ�
            os << mString.c_str();

#if 0
            Resource::save(filename ,os,ser);

            //shader����
            ser->writeNumber( os , (int)mShaderType );

            //mString
            ser->writeNumber( os , (unsigned int)mString.size() ); //ͨ����
            for (auto it = mString.begin(); it != mString.end(); it ++)
            {
                ser->writeNumber( os , it->first );  //ShaderProfileType
                ser->writeString( os , it->second ); //shader�ı���Ϣ
            }
#endif
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om
            , Referenced* rmg, bool load_ext) override
        {
            //���ļ���ȡ�ַ���
            std::ostringstream oss;
            oss << is.rdbuf();
            mString = oss.str();
            //if (mString[mString.size() - 1] != '\0')mString.push_back('\0');

#if 0
            Resource::load(filename ,is,ser,om,rmg,load_ext);

            //shader����
            ser->readNumber( is , (int&)mShaderType );

            //mString
            unsigned int shader_count = 0;
            ser->readNumber( is , shader_count ); //ͨ����
            for (unsigned int i = 0; i < shader_count; i++)
            {
                int shader_profile = 0;
                ser->readNumber( is , shader_profile ); //ShaderProfileType

                std::string shader_string;
                ser->readString( is , shader_string );  //shader�ı���Ϣ

                mString[(ShaderProfileType)shader_profile] = shader_string;
            }
#endif
        }

        ShaderType getShaderType(){return mShaderType;}
        void setShaderType(ShaderType st){mShaderType = st;}

#if 0
        std::string getString(ShaderProfileType proflie)
        {
            auto it = mString.find(proflie);
            if( it != mString.end() )
            {
                return it->second;
            }
            return "";
        }
        void setString(ShaderProfileType proflie,std::string str)
        {
            mString[proflie] = str;
        }
#endif

        std::string getString()
        {
            return mString;
        }
        void setString(std::string str)
        {
            mString = str;
        }

    //private:

        ShaderType mShaderType;

        std::string mString;
        //std::map< ShaderProfileType ,std::string > mString;

        RendererResourceSet<handle_shader> mShader;
    };
    
}//namespace nbSG


#endif//NIUBI_SG_SHADER_H
