#ifndef _EGG_Component_IconMesh_H_
#define _EGG_Component_IconMesh_H_

#include <eggEngine/Component.h>


namespace egg
{


    //��򵥵�ͼ����ƶ���
    //    ֻ�ܻ�����ɫ�ʵ��߶�
    class EGG_ENGINE_EXPORT IconMesh
    {
    public:
        IconMesh()
            //:m_geo(0)
        {}
        ~IconMesh() {}

        void build(std::vector<float> position_v4, std::vector<float> color_v4)
        {
            //������
            if (
                position_v4.size() % 4 != 0 //�������4�ı��� ��ʵ�Ǵ����
                || position_v4.size() != color_v4.size()  //pos��ɫ�ʵĸ�����һ��
                )
            {
                return;
            }

            m_buffer.resize(position_v4.size() * 2);//POS��ɫ�ʶ���v4

                                                    //ƴװbuffer
            for (size_t i = 0; i < position_v4.size() / 4; i++)
            {
                m_buffer[i * 8 + 0] = position_v4[i * 4 + 0];
                m_buffer[i * 8 + 1] = position_v4[i * 4 + 1];
                m_buffer[i * 8 + 2] = position_v4[i * 4 + 2];
                m_buffer[i * 8 + 3] = position_v4[i * 4 + 3];
                m_buffer[i * 8 + 4] = color_v4[i * 4 + 0];
                m_buffer[i * 8 + 5] = color_v4[i * 4 + 1];
                m_buffer[i * 8 + 6] = color_v4[i * 4 + 2];
                m_buffer[i * 8 + 7] = color_v4[i * 4 + 3];
            }

            //����VBO
            //m_fvf = Renderer::FVF_MASK(4, 0, false, true, 0); //ֻ����pos��ɫ��

        }
        void render(Renderer* render, Eigen::Matrix4f& projactiveMatrix, Eigen::Matrix4f& viewMatrix)
        {
#if 0
            if (m_geo == 0 && m_buffer.size() > 0)
            {
                m_geo = render->createGeometry(nbPlugins::Renderer::RAW, m_buffer.size() * 4, (char*)&m_buffer[0]);
            }

            //������ǲ����ھͲ���Ⱦ��
            if (m_geo != 0)
            {
                if (m_program.id == 0)
                {
                    //����shader
                    const char vs_string[] =
                    {
                        "attribute vec4 position;\n"
                        "attribute vec4 color; \n"
                        "uniform mat4 modelView; \n"
                        "uniform mat4 projection; \n"
                        "varying vec4 v_color; \n"
                        "void main()\n"
                        "{\n"
                        "    vec4 p = modelView * position; \n"
                        "    gl_Position = projection * p; \n"
                        "    v_color = color; \n"
                        "}\n"
                        "\n"
                        "\0"
                    };
                    const char fs_string[] =
                    {
                        "precision mediump float;\n"
                        "varying vec4 v_color;\n"
                        "void main()\n"
                        "{\n"
                        "    gl_FragColor = v_color;\n"
                        "}\n"
                        "\n"
                        "\0"
                    };

                    m_program = render->createProgram();
                    Renderer::handle_shader vs_shader = render->createShader(m_program, nbPlugins::Renderer::ST_VS, &vs_string[0]);
                    Renderer::handle_shader fs_shader = render->createShader(m_program, nbPlugins::Renderer::ST_FS, &fs_string[0]);
                    render->attachShader(m_program, Renderer::ST_VS, vs_shader);
                    render->attachShader(m_program, Renderer::ST_FS, fs_shader);

                    //�������������shader link֮ǰ
                    render->setAttribLocation(m_program, "position", 0);
                    render->setAttribLocation(m_program, "color", 2);

                }//if (m_program.id == 0)

                render->bindProgram(m_program);

                render->setUniformMatrix(m_program, "projection", 1, projactiveMatrix);
                render->setUniformMatrix(m_program, "modelView", 1, viewMatrix);

                //����mesh
                render->bindGeometry(m_geo, nbPlugins::Renderer::RAW, m_buffer.size() / 8, m_fvf);
                render->drawGeometryArray(nbPlugins::Renderer::LINES, 0, m_buffer.size() / 8);
                render->unbindGeometry(m_geo, nbPlugins::Renderer::RAW, m_fvf);

                render->unbindProgram();
            }
#endif
        }

        std::vector<float> m_buffer;

        //Renderer::handle_geometry m_geo;
        //Renderer::FVF_MASK m_fvf;
        //Renderer::handle_program m_program;
    };


}//namespace egg

#endif //_EGG_Component_IconMesh_H_
