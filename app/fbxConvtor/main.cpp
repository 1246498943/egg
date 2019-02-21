
#include "Export.h"


int main(int argc, char* argv[])
{
    //�����п���
    bool collapse_mesh = false;                 //����mesh���� ����ǿ��skin mesh����(���ɿ���)
    bool exoprt_sikinning_controler = false;    //�������skin����������
    bool exoprt_animation_controler = false;    //�����������������
    bool exoprt_animation = false;               //���������Ϣ�ļ�
    bool only_animation = false;                 //ֻ���������Ϣ�ļ�

    //Ĭ�������ʽ exe xx.fbx
    if (argc < 2) return 1;
    std::string fbxfilename = argv[1];

    //�жϿ���
    if (argc == 2) //ֻ���ļ��� û�п���
    {
        //Ĭ����������ܱ�������ԭʼ��Ϣ
        collapse_mesh = false;
        exoprt_sikinning_controler = true;
        exoprt_animation_controler = true;
        exoprt_animation = true;
    }
    else if (argc >= 3)
    {
        if (std::string(argv[2]) == "-static")
        {
            //��̬ģ������ ֻ������̬�Ŀ���Ⱦ��Ϣ
            collapse_mesh = true;
            exoprt_sikinning_controler = false;
            exoprt_animation_controler = false;
            exoprt_animation = false;
        }
        if (std::string(argv[2]) == "-skinning")
        {
            //���skin��ɫ
            collapse_mesh = true;  //��ɫ�����ϵĸ���ģ����Ϊû��skin������ ���Կ����������ݲ���
            exoprt_sikinning_controler = true;
            exoprt_animation_controler = true;
            exoprt_animation = false;
        }
        if (std::string(argv[2]) == "-animation")
        {
            //ֻ�������
            exoprt_animation = true;
            only_animation = true;
        }
    }


    //����FBX����
    FbxManager* lSdkManager = FbxManager::Create();
    FbxScene* fbx_scene = importFBXScene(lSdkManager, fbxfilename.c_str());
    if (!fbx_scene)
    {
        lSdkManager->Destroy();
        return 1;
    }

    //getPose( fbx_scene );

    //��ʼ����Դ��Դ������
    egg::niubi_entry_arg arg;
    egg::ref_ptr< egg::ResourceManager > rmg;
    rmg = new egg::ResourceManager();
    rmg->init(arg, true);

    //ע��д���·��
    std::string export_dirname = fbxfilename.substr(0, fbxfilename.size() - 4);
    rmg->registerArchive("", export_dirname, true);

    //����Ĭ����Դ
    //ExportDefaultProgramAndShader(rmg.get());
    //ExportDefaultTextureAndImage( rmg.get() );
    rmg->updateUniformAssetPath(); //����asset��


    //����������Ϣ����ǰĬ�ϵ�stack��
    egg::ref_ptr<egg::Animation> anim = LoadAnimation(fbx_scene);

    //���ֻ���������  �����г��� ֱ��д�붯�������� �Ա�ﵽ�滻������Ŀ��
    if (only_animation)
    {
        //ֱ��д��animation
        rmg->writeResource(anim.get());

        //�����г���
        egg::ResourceGroup rg;
        egg::Resource* res = rmg->readResourceGroup(egg::Path("|scene.scene"), &rg, false);
        if (res)
        {
            egg::Scene* nb_scene = dynamic_cast<egg::Scene*>(res);
            if (nb_scene)
            {
                printf("load scene : %s\n", nb_scene->getUniformPath().getPathMel().c_str());

                egg::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<egg::SkeletonActorControler>();
                if (sac) sac->setAnimation(anim.get());

                egg::ActorControler* ac = nb_scene->getRoot()->getComponent<egg::SkeletonActorControler>();
                if (sac) sac->setAnimation(anim.get());
            }
            rmg->writeResource(nb_scene);
        }

        // Destroy the SDK manager and all the other objects it was handling.
        lSdkManager->Destroy();
        printf("done animation\n");
        return 0;
    }


    //����DOM scene
    egg::Scene* scene = new egg::Scene();
    scene->setUniformPath("|scene");

    //�������ܴ��ڵĹ�������������
    egg::SkeletonActorControler* sac = LoadSkeletonActorControler(fbx_scene);
    if (sac && exoprt_sikinning_controler)
    {
        if (anim.valid() && exoprt_animation) sac->setAnimation(anim.get());
        scene->getRoot()->addComponent(sac);
    }
    else
    {
        //������ڶ�����Ϣ �ͽ����㼶����������
        if (anim.valid() && exoprt_animation_controler)
        {
            egg::ActorControler* ac = new egg::ActorControler();
            if (anim.valid() && exoprt_animation) ac->setAnimation(anim.get());
            scene->getRoot()->addComponent(ac);
        }
    }


    //��Ҫ���Ƶ��ⲿ���� ��Ҫ��image
    //from ���ؾ���·��
    //to   uniform·��
    std::map<std::string/*from*/, std::string/*to*/> copy_image_list;

    //�Ӹ��ڵ㿪ʼ�ݹ����
    printf("export root node\n");
    FbxNode* lRootNode = fbx_scene->GetRootNode();
    if (lRootNode)
    {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)
        {
            egg::SceneNode* child = exportNode(lRootNode->GetChild(i), sac
                , copy_image_list, collapse_mesh, exoprt_sikinning_controler);
            scene->getRoot()->addChild(child);
        }
    }
    printf("export root node done\n");

    //copy images
    printf("copy images\n");
    for (auto it = copy_image_list.begin(); it != copy_image_list.end(); it++)
    {
        printf("copy \"%s\" \"%s\"\n", it->first.c_str(), it->second.c_str());
        std::vector<char> f = readfile(it->first);
        rmg->writeFile(egg::Path(it->second), &f[0], f.size());
    }

    //build resource group
    egg::ResourceGroup rg(scene);
    rg.searchAll();

    //write resource group
    printf("write resource group\n");
    rmg->writeResourceGroup(&rg);

    //destroy fbx sdk manager
    lSdkManager->Destroy();
    printf("done\n");
    return 0;
}
