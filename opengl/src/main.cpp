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

static float Z_NEAR = 0.1;
static float Z_FAR = -100;
static int WINDOW_WIDTH = 640;
static int WINDOW_HEIGHT = 480;
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

float ermit(float p, float p1, float p2, float dir1, float dir2) {
    return p1 * (2 * p * p * p - 3 * p * p + 1) + p2 * (-2 * p * p * p + 3 * p * p)
        + dir1 * (p * p * p - 2 * p * p + p) + dir2 * (p * p * p - p * p);
}

Vec3f ermitSpline(float p,Vec3f p1, Vec3f p2,Vec3f dir1,Vec3f dir2) {
    Vec3f v = Vec3f(
        ermit(p,p1.x,p2.x,dir1.x,dir2.x),
        ermit(p,p1.y,p2.y,dir1.y,dir2.y),
        ermit(p,p1.z,p2.z,dir1.z,dir2.z)
    );
    return v;
}

float bezier(float p, float p1, float p2, float p3, float p4) {
    return (1 - p) * (1 - p) * (1 - p) * p1 +
        3 * p * (1 - p) * (1 - p) * p2 +
        3 * p * p * (1 - p) * p3 +
        p * p * p * p4;
}

float bezierDerivative(float p, float p1,float p2,float p3,float p4) {
    return -3 * (p - 1) * (p - 1) * p1 +
        3 * (p - 1) * (3 * p - 1) * p2 +
        3 * -p * (3 * p - 2) * p3 +
        3 * p * p * p4;
}

Vec3f bezierSpline(float p, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4) {
    Vec3f v = Vec3f(
        bezier(p,p1.x,p2.x,p3.x,p4.x),
        bezier(p,p1.y,p2.y,p3.y,p4.y),
        bezier(p,p1.z,p2.z,p3.z,p4.z)
    );
    return v;
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

    
    ObjModel* level = ObjModel::loadModel("level_model");
    ObjModel* rocket = ObjModel::loadModel("rocket");

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

    Texture texture = Texture("bait");
    Texture bricks = Texture("bricks");
    Texture welt = Texture("welt");
    Texture rocketTex = Texture("cringe");
    Framebuffer framebuffer = Framebuffer("shadow",0,0,0,1,1920,1080,0);
    

    Matrix4f lineMat = Matrix4f();
    points = new MouseMoveablePoints(WINDOW_WIDTH, WINDOW_HEIGHT);
    float frames = 0;
    Matrix4f mat = Matrix4f();

    Path* path = new Path();

    float r = 3;

    Vec3f p1 = Vec3f(r,2.5,r);
    Vec3f p12 = Vec3f(r - 2,1.5,r);
    Vec3f p2 = Vec3f(-r,1,r);
    Vec3f p23 = Vec3f(-r,0,r - 3);
    Vec3f p3 = Vec3f(-r,0,-r);
    Vec3f p34 = Vec3f(-r + 2,0,-r);
    Vec3f p4 = Vec3f(r,1,-r);
    path->
        addPoint(p1)->
        addPoint(p12)->

        addPoint(p2)->
        addPoint(p23)->

        addPoint(p3)->
        addPoint(p34)->

        addPoint(p4);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        MODELVIEW = camera.matrix();
        glEnable(GL_DEPTH_TEST);
        glLineWidth(4);
        drawOrthos(lineMat,lineb);




        //
        SHADERS.POSITION_COLOR_TEX_NORMAL->process();
        SHADERS.POSITION_COLOR_TEX_NORMAL->mat4uniform("modelview", MODELVIEW);
        SHADERS.POSITION_COLOR_TEX_NORMAL->mat4uniform("projection", PROJECTION);
        SHADERS.POSITION_COLOR_TEX_NORMAL->textureUniform("sampler0", bricks);
        level->render(mat,pct,pctr,1,1,1,1,1);

        Matrix4f tr = Matrix4f();
        Vec3f direction = Vec3f();
        Vec3f currentPoint = path->getPoint((float)frames / 1000.0,&direction);
        tr.translate(currentPoint.x,currentPoint.y,currentPoint.z);


        SHADERS.POSITION_COLOR_TEX_NORMAL->textureUniform("sampler0", rocketTex);
        RenderUtil::applyMovementMatrixRotations(tr, direction);
        rocket->render(tr, pct, pctr, 1, 1, 1, 1,1);
        
        SHADERS.POSITION_COLOR_TEX_NORMAL->stop();

        

        //

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


