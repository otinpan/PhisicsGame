#include "motherObject.h"
#include "object.h"
MotherObject::MotherObject(Object* obj)
	:mMotherObject(obj)
{
	obj->setMotherObject(this);
	obj->setIsCombined(true);
	obj->setIsMother(true);
	obj->setSMotherAngle(obj->getAngle());
	obj->setSMotherOffset(glm::vec2(0.0f, 0.0f));
}

MotherObject::~MotherObject() {
	mChildren.clear();
}

void MotherObject::addChild(Object* obj) {
	mChildren.emplace_back(obj);
	// 重心の変更
	glm::vec3 center = glm::vec3(
		(mMotherObject->getMass() * mMotherObject->getCenter() + obj->getMass() * obj->getCenter()) /
		(mMotherObject->getMass() + obj->getMass())
	);
	mMotherObject->setCenter(center);
	obj->setCenter(center);

	// 慣性モーメントの変更
	float inertia=(
		mMotherObject->getInertia() + obj->getInertia() +
		obj->getMass() * glm::dot(glm::vec2(obj->getCenter() - mMotherObject->getCenter()),
			glm::vec2(obj->getCenter() - mMotherObject->getCenter()))
	);
	mMotherObject->setInertia(inertia);
	obj->setInertia(inertia);

	// 質量の変更
	float mass = mMotherObject->getMass() + obj->getMass();
	mMotherObject->setMass(mass);
	obj->setMass(mass);

	// 初期の相対位置、角度の設定
	obj->setSMotherAngle(mMotherObject->getAngle());
	obj->setSMotherOffset(glm::vec2(obj->getPosition() - mMotherObject->getPosition()));

	// 速度の変更
	mMotherObject->setVelocity(glm::vec2(0.0f, 0.0f));
	obj->setVelocity(glm::vec2(0.0f, 0.0f));
	mMotherObject->setAngularSpeed(0.0f);
	obj->setAngularSpeed(0.0f);

	obj->setMotherObject(this);
	obj->setIsCombined(true);
}