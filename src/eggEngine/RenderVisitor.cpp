
#include <eggEngine/RenderVisitor.h>

#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/SceneNode.h>

using namespace egg;


void nv_update::apply(Node& node)
{
    //���ﵥ������Transfrom3D���� �Ժ����޸ĵ� ������View����һ������

    //���㵱ǰ����
    Eigen::Matrix4f saveCurrentViewMatrix = this->currentViewMatrix;
    SceneNode* sn = node.as<SceneNode>();
    Transfrom3D* t = 0;
    if (sn)
    {
        t = sn->getComponent<Transfrom3D>();
        if (t)
        {
            t->setWorldViewMatrix(this->currentViewMatrix * t->getLocalMatrix());
            this->currentViewMatrix = t->getWorldViewMatrix();
        }
    }

    //�����ӽڵ�
    traverse(node);

    //��ԭ����
    if (t)
    {
        Transfrom3D* t = node.as<Transfrom3D>();
        this->currentViewMatrix = saveCurrentViewMatrix;
    }
}
