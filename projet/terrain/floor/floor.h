#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/rotate_vector.hpp>

struct Light {
        glm::vec3 La = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Ld = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 light_pos = glm::vec3(1.0f, 1.0f, 2.0f);

        // pass light properties to the shader
        void Setup(GLuint program_id, float time) {
            glUseProgram(program_id);

            // given in camera space
            GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

            GLuint La_id = glGetUniformLocation(program_id, "La");
            GLuint Ld_id = glGetUniformLocation(program_id, "Ld");
            GLuint Ls_id = glGetUniformLocation(program_id, "Ls");
            glm::vec3 newpos = rotate(light_pos, time/(2.0f), glm::vec3(0, 0, 1));
            glUniform3fv(light_pos_id, ONE, glm::value_ptr(newpos));
            glUniform3fv(La_id, ONE, glm::value_ptr(La));
            glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
            glUniform3fv(Ls_id, ONE, glm::value_ptr(Ls));
        }
};

struct Water {
        glm::vec3 ka = glm::vec3(0.0f, 0.0f, 0.1f);
        glm::vec3 kd = glm::vec3(0.1f, 0.1f, 0.5f);
        glm::vec3 ks = glm::vec3(0.7f, 0.7f, 0.7f);
        float alpha = 64.0f;

        // pass material properties to the shaders
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            GLuint ka_id = glGetUniformLocation(program_id, "ka");
            GLuint kd_id = glGetUniformLocation(program_id, "kd");
            GLuint ks_id = glGetUniformLocation(program_id, "ks");
            GLuint alpha_id = glGetUniformLocation(program_id, "alpha");

            glUniform3fv(ka_id, ONE, glm::value_ptr(ka));
            glUniform3fv(kd_id, ONE, glm::value_ptr(kd));
            glUniform3fv(ks_id, ONE, glm::value_ptr(ks));
            glUniform1f(alpha_id, alpha);
        }
};
class Floor: public Water, public Light {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint vertex_normal_buffer_object_;
        GLuint texture_id_;
        GLuint reflexion_id_;
        GLuint normalmap_id_;
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID
        GLuint M_id_;                         // model, view, proj matrix ID
        GLuint V_id_;
        GLuint P_id_;

    public:
        void Init(int heightmap, int reflexion) {
            // compile the shaders
            program_id_ = icg_helper::LoadShaders("floor_vshader.glsl",
                                                  "floor_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            loadImage("normalmap.jpg", "normalmap", 3, normalmap_id_);

            // vertex coordinates and indices
            {
                std::vector<GLfloat> vertices;
                std::vector<GLuint> indices;
                int grid_dim = 256;


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
                this->texture_id_ = heightmap;
                //glBindTexture(GL_TEXTURE_2D, texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                GLuint tex_id = glGetUniformLocation(program_id_, "tex");
                glUniform1i(tex_id, texture_id_);

                this->reflexion_id_ = reflexion;
                //glBindTexture(GL_TEXTURE_2D, reflexion_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                GLuint ref_id = glGetUniformLocation(program_id_, "ref");
                glUniform1i(ref_id, reflexion_id_);
            }

            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            M_id_ = glGetUniformLocation(program_id_, "M");
            V_id_ = glGetUniformLocation(program_id_, "V");
            P_id_ = glGetUniformLocation(program_id_, "P");
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteBuffers(1, &vertex_normal_buffer_object_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteProgram(program_id_);
            glDeleteTextures(1, &texture_id_);
            glDeleteTextures(2, &reflexion_id_);
        }

        void Draw(const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX,
                  float time = 0,
                  vec3 cam_pos = glm::vec3(0, 0, 1)) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);


            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, reflexion_id_);
               
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Water::Setup(program_id_);
            Light::Setup(program_id_, time);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_id_);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, reflexion_id_);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, normalmap_id_);

            // setup MVP
            glm::mat4 MVP = projection*view*model;
            glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

            glUniformMatrix4fv(M_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(model));
            glUniformMatrix4fv(V_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(view));
            glUniformMatrix4fv(P_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(projection));

            glUniform3fv(glGetUniformLocation(program_id_, "cam_pos"), ONE, glm::value_ptr(cam_pos));

            // setup MVP
            //GLuint MVP_id = glGetUniformLocation(program_id_, "MVP");
            //glUniformMatrix4fv(MVP_id, 1, GL_FALSE, value_ptr(MVP));

            // time
            glUniform1f(glGetUniformLocation(program_id_, "time"), time);

            // draw
            glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
            glDisable(GL_BLEND);
        }

        void loadImage(string filename, string uniformName, int textureNb, GLuint& id) {
            // load texture
            {
                int width;
                int height;
                int nb_component;
                // set stb_image to have the same coordinates as OpenGL
                stbi_set_flip_vertically_on_load(1);
                unsigned char* image = stbi_load(filename.c_str(), &width,
                &height, &nb_component, 0);

                if(image == nullptr) {
                    throw(string("Failed to load texture " + filename));
                }

                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, -1);

                if(nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if(nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, image);
                }

                glGenerateMipmap(GL_TEXTURE_2D);

                GLuint tex_id = glGetUniformLocation(program_id_, uniformName.c_str());
                glUniform1i(tex_id, textureNb);

                // cleanup
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(image);
            }
        }
};
