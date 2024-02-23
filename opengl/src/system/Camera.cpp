#pragma once
#include "../math/Matrix4f.cpp"
#include "../math/Matrix3f.cpp"
#include "../Util.cpp";
class Camera {

private:
	float pitch = 0;

public:
	float x = 0;
	float y = 0;
	float z = 0;
	float yaw = 0;
	



	Matrix4f& matrix() {
		Vec3f pos = Vec3f(x, y, z);
		Vec3f look = this->getLookAngle();
		Vec3f lookAt = pos + look;
		Vec3f up = Vec3f(0, 1, 0);
		Matrix4f m = Matrix4f::modelview(pos, lookAt,up);
		return m;
	}

	Vec3f& getLookAngle() {
		Vec3f look = Vec3f(0, 0, -1);
		Matrix4f mat = Matrix4f();
		Quaternion q = Quaternion(0,1,0,yaw);
		Quaternion q2 = Quaternion(1,0,0,pitch);
		q.multiply(q2);
		mat.rotate(q);

		//mat.rotateYDegrees(yaw);
		//mat.rotateXDegrees(pitch);
		mat.transform(look);
		look.normalize();
		return look;
	}

	void move(Vec3f& move) {
		x += move.x;
		y += move.y;
		z += move.z;
	}

	void setPos(Vec3f& pos) {
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}

	void setPitch(float val) {
		pitch = Util::clamp(val, -89.9, 89.9);
	}
	
	float getPitch() {
		return pitch;
	}
};