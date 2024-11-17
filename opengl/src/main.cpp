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
#include "system/ObjModel.cpp"
#include "system/Framebuffer.cpp"
#include "system/Mouse.cpp";
#include "system/MouseMoveablePoints.cpp";
#include "misc/BezierSurface.cpp";
#include "misc/Path.cpp";
#include "splines/BSpline.h"
#include "splines/BezierSpline.h"

static float Z_NEAR = 0.1;
static float Z_FAR = -100;
static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;
static Matrix4f PROJECTION = Matrix4f::perspective(60, WINDOW_WIDTH / (float)WINDOW_HEIGHT, Z_NEAR, Z_FAR);
static Matrix4f MODELVIEW;
static Camera camera;
static GLFWwindow* WINDOW;
static int useWelt = 0;
static Mouse mouse;
static MouseMoveablePoints* points;



static void updateProjectionMatrix(int nw, int nh) {
    PROJECTION = Matrix4f::perspective(60, nw / (float)nh, Z_NEAR, Z_FAR);
}

static void setModelviewMatrix(Vec3f& eye,Vec3f& up, Vec3f& lookAt) {
    MODELVIEW = Matrix4f::modelview(eye, lookAt,up);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    updateProjectionMatrix(width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    points->onResize(width, height);
}

static Vec3f* e = new Vec3f(0,0,0);



int d = 0;
void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Vec3f move = camera.getLookAngle();
    Vec3f up = Vec3f(0, 1, 0);
    Vec3f left = move.cross(up);
    Vec3f forward = left.cross(up);
    forward.normalize();
    if (key == GLFW_KEY_W) {
        forward.reverse();
        
        camera.move(forward);
    } else if (key == GLFW_KEY_S) {
        
        camera.move(forward);
    } else if (key == GLFW_KEY_D) {
        Vec3f m = move.cross(up);
        m.normalize();
        camera.move(m);
    } else if (key == GLFW_KEY_A) {
        Vec3f m = move.cross(up);
        m.normalize();
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
        camera.y += 0.5;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        camera.y -= 0.5;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        useWelt = !useWelt;
    }
}



void setCursorPos(GLFWwindow* window, double xpos, double ypos) {
    
    mouse.setPos(xpos,WINDOW_HEIGHT -  ypos);
    /*if (glfwGetInputMode(WINDOW, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        camera.yaw += -(mouse.getMouseDX()) * 0.1;
        camera.setPitch(camera.getPitch() + (mouse.getMouseDY()) * 0.1);
    }*/
}

void onMouseButtonPress(GLFWwindow* window, int btn, int action, int mods) {
    
    if (action == GLFW_PRESS) {
        mouse.clickButton(btn);
        int id = points->onMouseClick(camera, PROJECTION, MODELVIEW, (int)mouse.getMouseX(), (int)mouse.getMouseY(),btn);
        
    } else if (action == GLFW_RELEASE) {
        mouse.releaseButton(btn);
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



float bezierDerivative(float p, float p1,float p2,float p3,float p4) {
    return -3 * (p - 1) * (p - 1) * p1 +
        3 * (p - 1) * (3 * p - 1) * p2 +
        3 * -p * (3 * p - 2) * p3 +
        3 * p * p * p4;
}


Vec3f bezierDirection(float p, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4) {
    Vec3f v = Vec3f(
        bezierDerivative(p,p1.x,p2.x,p3.x,p4.x),
        bezierDerivative(p,p1.y,p2.y,p3.y,p4.y),
        bezierDerivative(p,p1.z,p2.z,p3.z,p4.z)
    );
    return v;
}

void lineBetween(Matrix4f& mat,VertexBuffer* line,Vec3f p1,Vec3f p2,float r,float g,float b,float a) {
    line->position(mat, p1.x, p1.y, p1.z)->color(r, g, b, a)->endVertex();
    line->position(mat, p2.x, p2.y, p2.z)->color(r, g, b, a)->endVertex();
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
    Shader* circleShader = SHADERS.POSITION_COLOR_TEX_CIRCLE;
    Shader* shadow = SHADERS.SHADOW;


    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetWindowSizeCallback(window,*onWindowResize);
    glfwSetKeyCallback(window, *onKeyInput);
    glfwSetCursorPosCallback(window, *setCursorPos);
    glfwSetMouseButtonCallback(window, *onMouseButtonPress);




    Vec3f eye = Vec3f(0, 0, 0);
    Vec3f up = Vec3f(0, 1, 0);
    Vec3f lookAt = Vec3f(0, 0, -1);
    setModelviewMatrix(eye, up, lookAt);


    VertexBuffer* lineb = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_LINES);
    VertexBuffer* buffer = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_QUADS);
    VertexBuffer* test = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR_TEX,1024,GL_QUADS);
    VertexBuffer* pct = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR_TEX_NORMAL,1024,GL_QUADS);
    VertexBuffer* pctr = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR_TEX_NORMAL,1024,GL_TRIANGLES);

    

    Matrix4f lineMat = Matrix4f();
    points = new MouseMoveablePoints(WINDOW_WIDTH, WINDOW_HEIGHT);
    float frames = 0;
    Matrix4f mat = Matrix4f();

    BezierSpline spline = BezierSpline();

    auto pss = Util::readPointsFromFile("C:/Users/User/Desktop/MISC/test/asd.dat",200);

    for (int i = 0; i < pss.size();i++) {


        points->addPoint(&(pss.at(i)));
        spline.addPoint(&(pss.at(i)));

    }



    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        MODELVIEW = camera.matrix();
        glEnable(GL_DEPTH_TEST);
        glLineWidth(4);
        drawOrthos(lineMat,lineb);


        SHADERS.POSITION_COLOR->process();
        SHADERS.POSITION_COLOR->mat4uniform("projection", PROJECTION);
        SHADERS.POSITION_COLOR->mat4uniform("modelview", MODELVIEW);




        spline.draw(mat, lineb, 1, 1, 0, 1);

        lineb->draw(0);

        SHADERS.POSITION_COLOR->stop();



        points->tick(camera, mouse);
        points->renderAllPoints(camera, PROJECTION, MODELVIEW,0);
        glfwSwapBuffers(window);
        mouse.tick();
        if (glfwGetInputMode(WINDOW, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            camera.yaw += -(mouse.getMouseDX()) * 0.1;
            camera.setPitch(camera.getPitch() + (mouse.getMouseDY()) * 0.1);
        }
        glfwPollEvents();
        frames++;
    }

    glfwTerminate();
    return 0;
}


