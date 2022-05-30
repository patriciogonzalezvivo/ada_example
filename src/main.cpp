#include "ada/app.h"
#include "ada/geom/meshes.h"
#include "ada/geom/ops.h"
#include "ada/string.h"

using namespace std;
using namespace ada;
using namespace glm;

class myApp : public App {
    Vbo     myBillboard;
    Shader  myShader;

    void setup() {
        background(0.0f);

        myBillboard = rectMesh(0.0,0.0,width, height);

        string frag = R"(
        #ifdef GL_ES
        precision mediump float;
        #endif

        uniform vec2    u_resolution;
        uniform float   u_time;

        void main(void) {
            vec3 color = vec3(1.0);
            vec2 st = gl_FragCoord.xy/u_resolution.xy;

            color = vec3(st.x,st.y,abs(sin(u_time)));
            
            gl_FragColor = vec4(color, 1.0);
        }
        )";

        myShader = createShader(frag);
        blendMode(BLEND_ALPHA);
    }

    void draw() {
        shader( myShader );
        model( myBillboard );

        fill( 0.0f );
        textAlign(ALIGN_CENTER);
        textSize(28.0f);
        text("Hello World", width * 0.5f, height * 0.5f);
        
        vector<vec2> pts;
        for (size_t i = 0; i < 100; i++) 
            pts.push_back(vec2( getWindowWidth() * ( 0.25f + i * 0.005f ), 
                                getWindowHeight() * ( 0.25f + cos( i * .031415f + time) * 0.125f ) ));

        noStroke();
        fill( 0.0f );
        rect(pts[0], vec2(20.0f) );

        stroke( 0.0f );
        line(pts);
        
        fill( 1.0f );
        pointSize(10.0f);
        pointShape(X_SHAPE);
        simplify(pts);
        points(pts);

    }

};

myApp       app;

int main(int argc, char **argv) {
    // Set the size and type of window
    ivec4 window_viewport = ivec4(0);
    window_viewport.z = 512;
    window_viewport.w = 512;

    #if defined(DRIVER_BROADCOM) || defined(DRIVER_GBM) 
    // RASPBERRYPI default windows size (fullscreen)
    ivec2 screen = getScreenSize();
    window_viewport.z = screen.x;
    window_viewport.w = screen.y;
    #endif

    WindowProperties window_properties;
    window_properties.msaa = 4;
    window_properties.major = 2.0;
    window_properties.minor = 0.0;

    // Initialize openGL context
    app.run(window_viewport);

    return 1;
}