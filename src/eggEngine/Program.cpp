
#include <eggEngine/Program.h>

//#include <eggEngine/Renderer.h>
//#include <eggEngine/GraphicsContext.h>
#include <eggEngine/RenderState.h>
//#include <eggEngine/Renderer.h>
#include <eggEngine/Pass.h>

using namespace egg;


void ProgramParameterTemplate::load(/*std::istream& is*/EGG_XML_NODE *element)
{
    //std::string element_name = "";

    for (EGG_XML_NODE *child = element->first_node(); child; child = child->next_sibling())
    {

        if (compareElementName(child, "uniform"))
        {
            std::string name = getElementProperties(child, "name");
            std::string type = getElementProperties(child, "type");

        }
        //if (compareElementName(child, "attribute"))
        //{
        //    std::string name = getElementProperties(child, "name");
        //}
        if (compareElementName(child, "texture"))
        {
            std::string name = getElementProperties(child, "name");
        }
    }
}


#if 1
void Program::bind(HANDLE_TYPE contextID, Renderer* renderer
    //, vmml::mat4f& mv, vmml::mat4f& proj
)
{
    //RenderState* rs = renderer->getRenderState();

    if (mProgram.getID(contextID).program_id == 0)
    {
        //�����Դ����
        handle_program p = renderer->createProgram();
        mProgram.setID(contextID, p);

        //����shader
        std::vector< Shader::ShaderType > shader_keys = getKeysShader();
        for (unsigned int i = 0; i < shader_keys.size(); i++)
        {
            getShader(shader_keys[i])->bind(contextID, renderer, this);
        }

        //��������
        //�������������shader link֮ǰ
        for (auto it = mVANameMapping.begin()
            ; it != mVANameMapping.end()
            ; it++)
        {
            //render->setAttribLocation(mProgram, it->first, it->second->index);
            renderer->setAttribLocation(p, it->first , it->second );
        }
    }

    //����Զ�����
    handle_program& p = mProgram.getID(contextID);
    renderer->bindProgram(p);

    //������ص�uniform
    //render->setUniform(g_shader, "texture", 0);
    //render->setUniformMatrix(mProgram, NFX_gl_ModelViewMatrix  , 1, mv  );
    //render->setUniformMatrix(mProgram, NFX_gl_ProjectionMatrix , 1, proj);

}


void Program::unBind()
{

}

#endif
