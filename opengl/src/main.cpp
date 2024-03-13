#pragma once
#include "Shader.cpp";
#include <GLFW/glfw3.h>

#include <iostream>

#include "math/Matrix4f.cpp"
#include "math/Vec4f.cpp"
#include "buffers/FloatBuffer.cpp";
#include "buffers/ByteBuffer.cpp";
#include "batching/Elements.cpp";
#include "batching/VertexFormats.cpp"
#include "Shaders.cpp"
#include "batching/VertexBuffer.cpp"
#include "system/Camera.cpp"
#include "RenderUtil.cpp"
#include "Matrix4fStack.cpp"
#include "system/Texture.cpp"

static float Z_NEAR = 0.1;
static float Z_FAR = -100;
static int WINDOW_WIDTH = 640;
static int WINDOW_HEIGHT = 480;
static Matrix4f PROJECTION = Matrix4f::perspective(90, WINDOW_WIDTH / (float)WINDOW_HEIGHT, Z_NEAR, Z_FAR);
static Matrix4f MODELVIEW;
static Camera camera;
static GLFWwindow* WINDOW;

static void updateProjectionMatrix(int nw, int nh) {
    PROJECTION = Matrix4f::perspective(90, nw / (float)nh, Z_NEAR, Z_FAR);
}

static void setModelviewMatrix(Vec3f& eye,Vec3f& up, Vec3f& lookAt) {
    MODELVIEW = Matrix4f::modelview(eye, lookAt,up);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    updateProjectionMatrix(width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

static Vec3f* e = new Vec3f(0,0,0);

int d = 0;
void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Vec3f move = camera.getLookAngle();
    Vec3f up = Vec3f(0, 1, 0);
    Vec3f left = move.cross(up);
    Vec3f forward = left.cross(up);

    if (key == GLFW_KEY_W) {
        forward.reverse();
        camera.move(forward);
    } else if (key == GLFW_KEY_S) {
        
        camera.move(forward);
    } else if (key == GLFW_KEY_D) {
        Vec3f m = move.cross(up);
        camera.move(m);
    } else if (key == GLFW_KEY_A) {
        Vec3f m = move.cross(up);
        m.reverse();
        camera.move(m);
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (!d) {
            glfwSetInputMode(WINDOW,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        d = !d;
    } else if (key == GLFW_KEY_SPACE) {
        camera.y++;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        camera.y--;
    }
}

float prevPosX = 0;
float prevPosY = 0;
float posX = 0;
float posY = 0;

void setCursorPos(GLFWwindow* window, double xpos, double ypos) {
    posX = xpos;
    posY = ypos;
    if (glfwGetInputMode(WINDOW, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        camera.yaw += -(posX - prevPosX) * 0.1;
        camera.setPitch(camera.getPitch() + -(posY - prevPosY) * 0.1);
    }
}

/*
Vertex Buffer should be in lines mode!
*/
void drawOrthos(Matrix4f& mat, VertexBuffer* b) {
    glLineWidth(4);
    b->position(mat, 0, 0, 0)->color(1, 0, 0, 1)->endVertex();
    b->position(mat, 100, 0, 0)->color(1, 0, 0, 1)->endVertex();

    b->position(mat, 0, 0, 0)->color(0, 1, 0, 1)->endVertex();
    b->position(mat, 0, 100, 0)->color(0, 1, 0, 1)->endVertex();

    b->position(mat, 0, 0, 0)->color(0, 0, 1, 1)->endVertex();
    b->position(mat, 0, 0, 100)->color(0, 0, 1, 1)->endVertex();
    SHADERS.POSITION_COLOR->process();
    SHADERS.POSITION_COLOR->mat4uniform("projection", PROJECTION);
    SHADERS.POSITION_COLOR->mat4uniform("modelview", MODELVIEW);
    b->draw(0);
    SHADERS.POSITION_COLOR->stop();

}


int main(void) {

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    



    window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "Hello World", NULL, NULL);

    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!window) {
        glfwTerminate();
        return -1;
    }
   
    WINDOW = window;
    glfwMakeContextCurrent(window);
   
    camera.z = 10;

    if (glewInit() != GLEW_OK) {
        std::cout << "Shit" << std::endl;
    }

    VERTEX_FORMATS.initialize();
    SHADERS.initialize();
    
    Shader* shader = SHADERS.POSITION_COLOR;
    Shader* texshader = SHADERS.POSITION_COLOR_TEX;
    Shader* pctn = SHADERS.POSITION_COLOR_TEX_NORMAL;


    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetWindowSizeCallback(window,*onWindowResize);
    glfwSetKeyCallback(window, *onKeyInput);
    glfwSetCursorPosCallback(window, *setCursorPos);



    Vec3f eye = Vec3f(0, 0, 0);
    Vec3f up = Vec3f(0, 1, 0);
    Vec3f lookAt = Vec3f(0, 0, -1);
    setModelviewMatrix(eye, up, lookAt);

    int frame = 0;
    float s = 40;

    VertexBuffer* lineb = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_LINES);
    VertexBuffer* buffer = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_QUADS);
    VertexBuffer* test = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR_TEX,1024,GL_QUADS);
    VertexBuffer* pct = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR_TEX_NORMAL,1024,GL_QUADS);

    Texture texture = Texture("bait");
    Texture bricks = Texture("bricks");

    Matrix4f lineMat = Matrix4f();
    Matrix4f mat = Matrix4f();
    
    Matrix4f mat2 = Matrix4f();

    mat2.rotateZDegrees(15);
    float h = 10;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MODELVIEW = camera.matrix();
        glDisable(GL_DEPTH_TEST);
        texshader->process();
        texshader->mat4uniform("projection", PROJECTION);
        texshader->mat4uniform("modelview", MODELVIEW);
        texshader->textureUniform("sampler0", texture.getTexId());
        Matrix4f cm = Matrix4f();
        cm.translate(camera.x,camera.y,camera.z);

        test->position(cm, -100, -100, -100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, 100, -100, -100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, 100, 100, -100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, -100, 100, -100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->position(cm, -100, -100, 100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, 100, -100, 100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, 100, 100, 100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, -100, 100, 100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->position(cm, -100, -100, -100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, -100, -100, 100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, -100, 100, 100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, -100, 100, -100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->position(cm, 100, -100, -100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, 100, -100, 100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, 100, 100, 100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, 100, 100, -100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->position(cm, -100, -100, -100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, -100, -100, 100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, 100, -100, 100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, 100, -100, -100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->position(cm, -100, 100, -100)->color(1, 1, 1, 1)->uv(0, 0)->endVertex();
        test->position(cm, -100, 100, 100)->color(1, 1, 1, 1)->uv(10, 0)->endVertex();
        test->position(cm, 100, 100, 100)->color(1, 1, 1, 1)->uv(10, 10)->endVertex();
        test->position(cm, 100, 100, -100)->color(1, 1, 1, 1)->uv(0, 10)->endVertex();

        test->draw(0);
        texshader->stop();
        


        glEnable(GL_DEPTH_TEST);
        glLineWidth(4);
        drawOrthos(lineMat,lineb);




        //shader->process();
        //shader->mat4uniform("projection", PROJECTION);
        //shader->mat4uniform("modelview", MODELVIEW);

        

        pct->position(mat, -6, 4, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat,0,1,0)->endVertex();
        pct->position(mat, 0, 2, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 0, 2, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, -6, 4, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();
        
        pct->position(mat, -6, 4, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, -1, 0, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, -1, 0, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, -6, 4, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();
        
        
        pct->position(mat, -1, 0, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, -2, -5, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, -2, -5, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, -1, 0, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();
        
        //green
 

        for (int i = 0; i < 50; i++) {
            float x = i / 10. - 2;
            float x1 = (i+1) / 10. - 2;
            float u1 = (float)i / 50;
            float u2 = ((float)i + 1) / 50;
            float scaleFactor = 1 - abs(i - 25) / 25.;
            scaleFactor = cbrt(scaleFactor);
            scaleFactor *= 0.25;
            scaleFactor++;
            
            float scaleFactorn = 1 - abs((i+1) - 25) / 25.;
            scaleFactorn = cbrt(scaleFactorn);
            scaleFactorn *= 0.25;
            
            scaleFactorn++;

            pct->position(mat,x,-5 * scaleFactor,0)->color(0,0.8,0,1)->uv(u1,0)->normal(mat, 0, 1, 0)->endVertex();
        
            pct->position(mat2,x,-5 * scaleFactor,-h)->color(0,0.8,0,1)->uv(u1,1)->normal(mat, 0, 1, 0)->endVertex();
            pct->position(mat2,x1,-5 * scaleFactorn,-h)->color(0,0.8,0,1)->uv(u2,1)->normal(mat, 0, 1, 0)->endVertex();
            
            pct->position(mat, x1, -5 * scaleFactorn, 0)->color(0, 0.8, 0, 1)->uv(u2,0)->normal(mat, 0, 1, 0)->endVertex();

        }

        //end green

        pct->position(mat, 3, -5, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, 2, 1, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 2, 1, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 3, -5, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();
        
        pct->position(mat, 2, 1, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, 8, 5, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 8, 5, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 2, 1, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();
        
        //purple
  
        Vec3f p1 = Vec3f(8, 5, 0);
        Vec3f p2 = Vec3f(1, 7, 0);
        float step = 0.1;
        for (float i = 0; i < 1; i += step) {
            Vec3f between = p2 - p1;
            Vec3f b = between * i;
            Vec3f bn = between * (i+step);
            Vec3f p1c = p1 + b;
            Vec3f p1n = p1 + bn;
            float scale1 = 0.5-abs(i - 0.5); scale1 = cbrt(scale1); scale1 *= 0.25; scale1 += 1;
            float scale2 = 0.5-abs((i + step) - 0.5); scale2 = cbrt(scale2); scale2 *= 0.25; scale2 += 1;
            p1c = p1c / scale1;
            p1n = p1n / scale2;
            pct->position(mat, p1c.x, p1c.y, 0)->color(0.75, 0, 1, 1)->uv(i,0)->normal(mat, 0, 1, 0)->endVertex();
            pct->position(mat2, p1c.x, p1c.y, -h)->color(0.75, 0, 1, 1)->uv(i,1)->normal(mat, 0, 1, 0)->endVertex();
            pct->position(mat2, p1n.x, p1n.y, -h)->color(0.75, 0, 1, 1)->uv(i+step,1)->normal(mat, 0, 1, 0)->endVertex();
            pct->position(mat, p1n.x, p1n.y, 0)->color(0.75, 0, 1, 1)->uv(i+step,0)->normal(mat, 0, 1, 0)->endVertex();
        }
        //end purple
        
        pct->position(mat, 1, 7, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, 0, 2, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 0, 2, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat2, 1, 7, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, 0, 1, 0)->endVertex();


        pctn->process();
        pctn->mat4uniform("projection", PROJECTION);
        pctn->mat4uniform("modelview", MODELVIEW);
        pctn->textureUniform("sampler0", bricks.getTexId());
        pct->draw(0);
        pctn->stop();

        //buffer->draw(0);

        //shader->stop();

        glfwSwapBuffers(window);
        prevPosX = posX;
        prevPosY = posY;
        glfwPollEvents();
        frame++;
    }

    glfwTerminate();
    return 0;
}


