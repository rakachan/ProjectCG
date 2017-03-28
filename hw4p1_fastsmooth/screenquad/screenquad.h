#pragma once
#include "icg_helper.h"
#include <iostream>

class ScreenQuad {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_id_;             // texture ID
        GLuint tmp_texture_id_;

        float screenquad_width_;
        float screenquad_height_;
        float std_;

    public:
        void Init(float screenquad_width, float screenquad_height,
                  GLuint texture, GLuint tmp) {

            // set screenquad size
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;

            // compile the shaders
            program_id_ = icg_helper::LoadShaders("screenquad_vshader.glsl",
                                                  "screenquad_fshader.glsl");
            // init std to default value
            std_ = 3.0;

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                                 /*V2*/ +1.0f, -1.0f, 0.0f,
                                                 /*V3*/ -1.0f, +1.0f, 0.0f,
                                                 /*V4*/ +1.0f, +1.0f, 0.0f};
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                             vertex_point, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // texture coordinates
            {
                const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                               /*V2*/ 1.0f, 0.0f,
                                                               /*V3*/ 0.0f, 1.0f,
                                                               /*V4*/ 1.0f, 1.0f};

                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                             vertex_texture_coordinates, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                     "vtexcoord");
                glEnableVertexAttribArray(vertex_texture_coord_id);
                glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                      DONT_NORMALIZE, ZERO_STRIDE,
                                      ZERO_BUFFER_OFFSET);
            }

            // load/Assign texture
            this->texture_id_ = texture;
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLuint tex_id = glGetUniformLocation(program_id_, "tex");
            glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);


            this->tmp_texture_id_ = tmp;
            glBindTexture(GL_TEXTURE_2D, tmp_texture_id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            GLuint tmp_tex_id = glGetUniformLocation(program_id_, "tmp_tex");
            glUniform1i(tmp_tex_id, 1 /*GL_TEXTURE1*/);

            glBindTexture(GL_TEXTURE_2D, 0);

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_id_);
            glDeleteTextures(1, &tmp_texture_id_);
        }

        void UpdateSize(int screenquad_width, int screenquad_height) {
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;
        }

        void Draw(int tex) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // window size uniforms
            glUniform1f(glGetUniformLocation(program_id_, "tex_width"),
                        this->screenquad_width_);
            glUniform1f(glGetUniformLocation(program_id_, "tex_height"),
                        this->screenquad_height_);

            int size = 1 + 6 * int(ceil(std_));
            const int MAX_SIZE = (int) min(this->screenquad_width_,this->screenquad_height_);
            glUniform1i(glGetUniformLocation(program_id_, "width_kernel"), size);
            float* kernel = new float[MAX_SIZE];
            compute_1D_kernel(kernel, size, std_);
            glUniform1fv(glGetUniformLocation(program_id_, "one_kernel"), MAX_SIZE, kernel);

            // bind texture
            GLuint texid;
            if(tex == 0) {
                texid = texture_id_;
            } else {
                texid = tmp_texture_id_;
            }
            glUniform1i(glGetUniformLocation(program_id_,
            "vertical"), 1-tex);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texid);

            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
            delete [] kernel;
        }

        void compute_1D_kernel(float* a,int size, float std) {
            int cap_size = min(this->screenquad_height_, this->screenquad_width_);
            if(size > cap_size) {
                size = cap_size;
            }
            for(int i = 0; i < size; ++i) {
                float x = i - size/2.0;
                a[i] = exp(-(x*x)/(2.0*std*std*std*std)); //1.0/(2.0*6.28*std*std) *
            }
        }

        void changeStd(float inc) {
            std_ += inc;
            if(std_ < 0.0) {
                std_ = 0.0;
            }
        }
};
