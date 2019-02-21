
#include <eggEngine/ResourceManager.h>

#include <eggEngine/image.h>
#include <eggEngine/Texture.h>
#include <eggEngine/Shader.h>
#include <eggEngine/Program.h>
#include <eggEngine/Material.h>
#include <eggEngine/Geometry.h>
#include <eggEngine/Scene.h>
#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/Components/Joint3D.h>
#include <eggEngine/Components/MeshRender.h>


TEST(eggEngine, ResourceManager)
{


}

//��ֲ������
//�����ڴ潨����Դ������д����ȷ��
void buildNewLibrary()
{

    /*
    ������ֽ���ȫ��������Դ �����д�� ��ȡ ��д�� �Ա����������Ŀ¼����֤save load�Ƿ���ȷ
    */

    //��ʼ����Դ��Դ������
    egg::niubi_entry_arg arg;
    egg::ref_ptr< egg::ResourceManager > rmg = new egg::ResourceManager();
    rmg->init(arg, true);
    egg::ref_ptr< egg::ResourceManager > rmg2 = new egg::ResourceManager();
    rmg2->init(arg, true);

    //ע���
    rmg->registerArchive("", "test_dom_01", true);
    rmg2->registerArchive("", "test_dom_01a", true);

    //
    //��д�ļ�
    //

    std::string txt = "1234567890";
    rmg->writeFile(egg::Path("|test.txt"), &txt[0], txt.size());

    std::vector<char> buffer;
    unsigned int len = 0;
    rmg->readFile(egg::Path("|test.txt"), buffer, len);


    //
    //����Դ��д
    //

    //image
    egg::Image* image = new egg::Image();
    image->setUniformPath("|image01");
    {
        image->alloc(8, 8, 1, egg::PixelFormat::PF_RGBA, egg::DataType::DT_UNSIGNED_BYTE, 8 * 8 * 1 * 4); //��������
        image->getData()[0] = '\x99';
        image->getData()[1] = '\xF9';
    }
    rmg->writeResource(image);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* image2 = rmg->readResource(egg::Path("|image01.image"), false);
    rmg2->writeResource(image2);

    //texture
    egg::Texture* texture = new egg::Texture();
    texture->setUniformPath("|texture01");
    {
        texture->setImage(image, egg::RLT_Local_Relative);   //����image����
    }
    rmg->writeResource(texture);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* texture2 = rmg->readResource(egg::Path("|texture01.texture"), false);
    rmg2->writeResource(texture2);

    //shader
    egg::Shader* shader = new egg::Shader();
    shader->setUniformPath("|shader01");
    {
        shader->setShaderType(egg::Shader::NB_VERTEX_SHADER);
        shader->setString(/*egg::Shader::ShaderProfileType::SPT_GL_CORE_330,*/ "aasdasdasdasdasdasdasd asdasd asd 123123 asdas");
    }
    rmg->writeResource(shader);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* shader2 = rmg->readResource(egg::Path("|shader01.shader"), false);
    rmg2->writeResource(shader2);

    //program
    egg::Program* program = new egg::Program();
    program->setUniformPath("|program01");
    {
        program->setShader(shader->getShaderType(), shader);//������ζ�� shader�����Ͳ����޸���
    }
    rmg->writeResource(program);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* program2 = rmg->readResource(egg::Path("|program01.program"), false);
    rmg2->writeResource(program2);

    //material
    egg::Material* material = new egg::Material();
    material->setUniformPath("|material01");
    {
        egg::Pass* p0 = new egg::Pass();
        material->getTech(material->getCurrentTech())->mPasses.push_back(p0);

        p0->setProgram(program);
        p0->setTexture(0, texture);
        //p0->mAttributes["attr"] = new nbSG::VectexAttribute("attr",0);
        //p0->mUniforms["uuu"] = new nbSG::Uniform<int>("uuu",nbSG::UniformBase::UniformDataType::UDT_INT,998,"");
        //p0->mAttributes.push_back( new nbSG::VectexAttribute("attr",0) );
        //p0->mUniforms.push_back( new nbSG::Uniform<int>("uuu",nbSG::UniformBase::UniformDataType::UDT_INT,998,""));

    }
    rmg->writeResource(material);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* material2 = rmg->readResource(egg::Path("|material01.material"), false);
    rmg2->writeResource(material2);

    //geometry
    egg::Geometry* geometry = new egg::Geometry();
    geometry->setUniformPath("|geometry01");
    {
        //geometry->mBuffers["aaa"] = std::vector<float>();
        //geometry->mBuffers["aaa"].push_back(1.1f);
        //geometry->mBuffers["aaa"].push_back(2.2f);
        //geometry->mBuffers["aaa"].push_back(3.3f);
        //geometry->mBuffers["aaa"].push_back(4.4f);

        //geometry->mSubGeometry.push_back( new nbSG::SubGeometry( 11 , 0 ,12) );
    }
    rmg->writeResource(geometry);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* geometry2 = rmg->readResource(egg::Path("|geometry01.geometry"), false);
    rmg2->writeResource(geometry2);

    //mesh
    //nbSG::Mesh* mesh = new nbSG::Mesh();
    //mesh->uniform_path = "|mesh01";
    //rmg->writeResource(mesh);

    //scene
    egg::Scene* scene = new egg::Scene();
    scene->setUniformPath("|scene01");
    {
        egg::SceneNode* sn1 = new egg::SceneNode("qq1");
        egg::SceneNode* sn2 = new egg::SceneNode("qq2");
        egg::SceneNode* sn3 = new egg::SceneNode("qq3");
        sn1->addChild(sn2);
        sn2->addChild(sn3);
        scene->getRoot()->addChild(sn1);

        egg::Transfrom3D* trans = new egg::Transfrom3D();
        sn3->addComponent(trans);
        egg::Joint3D* joint = new egg::Joint3D();
        sn3->addComponent(joint);

        egg::MeshRender* mr = new egg::MeshRender();
        sn3->addComponent(mr);

        mr->setMaterial(0, material);
        //mr->removeMaterial(0);
        mr->setGeometry(geometry);
    }
    rmg->writeResource(scene);
    rmg->updateUniformAssetPath(); //����asset��

    egg::Resource* scene2 = rmg->readResource(egg::Path("|scene01.scene"), false);
    rmg2->writeResource(scene2);


}


//��ֲ������
//���Գ���ĳ�����Դ��д��ȷ��  ����һ������buildNewLibrary()д�����Դ
void readLibraryAndRewrite()
{
    //
    //����Դ��д
    //

    //��ʼ����Դ��Դ������
    egg::niubi_entry_arg arg;
    egg::ref_ptr< egg::ResourceManager > rmg = new egg::ResourceManager();
    rmg->init(arg, true);
    egg::ref_ptr< egg::ResourceManager > rmg2 = new egg::ResourceManager();
    rmg2->init(arg, true);

    //ע���
    rmg->registerArchive("", "test_dom_01", true);
    rmg2->registerArchive("", "test_dom_02", true);

    //���������
    egg::ResourceGroup rg;
    rmg->readResourceGroup(egg::Path("|scene01.scene"), &rg, false);

    //����д�볡��
    rmg2->writeResourceGroup(&rg);

}


