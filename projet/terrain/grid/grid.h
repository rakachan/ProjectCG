#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

struct Light {
        glm::vec3 La = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Ld = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 2.0f);

        // pass light properties to the shader
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            // given in camera space
            GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

            GLuint La_id = glGetUniformLocation(program_id, "La");
            GLuint Ld_id = glGetUniformLocation(program_id, "Ld");
            GLuint Ls_id = glGetUniformLocation(program_id, "Ls");

            glUniform3fv(light_pos_id, ONE, glm::value_ptr(light_pos));
            glUniform3fv(La_id, ONE, glm::value_ptr(La));
            glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
            glUniform3fv(Ls_id, ONE, glm::value_ptr(Ls));
        }
};

class Grid: public Light {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint heightmap_;                     // texture ID
        GLuint colormap_;                       // Colormap
        GLuint num_indices_;                    // number of vertices to render
        GLuint M_id_;                         // model, view, proj matrix ID
        GLuint V_id_;
        GLuint P_id_;


    public:
        void Init(GLuint tex) {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("grid_vshader.glsl",
                                                  "grid_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates and indices
            {
                std::vector<GLfloat> vertices;
                std::vector<GLuint> indices;
                int grid_dim = 100;

                for(int i=-grid_dim/2; i<grid_dim/2; i++) {
                    for(int j=-grid_dim/2; j<grid_dim/2; j++) {
                        vertices.push_back((j)/(grid_dim/2.0f)); vertices.push_back((i)/(grid_dim/2.0f));
                        vertices.push_back((j +1)/(grid_dim/2.0f)); vertices.push_back((i)/(grid_dim/2.0f));
                        vertices.push_back((j)/(grid_dim/2.0f)); vertices.push_back((i +1)/(grid_dim/2.0f));
                        vertices.push_back((j +1)/(grid_dim/2.0f)); vertices.push_back((i +1)/(grid_dim/2.0f));
                    }
                }

                int shift = 0;
                for(int i=0; i<grid_dim; i++) {
                    for(int j=0; j<grid_dim; j++) {
                        shift = i*4*grid_dim+j*4;
                        indices.push_back(0+shift);
                        indices.push_back(1+shift);
                        indices.push_back(2+shift);
                        indices.push_back(1+shift);
                        indices.push_back(3+shift);
                        indices.push_back(2+shift);
                    }
                }

                num_indices_ = indices.size();

                // position buffer
                glGenBuffers(1, &vertex_buffer_object_position_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                             &vertices[0], GL_STATIC_DRAW);

                // vertex indices
                glGenBuffers(1, &vertex_buffer_object_index_);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                             &indices[0], GL_STATIC_DRAW);

                // position shader attribute
                GLuint loc_position = glGetAttribLocation(program_id_, "position");
                glEnableVertexAttribArray(loc_position);
                glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // load texture
            {
                heightmap_ = tex;
                GLuint tex_id = glGetUniformLocation(program_id_, "tex");
                glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
            }

            {
                const int ColormapSize=3;
                GLfloat tex[3*ColormapSize] = {
                                               /*green*/    48.0f/255.0f, 186.0f/256.0f, 143.0f/255.0f,
                                               /*yellow*/   186/255.0f, 142/255.0f, 47/255.0f,
                                               /*white*/    150/255.0f, 141/255.0f, 153/255.0f};
                glGenTextures(1, &colormap_);
                glBindTexture(GL_TEXTURE_1D, colormap_);
                glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, ColormapSize, 0, GL_RGB, GL_FLOAT, tex);
                glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                GLuint tex_id = glGetUniformLocation(program_id_, "colormap");
                glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
                // check_error_gl();
            }
            // other uniforms
            M_id_ = glGetUniformLocation(program_id_, "M");
            V_id_ = glGetUniformLocation(program_id_, "V");
            P_id_ = glGetUniformLocation(program_id_, "P");
            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);

            Light::Setup(program_id_);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteProgram(program_id_);
            glDeleteTextures(1, &heightmap_);
        }

        void Draw(const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, heightmap_);

            // setup MVP
            glUniformMatrix4fv(M_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(model));
            glUniformMatrix4fv(V_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(view));
            glUniformMatrix4fv(P_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(projection));

            // draw
            // TODO 5: for debugging it can be helpful to draw only the wireframe.
            // You can do that by uncommenting the next line.
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
