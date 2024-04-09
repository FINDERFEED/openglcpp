
#include "../math/Vec3f.cpp";
#include "../math/Vec4f.cpp";
#include "../math/Matrix4f.cpp";
#include "../Matrix4fStack.cpp";
#include "Camera.cpp";
#include "Framebuffer.cpp";
#include "../batching/VertexBuffer.cpp";
#include "../Shaders.cpp";

class MouseMoveablePoints {

public:

	std::vector<Vec3f> points;
	Framebuffer* pickingFramebuffer;
	VertexBuffer* buffer;

	MouseMoveablePoints(){}

	MouseMoveablePoints(int windowWidth,int windowHeight) {
		TextureParameters params = TextureParameters().setFilter(GL_NEAREST, GL_NEAREST).setWrap(GL_REPEAT, GL_REPEAT)
			.setLoadParameters(GL_RGB32F, GL_RGB).setResolution(windowWidth, windowHeight);
		TextureParameters depthParams = TextureParameters().setFilter(GL_NEAREST, GL_NEAREST).setWrap(GL_REPEAT, GL_REPEAT)
			.setLoadParameters(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT).setResolution(windowWidth, windowHeight);
		pickingFramebuffer = new Framebuffer("picking_framebuffer", params, depthParams,windowWidth,windowHeight);
		buffer = new VertexBuffer(VERTEX_FORMATS.POSITION_COLOR,1024,GL_QUADS);
	}

	void addPoint(float x,float y,float z) {
		Vec3f v = Vec3f(x, y, z);
		points.push_back(v);
	}

	int onMouseClick(Camera& camera,Matrix4f& projection,Matrix4f& modelview,int mx,int my) {

		this->renderAllPoints(camera, projection, modelview);

		pickingFramebuffer->bindWrite();
		float pixels[4];
		glReadPixels(mx,my,1,1,GL_RGBA,GL_FLOAT,pixels);
		pickingFramebuffer->unbind();
		return (int)pixels[0];
	}
	
	void renderAllPoints(Camera& camera,Matrix4f& projection, Matrix4f& modelview) {
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		pickingFramebuffer->bindWrite();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix4fStack stack = Matrix4fStack();
		stack.push();
		Quaternion q = camera.getRotationQuaternion();
		int point = 10;
		for (Vec3f v : points) {
			stack.push();
			stack.translate(v.x, v.y, v.z);
			stack.rotate(q);
			Matrix4f cameraMat = *stack.getLast();
			buffer->position(cameraMat, -0.5, -0.5, 0)->color(point,point,point,1)->endVertex();
			buffer->position(cameraMat, -0.5, 0.5, 0)->color(point, point, point, 1)->endVertex();
			buffer->position(cameraMat, 0.5, 0.5, 0)->color(point, point, point, 1)->endVertex();
			buffer->position(cameraMat, 0.5, -0.5, 0)->color(point, point, point, 1)->endVertex();
			stack.pop();
			point++;
		}
		SHADERS.POSITION_COLOR->process();
		SHADERS.POSITION_COLOR->mat4uniform("projection", projection);
		SHADERS.POSITION_COLOR->mat4uniform("modelview", modelview);
		buffer->draw(1);
		
	    pickingFramebuffer->unbind();
		buffer->draw(0);
		SHADERS.POSITION_COLOR->stop();

		stack.pop();
	}

	void onResize(int newWindowWidth, int newWindowHeight) {
		pickingFramebuffer->resize(newWindowWidth, newWindowHeight, newWindowWidth, newWindowHeight);
	}

};