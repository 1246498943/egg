
#include <iostream>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Texture.h>
#include <eggEngine/ResourceManager.h>
#include <eggEngine/Renderer.h>

using namespace fbxsdk;

//����Ĭ�ϵ� color normal specular
void ExportDefaultTextureAndImage(egg::ResourceManager* rmg)
{
    //default_color_image RGBA=1,1,1,1
    egg::ref_ptr<egg::Image> default_color_image = new egg::Image();
    default_color_image->setUniformPath("|shared|default_color_image");
    {
        default_color_image->alloc(8, 8, 1, egg::PixelFormat::PF_RGBA, egg::DataType::DT_UNSIGNED_BYTE, 8 * 8 * 1 * 4);
    }
    rmg->writeResource(default_color_image.get());

    //default_normal_image RGBA=0.5,0.5,1,1
    egg::ref_ptr<egg::Image> default_normal_image = new egg::Image();
    default_normal_image->setUniformPath("|shared|default_normal_image");
    {
        default_normal_image->alloc(8, 8, 1, egg::PixelFormat::PF_RGBA, egg::DataType::DT_UNSIGNED_BYTE, 8 * 8 * 1 * 4);
    }
    rmg->writeResource(default_normal_image.get());

    //default_specular_image RGBA=1,1,1,1
    egg::ref_ptr<egg::Image> default_specular_image = new egg::Image();
    default_specular_image->setUniformPath("|shared|default_specular_image");
    {
        default_specular_image->alloc(8, 8, 1, egg::PixelFormat::PF_RGBA, egg::DataType::DT_UNSIGNED_BYTE, 8 * 8 * 1 * 4);
    }
    rmg->writeResource(default_specular_image.get());


    //default_color_texture
    egg::ref_ptr<egg::Texture> default_color_texture = new egg::Texture();
    default_color_texture->setUniformPath("|shared|default_color_texture");
    {
        default_color_texture->setImage(default_color_image.get(), egg::RLT_Local_Relative);   //����image����
    }
    rmg->writeResource(default_color_texture.get());

    //default_normal_texture
    egg::ref_ptr<egg::Texture> default_normal_texture = new egg::Texture();
    default_normal_texture->setUniformPath("|shared|default_normal_texture");
    {
        default_normal_texture->setImage(default_normal_image.get(), egg::RLT_Local_Relative);   //����image����
    }
    rmg->writeResource(default_normal_texture.get());

    //default_specular_texture
    egg::ref_ptr<egg::Texture> default_specular_texture = new egg::Texture();
    default_specular_texture->setUniformPath("|shared|default_specular_texture");
    {
        default_specular_texture->setImage(default_specular_image.get(), egg::RLT_Local_Relative);   //����image����
    }
    rmg->writeResource(default_specular_texture.get());

}


////�Ӳ��ʲ�ѯTexture����
//void LoadMaterialTexture( FbxSurfaceMaterial* pSurfaceMaterial , std::vector<FbxTexture*>& result )  
//{
//    int textureLayerIndex;  
//    FbxProperty pProperty;  
//    //int texID;  
//    //MaterialTextureDesc::MtlTexTypeEnum texType;  
//
//    for(textureLayerIndex = 0 ; textureLayerIndex < FbxLayerElement::sTypeTextureCount ; ++textureLayerIndex)  
//    {  
//        pProperty = pSurfaceMaterial->FindProperty( FbxLayerElement::sTextureChannelNames[textureLayerIndex]);  
//        if(pProperty.IsValid())  
//        {  
//            int textureCount = pProperty.GetSrcObjectCount( FbxTexture::ClassId );  
//            for(int j = 0 ; j < textureCount ; ++j)  
//            {  
//                FbxTexture* pTexture = FbxCast<FbxTexture>(pProperty.GetSrcObject(FbxTexture::ClassId,j));  
//                if(pTexture)  
//                {  
//                    // Use pTexture to load the attribute of current texture...  
//                    //printf("Texture\n");
//                    result.push_back(pTexture);
//                }
//            }  
//        }  
//    }  
//}


//void ExportTextureAndImage( nbSG::ResourceManager* rmg , std::vector<FbxTexture*>& result )
//{
//    //����ȫ�������ռ�ȫ����ͼ
//}  
//


#if 0
//
void LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial)
{
    //int textureLayerIndex;  
    //FbxProperty pProperty;  
    //int texID;  
    //MaterialTextureDesc::MtlTexTypeEnum texType;  

    for (int textureLayerIndex = 0; textureLayerIndex < FbxLayerElement::sTypeTextureCount; ++textureLayerIndex)
    {
        FbxProperty pProperty = pSurfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureLayerIndex]);
        if (pProperty.IsValid())
        {
            int textureCount = pProperty.GetSrcObjectCount(FbxTexture::ClassId);
            for (int j = 0; j < textureCount; ++j)
            {
                FbxTexture* pTexture = FbxCast<FbxTexture>(pProperty.GetSrcObject(FbxTexture::ClassId, j));
                if (pTexture)
                {
                    // Use pTexture to load the attribute of current texture...  
                    printf("Texture %d:%s: %s\n", textureLayerIndex, pProperty.GetName(), pTexture->GetName());
                    //result.push_back(pTexture);
                }
            }
        }
    }
}
#endif


egg::Texture* LoadMaterialTexture(FbxProperty* pProperty, std::map<std::string, std::string>& copy_image_list)
{
    egg::Texture* tex = 0;

    if (pProperty)
    {
        //��ѯ��ͼ����
        int textureCount = pProperty->GetSrcObjectCount(FbxTexture::ClassId);

        //Ŀǰֻ�����һ���ҵ�����ͼ
        for (int j = 0; j < textureCount; ++j)
        {
            FbxTexture* pTexture = FbxCast<FbxTexture>(pProperty->GetSrcObject(FbxTexture::ClassId, j));
            if (pTexture)
            {
                printf("Texture %s: %s\n", pProperty->GetName().Buffer(), pTexture->GetName());

                //�����ͼ����
                tex = new egg::Texture();
                tex->setUniformPath(pTexture->GetName());

                //�����file��ͼ
                FbxFileTexture* file_tex = FbxCast<FbxFileTexture>(pTexture);
                if (file_tex)
                {
                    //����image����
                    egg::Image* image = new egg::Image();
                    tex->setImage(image);

                    //·������
                    std::string filename = file_tex->GetFileName(); //����·��
                    egg::Path p(filename);
                    std::string name = p.getLastName();

                    //���ö���
                    image->setUniformPath(name);
                    image->setExternalPath(name);

                    //�����ⲿ�ļ�
                    //  �� filename ����·��
                    //  �� name     uniform·��
                    //printf("copy %s %s\n" ,filename.c_str() , name.c_str() );
                    copy_image_list[egg::Path(filename).getNativePath()] = name;
                }

                //ֻ���ص�һ��
                return tex;
            }
        }
    }

    return tex;
}

