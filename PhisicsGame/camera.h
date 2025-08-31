#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Camera {
public:
	Camera(glm::vec3 position,glm::vec3 front,glm::vec3 up);
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float roll);
	~Camera();

	void updatePosition(Camera_Movement direction, float deltaTime);
	void updateDirection(double xpos, double ypos);
	void updateFov(double yoffset);

	glm::vec3 getPosition() const { return mPosition; }
	glm::vec3 getFront() const { return mFront; }
	glm::vec3 getUp() const { return mUp; }
	float getFov()const { return mFov; }

	

private:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;

	float mYaw;
	float mPitch;
	float mRoll;

	float mSpeed; //�J�����̈ړ��̑���

	float mSensitivity;

	bool isFirstMouse; //�}�E�X�����߂ē����Ă������ǂ���

	float lastX; //�O��̃}�E�X��X���W
	float lastY; //�O��̃}�E�X��Y���W

	float mFov; //����p


};