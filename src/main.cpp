#include <iostream>
#include <string>
#include <fstream>
#include <iterator>

#include "ada/app.h"
#include "ada/gl/gl.h"
#include "ada/gl/mesh.h"
#include "ada/gl/shader.h"
#include "ada/shaders/defaultShaders.h"
#include "ada/tools/fs.h"
#include "ada/tools/text.h"
#include "ada/tools/font.h"
#include "ada/tools/geom.h"

class myApp : public ada::App {
    ada::Vbo*   billboard;
    ada::Shader shader;
    ada::Font   font;

    void setup() {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // font.load("new_media.ttf");
        font.setAlign(ada::ALIGN_CENTER);
        font.setSize(28);
        font.setColor(0.0);

        billboard = ada::rect(0.0,0.0,1.0,1.0).getVbo();

        const std::string vert = R"(
        #ifdef GL_ES
        precision mediump float;
        #endif
        attribute vec4 a_position;
        attribute vec2 a_texcoord;
        varying vec4 v_position;
        varying vec2 v_texcoord;
        void main(void) {
            v_position =  a_position;
            v_texcoord = a_texcoord;
            
            gl_Position = v_position;
        }
        )";

        const std::string frag = R"(
        #ifdef GL_ES
        precision mediump float;
        #endif

        uniform vec2    u_resolution;
        uniform float   u_time;

        varying vec2    v_texcoord;

        void main(void) {
            vec3 color = vec3(1.0);
            vec2 st = gl_FragCoord.xy/u_resolution.xy;
            st = v_texcoord;
            
            color = vec3(st.x,st.y,abs(sin(u_time)));
            
            gl_FragColor = vec4(color, 1.0);
        }
        )";

        shader.load(frag, vert);
        // shader.load(frag, ada::getDefaultSrc(ada::VERT_DEFAULT) );
        shader.use();
    }

    void draw() {
        float width = ada::getWindowWidth();
        float height = ada::getWindowHeight();
        shader.setUniform("u_resolution", width, height );
        shader.setUniform("u_time", (float)ada::getTime());
        billboard->render( &shader );

        font.render("ABCDEFJHIJKLMNOPKRSTUVWXYZ", width * 0.5, height * 0.43);
        font.render("abcdefghijklmnopkrstuvwxyz", width * 0.5, height * 0.5);
        font.render("012345679 _,:;-'\"!@#$%^&*()-=", width * 0.5, height * 0.57);
    }

};

myApp       app;

int main(int argc, char **argv) {
    // Set the size and type of window
    glm::ivec4 window_viewport = glm::ivec4(0);
    window_viewport.z = 512;
    window_viewport.w = 512;

    #if defined(DRIVER_BROADCOM) || defined(DRIVER_GBM) 
    // RASPBERRYPI default windows size (fullscreen)
    glm::ivec2 screen = ada::getScreenSize();
    window_viewport.z = screen.x;
    window_viewport.w = screen.y;
    #endif

    ada::WindowProperties window_properties;
    window_properties.msaa = 4;
    window_properties.major = 2.0;
    window_properties.minor = 0.0;

    // Initialize openGL context
    app.run(window_viewport);

    return 1;
}