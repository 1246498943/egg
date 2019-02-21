#ifndef NIUBI_SG_TEXTURE_H
#define NIUBI_SG_TEXTURE_H

#include <eggEngine/Export.h>

//#include <eggBase/Public.h>

#include <eggEngine/Resource.h>
#include <eggEngine/DataDefine.h>
#include <eggEngine/Image.h>



/*
* ������Դ
*/
namespace egg
{

    class EGG_ENGINE_EXPORT Texture
        :public Resource
    {
    public:
        Texture() {}
        virtual ~Texture() {}

        //virtual std::string getExt(){return "texture";}
        virtual std::string ObjectClassName() override { return "texture"; }

        virtual void save(egg::Path filename, std::ostream& os, Serializer* ser) const override;

        virtual void load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override;

        //�ռ��Ѿ����ص���Դ����
        virtual void collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result) override;

        //�ռ���δ���ص���Դ·��
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result) override;

        //GL���
        //    �ھ�����Ⱦ��ʱ��������
        RendererResourceSet<handle_texture> mGLHandle;


        RESOURCE_LINK(Image)
    };


}//namespace nbSG

#endif//NIUBI_SG_TEXTURE_H
