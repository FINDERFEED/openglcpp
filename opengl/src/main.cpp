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

    ObjModel* model = ObjModel::loadModel("testsphere");
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

    int frame = 0;
    float s = 40;

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
    Matrix4f mat = Matrix4f();
    
    Matrix4f mat2 = Matrix4f();
    mat2.rotateZDegrees(15);

    points = new MouseMoveablePoints(WINDOW_WIDTH, WINDOW_HEIGHT);
    Vec3f* point = new Vec3f(0,1,0);
    points->addPoint(point);

    Vec3f* pb1 = new Vec3f(15, 0, 5);
    Vec3f* pb2 = new Vec3f(15, 0, 15);
    Vec3f* pb3 = new Vec3f(15, 10, 7);
    Vec3f* pb4 = new Vec3f(15, 10, 12);
    points->addPoint(pb1);
    points->addPoint(pb2);
    points->addPoint(pb3);
    points->addPoint(pb4);

    Vec3f center = Vec3f(30, 0, 30);
    BezierSurface surface = BezierSurface(center,10);
    surface.addAllPointsAsMoveable(*points);

    float h = 10;
    float frames = 0;
    float rocketFlightProgress = 0;
    while (!glfwWindowShouldClose(window)) {
        
        Matrix4f lightProj = Matrix4f::perspective(60, 1, Z_NEAR, Z_FAR);
 
        Vec3f eye = Vec3f(0, 40, 0);
        Vec3f up = Vec3f(0, 1, 0);
        Vec3f lookAt = Vec3f(0,0,0.01);
        Matrix4f lightModelview = Matrix4f::modelview(eye,lookAt,up);

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        MODELVIEW = camera.matrix();
        glDisable(GL_DEPTH_TEST);
       
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

        texshader->process();
        texshader->mat4uniform("projection", PROJECTION);
        texshader->mat4uniform("modelview", MODELVIEW);
        texshader->textureUniform("sampler0", texture);
        test->draw(0);
        texshader->stop();
        


        glEnable(GL_DEPTH_TEST);
        glLineWidth(4);
        drawOrthos(lineMat,lineb);

        Vec3f n1 = Util::computeNormal(
            -6, 4, 0,
            0, 2, 0,
            0, 2, -h
        );
        pct->position(mat, -6, 4, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat,n1)->endVertex();
        pct->position(mat, 0, 2, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, 0, 2, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat2, n1)->endVertex();
        pct->position(mat2, -6, 4, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat2, n1)->endVertex();
        


        n1 = Util::computeNormal(
            -6, 4, 0,
            -1, 0, 0,
            -1, 0, -h
        );
        n1.reverse();
        pct->position(mat, -6, 4, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, n1)->endVertex();
        pct->position(mat, -1, 0, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, -1, 0, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat2, n1)->endVertex();
        pct->position(mat2, -6, 4, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat2, n1)->endVertex();
        
        n1 = Util::computeNormal(
            -1, 0, 0,
            -2, -5, 0,
            -2, -5, -h
        );
        n1.reverse();
        pct->position(mat, -1, 0, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, n1)->endVertex();
        pct->position(mat, -2, -5, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, -2, -5, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat, n1)->endVertex();
        pct->position(mat2, -1, 0, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat, n1)->endVertex();
        
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

            n1 = Util::computeNormal(
                x, -5 * scaleFactor, 0,
                x, -5 * scaleFactor, -h,
                x1, -5 * scaleFactorn, -h
            );
            pct->position(mat,x,-5 * scaleFactor,0)->color(0, 0.8, 0, 1)->uv(u1,0)->normal(mat, n1)->endVertex();
        
            pct->position(mat2,x,-5 * scaleFactor,-h)->color(0, 0.8, 0, 1)->uv(u1,1)->normal(mat2, n1)->endVertex();
            pct->position(mat2,x1,-5 * scaleFactorn,-h)->color(0, 0.8, 0, 1)->uv(u2,1)->normal(mat2, n1)->endVertex();
            
            pct->position(mat, x1, -5 * scaleFactorn, 0)->color(0, 0.8, 0, 1)->uv(u2,0)->normal(mat, n1)->endVertex();

        }

        //end green
        n1 = Util::computeNormal(

            3, -5, 0,
            2, 1, 0,
            2, 1, -h

        );
        n1.reverse();
        pct->position(mat, 3, -5, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, n1)->endVertex();
        pct->position(mat, 2, 1, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, 2, 1, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat2, n1)->endVertex();
        pct->position(mat2, 3, -5, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat2, n1)->endVertex();
        

        n1 = Util::computeNormal(
            2, 1, 0,
            8, 5, 0,
            8, 5, -h
        );
        n1.reverse();
        pct->position(mat, 2, 1, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, n1)->endVertex();
        pct->position(mat, 8, 5, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, 8, 5, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat2, n1)->endVertex();
        pct->position(mat2, 2, 1, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat2, n1)->endVertex();
        
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

            n1 = Util::computeNormal(
                p1c.x, p1c.y, 0,
                p1c.x, p1c.y, -h,
                p1n.x, p1n.y, -h

            );
            pct->position(mat, p1c.x, p1c.y, 0)->color(0.75, 0, 1, 0.75)->uv(i,0)->normal(mat,n1)->endVertex();
            pct->position(mat2, p1c.x, p1c.y, -h)->color(0.75, 0, 1, 0.75)->uv(i,1)->normal(mat2, n1)->endVertex();
            pct->position(mat2, p1n.x, p1n.y, -h)->color(0.75, 0, 1, 0.75)->uv(i+step,1)->normal(mat2, n1)->endVertex();
            pct->position(mat, p1n.x, p1n.y, 0)->color(0.75, 0, 1, 0.75)->uv(i+step,0)->normal(mat, n1)->endVertex();
        }
        //end purple
        
        n1 = Util::computeNormal(
            1, 7, 0,
            0, 2, 0,
            0, 2, -h
        );
        n1.reverse();
        pct->position(mat, 1, 7, 0)->color(1, 1, 1, 1)->uv(0,0)->normal(mat, n1)->endVertex();
        pct->position(mat, 0, 2, 0)->color(1, 1, 1, 1)->uv(1,0)->normal(mat, n1)->endVertex();
        pct->position(mat2, 0, 2, -h)->color(1, 1, 1, 1)->uv(1,1)->normal(mat2, n1)->endVertex();
        pct->position(mat2, 1, 7, -h)->color(1, 1, 1, 1)->uv(0,1)->normal(mat2, n1)->endVertex();


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        pctn->process();
        pctn->mat4uniform("projection", PROJECTION);
        pctn->mat4uniform("modelview", MODELVIEW);
        if (!useWelt) {
            pctn->textureUniform("sampler0", bricks);
        }
        else {
            pctn->textureUniform("sampler0", welt);

        }

        glDisable(GL_CULL_FACE);
        step = 0.1f;
        Matrix4f bezierMat = Matrix4f();
        for (float u = 0; u < 1; u += step) {
            for (float v = 0; v < 1; v += step) {
                Vec3f p1 = surface.calcPoint(u, v);
                Vec3f p2 = surface.calcPoint(u + step, v);
                Vec3f p3 = surface.calcPoint(u + step, v + step);
                Vec3f p4 = surface.calcPoint(u, v + step);

                Vec3f normalv = Util::computeNormal(p3, p2, p1);

                pct->position(bezierMat, p4.x, p4.y, p4.z)->color(0.5, 0.5, 0.5, 1)->uv(u, v + step)->normal(bezierMat, normalv)->endVertex();
                pct->position(bezierMat, p3.x, p3.y, p3.z)->color(0.5, 0.5, 0.5, 1)->uv(u + step, v + step)->normal(bezierMat, normalv)->endVertex();
                pct->position(bezierMat, p2.x, p2.y, p2.z)->color(0.5, 0.5, 0.5, 1)->uv(u + step, v)->normal(bezierMat, normalv)->endVertex();
                pct->position(bezierMat, p1.x, p1.y, p1.z)->color(0.5, 0.5, 0.5, 1)->uv(u, v)->normal(bezierMat, normalv)->endVertex();


            }
        }


        pct->draw(0);
        pctn->stop();

        Matrix4f ch;
        ch.translate(0, sin(frame / PI / 10), 0);

        framebuffer.bindWrite();
        Texture d = *framebuffer.getDepthTexture();
        glViewport(0, 0, d.getWidth(), d.getHeight());
        glEnable(GL_DEPTH_TEST);

        shadow->process();

        shadow->mat4uniform("projection", lightProj);
        shadow->mat4uniform("modelview",lightModelview);
        model->render(ch, pct, pctr, 1, 1, 1, 1, 1);
        framebuffer.unbind();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        shadow->stop();

    
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->process();
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->mat4uniform("projection", PROJECTION);
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->mat4uniform("modelview", MODELVIEW);
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->mat4uniform("lightProjection", lightProj);
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->mat4uniform("lightModelview", lightModelview);
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->textureUniform("sampler0", bricks);
        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->textureUniform("shadowSampler", d,1);

        pct->position(mat, -100, -10, -100)->color(1, 1, 1, 1)->uv(0, 0)->normal(mat,0,1,0)->endVertex();
        pct->position(mat, 100, -10, -100)->color(1, 1, 1, 1)->uv(1, 0)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, 100, -10, 100)->color(1, 1, 1, 1)->uv(1, 1)->normal(mat, 0, 1, 0)->endVertex();
        pct->position(mat, -100, -10, 100)->color(1, 1, 1, 1)->uv(0, 1)->normal(mat, 0, 1, 0)->endVertex();

        


        pct->draw(0);

        SHADERS.POSITION_COLOR_TEX_NORMAL_SHADOW->stop();
        
        framebuffer.bindWrite();
        glClearColor(0, 0, 0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        framebuffer.unbind();
                
        glDisable(GL_BLEND);




        Vec3f begin = Vec3f(5,0,5);
        Vec3f end = Vec3f(10,0,10);
        Vec3f dirb = Vec3f(0, 10, 0);
        Vec3f dire = Vec3f(5,-10,5);
        Vec3f v1 = begin;
        Vec3f v2 = begin + dirb;
        lineb->position(mat, v1.x, v1.y, v1.z)->color(1, 1, 1, 1)->endVertex();
        lineb->position(mat, v2.x, v2.y, v2.z)->color(1, 1, 1, 1)->endVertex();
        v1 = end;
        v2 = end + dire;
        lineb->position(mat, v1.x, v1.y, v1.z)->color(1, 1, 1, 1)->endVertex();
        lineb->position(mat, v2.x, v2.y, v2.z)->color(1, 1, 1, 1)->endVertex();

        float st = 0.05;
        for (float i = 0; i < 1; i += st) {
            float i1 = i + st;
            Vec3f v = ermitSpline(i, begin, end, dirb, dire);
            Vec3f v1 = ermitSpline(i1, begin, end, dirb, dire);
            lineb->position(mat, v.x, v.y, v.z)->color(1, 1, 0,1)->endVertex();
            lineb->position(mat, v1.x, v1.y, v1.z)->color(1, 1, 0,1)->endVertex();
        }



        begin = *pb1;
        end = *pb2;
        dirb = *pb3;
        dire = *pb4;
        lineBetween(mat, lineb, begin, dirb, 1, 1, 1, 1);
        lineBetween(mat, lineb, dirb, dire, 1, 1, 1, 1);
        lineBetween(mat, lineb, dire, end, 1, 1, 1, 1);
        for (float i = 0; i < 1; i += st) {
            float i1 = i + st;
            Vec3f v = bezierSpline(i, begin, dirb, dire, end);
            Vec3f v1 = bezierSpline(i1, begin, dirb, dire, end);
            lineb->position(mat, v.x, v.y, v.z)->color(1, 1, 0,1)->endVertex();
            lineb->position(mat, v1.x, v1.y, v1.z)->color(1, 1, 0,1)->endVertex();
        }

        Matrix4f rocketMat = Matrix4f();
        
        float flight = 0.001;
        rocketFlightProgress += flight;
        if (rocketFlightProgress >= 1) {
            rocketFlightProgress = 0;
        }

        Vec3f flightPoint1 = bezierSpline(rocketFlightProgress, begin, dirb, dire, end);
        Vec3f dir = bezierDirection(rocketFlightProgress, begin, dirb, dire, end);
        rocketMat.translate(flightPoint1.x, flightPoint1.y, flightPoint1.z);
        RenderUtil::applyMovementMatrixRotations(rocketMat, dir);

        SHADERS.POSITION_COLOR_TEX_NORMAL->process();
        SHADERS.POSITION_COLOR_TEX_NORMAL->mat4uniform("projection", PROJECTION);
        SHADERS.POSITION_COLOR_TEX_NORMAL->mat4uniform("modelview", MODELVIEW);
        SHADERS.POSITION_COLOR_TEX_NORMAL->textureUniform("sampler0",rocketTex);
        rocket->render(rocketMat,pct,pctr,1,1,1,1,1);
        SHADERS.POSITION_COLOR_TEX_NORMAL->stop();


        

        SHADERS.POSITION_COLOR->process();
        SHADERS.POSITION_COLOR->mat4uniform("projection", PROJECTION);
        SHADERS.POSITION_COLOR->mat4uniform("modelview", MODELVIEW);
        
        
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
        

        frame++;
    }

    glfwTerminate();
    return 0;
}


