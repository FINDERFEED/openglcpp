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

static float Z_NEAR = 0.1;
static float Z_FAR = -100;
static int WINDOW_WIDTH = 640;
static int WINDOW_HEIGHT = 480;
static Matrix4f PROJECTION = Matrix4f::perspective(90, WINDOW_WIDTH / (float)WINDOW_HEIGHT, Z_NEAR, Z_FAR);
static Matrix4f MODELVIEW;

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

void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Vec3f lookAt = Vec3f(0, 0, -1);
    Vec3f up = Vec3f(0, 1, 0);
    if (key == GLFW_KEY_W) {
        e->z -= 3;
        Vec3f v = *e + lookAt;
        setModelviewMatrix(*e, up, v);
    } else if (key == GLFW_KEY_S) {
        e->z += 1;
        Vec3f v = *e + lookAt;
        setModelviewMatrix(*e, up, v);
    }
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

    glfwMakeContextCurrent(window);
   

    if (glewInit() != GLEW_OK) {
        std::cout << "Shit" << std::endl;
    }

    VERTEX_FORMATS.initialize();
    SHADERS.initialize();
    


    GLuint vbo; glGenBuffers(1, &vbo);
    GLuint ibo; glGenBuffers(1, &ibo);
    GLuint vao; glGenVertexArrays(1, &vao);

    Shader* shader = SHADERS.POSITION_COLOR;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float data1[] = {
            -0.5,-0.5,0,0,0,0,1,
            0.5,-0.5,0,1,0,0,1,
            0.5,0.5,0,0,1,0,1,
            -0.5,0.5,0,0,0,1,1
    };
    glBufferData(GL_ARRAY_BUFFER,7 * 4 * sizeof(float), data1, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
    ByteBuffer buffer = ByteBuffer(1024);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetWindowSizeCallback(window,*onWindowResize);
    glfwSetKeyCallback(window, *onKeyInput);



    Vec3f eye = Vec3f(0, 0, 0);
    Vec3f up = Vec3f(0, 1, 0);
    Vec3f lookAt = Vec3f(0, 0, -1);
    setModelviewMatrix(eye, up, lookAt);

    int frame = 0;
    float s = 40;

    VertexBuffer* b = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_TRIANGLES);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        
  
        Matrix4f mat = Matrix4f();
        mat.rotateZDegrees(frame);

        float z = -40;
        Vec4f v1 = Vec4f(-s, -s, z, 1); //mat.transform(v1);
        Vec4f v2 = Vec4f(s, -s, z, 1); //mat.transform(v2);
        Vec4f v3 = Vec4f(s, s, z, 1); //mat.transform(v3);
        Vec4f v4 = Vec4f(-s, s, z, 1); //mat.transform(v4);
        b->position(mat, v1.x, v1.y, v1.z)->color(0, 0, 0, 1)->endVertex();
        b->position(mat, v2.x, v2.y, v2.z)->color(1, 0, 0, 1)->endVertex();
        b->position(mat, v3.x, v3.y, v3.z)->color(0, 1, 0, 1)->endVertex();
        b->position(mat, v4.x, v4.y, v4.z)->color(0, 0, 1, 1)->endVertex();
        float* data = new float[7*4]{
            v1.x,v1.y,v1.z,0,0,0,1,
            v2.x,v2.y,v2.z,1,0,0,1,
            v3.x,v3.y,v3.z,0,1,0,1,
            v4.x,v4.y,v4.z,0,0,1,1
        };
        buffer.putFloats(28, data);


        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * 4, buffer.getData(), GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
        //glBindVertexArray(vao);
        //glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
        //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));

       
        shader->process();
        shader->mat4uniform("projection",PROJECTION);

        shader->mat4uniform("modelview", MODELVIEW);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        b->draw();
        shader->stop();

        
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
        buffer.resetPosition();
        delete[] data;
    }

    glfwTerminate();
    return 0;
}

