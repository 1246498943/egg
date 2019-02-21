
#include <eggEngine/Components/MeshRender.h>
#include <eggEngine/RenderVisitor.h>
#include <eggEngine/RenderNode.h>
#include <eggEngine/SceneNode.h>

using namespace egg;


void MeshRender::traverse(NodeVisitor& nv)
{
    if (nv.as<nv_collectionResourceObject>())
    {
        nv_collectionResourceObject* cro = nv.as<nv_collectionResourceObject>();

        //Geometry
        if (mGeometry->isLoaded())
        {
            cro->result[mGeometry->getPath()] = mGeometry->getResource();
        }

        //Material
        for (auto it = mMaterial.begin(); it != mMaterial.end(); it++)
        {
            if ((*it)->isLoaded())
            {
                Path p = (*it)->getPath();
                if (p.size() > 0)
                {
                    cro->result[p] = (*it)->getResource();
                }
                else
                {
                    printf("Material path is null\n");
                }
            }
        }
    }
    else if (nv.as<nv_collectionResourceLink>())
    {
        nv_collectionResourceLink* crl = nv.as<nv_collectionResourceLink>();

        //Geometry
        if (!mGeometry->isLoaded())
        {
            crl->result.push_back(mGeometry.get());
        }

        //Material
        for (auto it = mMaterial.begin(); it != mMaterial.end(); it++)
        {
            if (!(*it)->isLoaded())
            {
                crl->result.push_back((*it).get());
            }
        }
    }

    ComponentBase::traverse(nv);
}

Material* MeshRender::fixMaterial(unsigned int index)
{
    //��ѯ��Ӧ�Ĳ���  �����Ⱦ���������ڲ����������������һ��
    Material* m = 0;
    if (getNumMaterial() == 0)       //���û�в��� m=0
    {
        //m = mMaterialList[0].get();
    }
    else if (index < getNumMaterial())   //������ȷ��material m=id
    {
        m = getMaterial(index);
    }
    else                                 //���������material m=���һ������
    {
        m = getMaterial(getNumMaterial() - 1);
    }

    //���û���ҵ��κβ��� ��Ⱦ���޷����е� ���Կ�������Ĭ�ϲ���
    //if (m == 0)
    //{
    //    //��ʾ����
    //    printf("no material\n");
    //    return;
    //}
    return m;
}

void MeshRender::onCull(nv_cull* nv, Eigen::Matrix4f& vm)
{
    //���û��mesh����ȫ��Ч��
    if (!mGeometry->isLoaded()) return;

    //nv_cull* cull = nv.as<nv_cull>();
    RenderNode* rendernode = nv->mRenderNode;

    //����ȫ��subgeometry
    unsigned int sg_size = mGeometry->getResource()->getNumSubGeometry();
    for (size_t sg_index = 0; sg_index < sg_size; sg_index++)
    {
        //ȷ����ǰmaterial����
        Material* material = fixMaterial(sg_index);

        //����ȫ��pass
        unsigned int pass_num = material->getTech(material->getCurrentTech())->mPasses.size();
        for (size_t pass_index = 0; pass_index < pass_num; pass_index++)
        {
            //��ǰpass
            Pass* pass = material->getTech(material->getCurrentTech())->mPasses[pass_index].get();

            //��ѯ��ǰRN��RF
            RenderNode* child_rn = rendernode->getOrCreateChild(pass);
            child_rn->setPass(pass);

            //������subgeomtery��ӵ�leaf
            RenderLeaf* rl = child_rn->getOrCreateRenderLeaf();
            rl->setViewMatrix(nv->cameraViewMatrix * vm);
            rl->setProjectionMatrix(nv->projactiveMatrix);

            RenderLeaf::DrawableSet sgs;
            sgs.drawable = mGeometry->getResource();
            sgs.subgeometry_index = sg_index;

            rl->addDrawable(sgs);
        }
    }
}

void MeshRender::onRender(nv_render* nv, Eigen::Matrix4f& mv)
{
#if 0
    //for (int i = 0; i < mMeshList.size(); i++)
    //{
    //    mMeshList[i]->render(nv->render,nv->projactiveMatrix,mv);
    //}

    if (!mGeometry->isLoaded()) return;

    //����subgeometry����Ⱦ������
    unsigned int num_subgeometry = mGeometry->getResource()->getNumSubGeometry();

    //ѭ������geometry��Ⱦ��
    for (size_t i = 0; i < num_subgeometry; i++)
    {
        //��ѯ��Ӧ�Ĳ���  �����Ⱦ���������ڲ����������������һ��
        Material* m = 0;
        if (getNumMaterial() == 0)       //���û�в��� m=0
        {
            //m = mMaterialList[0].get();
        }
        else if (i < getNumMaterial())   //������ȷ��material m=id
        {
            m = getMaterial(i);
        }
        else                                 //���������material m=���һ������
        {
            m = getMaterial(getNumMaterial() - 1);
        }

        //���û���ҵ��κβ��� ��Ⱦ���޷����е� ���Կ�������Ĭ�ϲ���
        if (m == 0)
        {
            //��ʾ����
            printf("no material\n");
            return;
        }

        //ѭ������pass ��Ⱦ��������
        unsigned int pass_num = m->getTech(m->getCurrentTech())->mPasses.size();
        for (size_t ipass = 0; ipass < pass_num; ipass++)
        {
            //���ò���
            Material::Pass* p = m->getTech(m->getCurrentTech())->mPasses[ipass].get();

            //Ԥ�����Զ�����
            //�������Դ���  �Ժ�ý���һ����׼�ӿ�  ��ȷ��ʾ���ԵĴ���
            auto it = mGeometry->getResource()->mVectexAttribute.find(NFX_VectexAttribute_bone_indices);
            if (it != mGeometry->getResource()->mVectexAttribute.end())
            {
                int skinning = 1;
                p->setUniform<int>("skinning", UniformBase::UniformDataType::UDT_INT, &skinning, 1, "");
            }
            else
            {
                int skinning = 0;
                p->setUniform<int>("skinning", UniformBase::UniformDataType::UDT_INT, &skinning, 1, "");
            }

            //bind pass
            p->bind(nv->render, mv, nv->projactiveMatrix);

            //��Ⱦsubgeometry
            mGeometry->getResource()->renderSubGeometry(nv->render, p, i);
        }

    }//for
#endif //0
}

void MeshRender::save(Path filename, std::ostream& os, Serializer* ser) const
{
    ComponentBase::save(filename, os, ser);

    //Geometry
    saveResourceLink<Geometry>(mGeometry.get(), Path(filename), os, ser);

    //Material
    unsigned int tex_num = getNumMaterial();
    ser->writeNumber(os, tex_num);  //Texture����
    for (unsigned int j = 0; j < tex_num; j++)
    {
        saveResourceLink<Material>(mMaterial[j].get(), Path(filename), os, ser);
    }
}

void MeshRender::load(Path filename, std::istream& is
    , Serializer* ser, ObjectManager* om
    , Referenced* rmg, bool load_ext)
{
    ComponentBase::load(filename, is, ser, om, rmg, load_ext);

    //Geometry
    mGeometry = new ResourceLinkT< Geometry >();
    loadResourceLink<Geometry>(filename, mGeometry.get(), is, ser);

    //Material
    unsigned int mtl_num = 0;
    ser->readNumber(is, mtl_num);  //Texture����
    for (unsigned int t = 0; t < mtl_num; t++)
    {
        ResourceLinkT< Material >* material = new ResourceLinkT< Material >();
        loadResourceLink<Material>(filename, material, is, ser);
        mMaterial.push_back(material);
    }
}

