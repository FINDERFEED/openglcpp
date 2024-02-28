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
    Vec3f move = camera.getLookAngle() * 0.5;
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
   

    if (glewInit() != GLEW_OK) {
        std::cout << "Shit" << std::endl;
    }

    VERTEX_FORMATS.initialize();
    SHADERS.initialize();
    
    Shader* shader = SHADERS.POSITION_COLOR;


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

    VertexBuffer* b = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_TRIANGLES);

    VertexBuffer* lineb = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_LINES);
    Matrix4f lineMat = Matrix4f();
    Matrix4f mat = Matrix4f();
    //Quaternion q = Quaternion(0, 0, 1, 45);
    //q.multiply(q);
    //mat.rotate(q);
    
    //mat.rotate(q);
    glLineWidth(4);
    b->position(mat, 0, 0, 0)->color(1, 0, 0, 1)->endVertex();
    b->position(mat, 1, 0, 1)->color(0, 1, 0, 1)->endVertex();
    b->position(mat, 1, 1, 1)->color(0, 0, 1, 1)->endVertex();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        MODELVIEW = camera.matrix();

        drawOrthos(lineMat,lineb);

       

        shader->process();
        shader->mat4uniform("projection",PROJECTION);
        shader->mat4uniform("modelview",MODELVIEW);
        b->draw(1);

        shader->stop();

        glfwSwapBuffers(window);
        prevPosX = posX;
        prevPosY = posY;
        glfwPollEvents();
        frame++;
    }

    glfwTerminate();
    return 0;
}


