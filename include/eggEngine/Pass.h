#ifndef NIUBI_PASSS_H
#define NIUBI_PASSS_H

#include <eggEngine/Export.h>

#include <eggEngine/Program.h>
#include <eggEngine/Texture.h>

namespace egg
{

    /*
    *  ���ʸ�����Ϣ����
    *       ��Ҫ��Ϊ�˴�DCCT�������չ������
    *           ��չ��Ⱦ״̬       ���� alpha test �ڲ�ͬ����Ⱦʵ������Ͳ�ͬ
    *           ��չuniform��ֵ    ���� alpha refֵ ��˵��������ǿ Ӧ��Ҳû��ʲô���Ե�uniform��Ҫ���������ã�
    *           �������̷ּ�����   ���� skinning ���������ּ�shader�õ�  normalmap��ת���� �Ǹ����������ת����ͼ�õ�
    *  ע�� ��max�е�ö���Ǵ�1��ʼ��¼��
    */
    class EGG_ENGINE_EXPORT CustomMaterialProperty
    {
    public:
        CustomMaterialProperty()
            /*Ĭ��ֵ��ԭ���Ǽ�ʹ������Ҳ����������Ⱦ*/
            :_alpha_test(false), _alpha_ref(0.5), _alpha_mode(7 - 1)
            , _blend(false), _blend_src(5 - 1), _blend_dst(6 - 1)
            , _depth_test(true), _depth_mode(4 - 1), _depth_write(true), _depth_near(0.0), _depth_far(1.0)
            , _reflect_type(1 - 1), _cubemap_z_flip(false), _normalmap_x_flip(false), _normalmap_y_flip(false)
            , _skin(false)
            , _dither(false)
            , _sample_alpha_to_coverage(false)
            , _sample_coverage(false)
            , _wire(false)
            , _double_sided(false)
        {
        }

        bool _alpha_test;   //��Ⱦ״̬
        float _alpha_ref;
        int _alpha_mode;

        bool _blend;        //��Ⱦ״̬
        int _blend_src;
        int _blend_dst;

        bool _depth_test;   //��Ⱦ״̬
        int _depth_mode;
        bool _depth_write;  //��Ⱦ״̬
        float _depth_near;
        float _depth_far;

        int _reflect_type;      //��ʾ������ͼ������
        bool _cubemap_z_flip;   //��ʾcubemap��z����    cubemap ֻ��z��������ģʽ
        bool _normalmap_x_flip; //��ʾnormalmap��x����  �Ҳ�z�᲻���ܷ�ת ����ֻ���������߿ռ�
        bool _normalmap_y_flip; //��ʾnormalmap��y����

        bool _skin;                     //��ʾʹ��skinning��shader
        bool _dither;                   //��Ⱦ״̬
        bool _sample_alpha_to_coverage; //��Ⱦ״̬
        bool _sample_coverage;          //��Ⱦ״̬
        bool _wire;                     //��ʾʹ���߿�ʽ��Ⱦ
        bool _double_sided;             //��ʾ�ر�cull face


        void save( /*nbBase::Path filename ,*/ std::ostream& os, Serializer* ser) const;

        void load( /*nbBase::Path filename ,*/ std::istream& is, Serializer* ser
            //, ObjectManager* om, nbBase::Referenced* rmg, bool load_ext
        );
    };


    //Pass����
    class EGG_ENGINE_EXPORT Pass
        :public Object
    {
    public:
        friend class Material;
        friend class MeshRender;

        Pass()
        {
            //mProgram = new Program();
        }
        virtual ~Pass() {}

        void changeProgramSetting(Program* program);
        void changeDefaultProgramSetting();

#if 0
        void bind(nbPlugins::Renderer* render, vmml::mat4f& mv, vmml::mat4f& proj);
        void unBind();
#endif

        //ͨ��uniform����
        bool hasUniform(const std::string& name);
        void delUniform(const std::string& name);
        unsigned int getUniformNum();
        std::vector< std::string > getUniformNames();

        //uniform��Ϣ��ѯ
        void getUniformInfo(const std::string& name, UniformBase::UniformDataType& type, unsigned int& count, std::string& semantics);

        //��ֵ����uniform����
        template <class T>
        bool setUniform(const std::string& name, UniformBase::UniformDataType t, T* v, unsigned int count, const std::string& semantics)
        {
            mUniforms[name] = new Uniform<T>(name, t, count, v, semantics);
            return true;
        }
        template <class T>
        bool getUniform(const std::string& name, T* v, unsigned int& count, std::string& semantics)
        {
            if (!hasUniform(name)) return false;
            UniformBase::UniformDataType type;
            UniformBase* ub = mUniforms[name].get();
            Uniform<T>* u = dynamic_cast<Uniform<T>*>(ub);
            type = u->getType();
            count = u->getCount();
            u->get(name, count, v, semantics);
            return true;
        }

        //textureͨ�����Ͳ��� sampler ǿ����intȡ�� ��Ҫ��Ϊ�˼����Ժ�ֱ��ת��
        void setTextureUnit(const std::string& name, int int_id);
        unsigned int getTextureUnit(const std::string& name, int& int_id);

        void setCustomMaterialProperty(CustomMaterialProperty cmp) { mCustomMaterialProperty = cmp; }
        CustomMaterialProperty getCustomMaterialProperty() { return mCustomMaterialProperty; }

        std::map<std::string, egg::ref_ptr<UniformBase> > getUniforms() { return mUniforms; }

        int getRenderOrder() { return mRenderOrder; }
        void setRenderOrder(int order) { mRenderOrder = order; }

    private:

        int mRenderOrder = 0;

        //���Ӷ�������
        CustomMaterialProperty mCustomMaterialProperty;

        //attribute��Ӧ��ϵ (����ֻ��¼��������Ƶ�����  VA���Ƽ�¼��program��)
        //std::map<std::string, nbBase::ref_ptr<VectexAttribute> > mAttributes;
        //std::vector< nbBase::ref_ptr<VectexAttribute> > mAttributes;
        std::map<std::string/*attribute*/, std::string/*semantics*/ > mAttributeSemantics;

        //uniform����
        std::map<std::string, egg::ref_ptr<UniformBase> > mUniforms;
        //std::vector< nbBase::ref_ptr<UniformBase> > mUniforms; //�����޸ĳ�set get del�Ľӿ� map�� 

        //shader����
        //nbBase::ref_ptr< Program > mProgram; //vs fs ...
        RESOURCE_LINK(Program);

        //texture
        RESOURCE_LINK_VECTOR(Texture);

    };

}//namespace egg

#endif //NIUBI_PASSS_H

