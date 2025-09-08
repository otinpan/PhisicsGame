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
	//mChildren.clear();
}

void MotherObject::addChild(Object* obj) {
	mChildren.emplace_back(obj);

	
	// 重心の変更
	glm::vec3 center = glm::vec3(
		(mMotherObject->getMass() * mMotherObject->getCenter() + obj->getMass() * obj->getCenter()) /
		(mMotherObject->getMass() + obj->getMass())
	);
	mMotherObject->setCenter(center);


	// 慣性モーメントの変更
	float inertia=(
		mMotherObject->getInertia() + obj->getInertia() +
		obj->getMass() * glm::dot(glm::vec2(obj->getCenter() - mMotherObject->getCenter()),
			glm::vec2(obj->getCenter() - mMotherObject->getCenter()))
	);
	mMotherObject->setInertia(inertia);

	// 質量の変更
	float mass = mMotherObject->getMass() + obj->getMass();
	mMotherObject->setMass(mass);
	obj->setPartialMass(obj->getMass());


	// 初期の相対位置、角度の設定
	obj->setSMotherAngle(mMotherObject->getAngle());
	obj->setSMotherOffset(glm::vec2(obj->getPosition() - mMotherObject->getPosition()));
	obj->setSCombinedAngle(obj->getAngle());

	// 速度の変更
	mMotherObject->setVelocity(glm::vec2(0.0f, 0.0f));
	obj->setVelocity(glm::vec2(0.0f, 0.0f));
	mMotherObject->setAngularSpeed(0.0f);
	obj->setAngularSpeed(0.0f);

	obj->setMotherObject(this);
	obj->setIsCombined(true);

	// 全てのchildに採用
	for (auto& child : mChildren) {
		child->setCenter(center);
		child->setInertia(inertia);
		child->setMass(mass);
		child->setVelocity(glm::vec2(0.0f, 0.0f));
		child->setAngularSpeed(0.0f);
	}

}

void MotherObject::addMother(MotherObject* mother) {

	// 重心の変更
	glm::vec3 center = glm::vec3(
		(mMotherObject->getMass() * mMotherObject->getCenter() +
			mother->getMotherObject()->getMass() * mother->getMotherObject()->getCenter()) 
		/ (mMotherObject->getMass() + mother->getMotherObject()->getMass())
	);
	mMotherObject->setCenter(center);


	// 慣性モーメントの変更
	float inertia = (
		mMotherObject->getInertia() + mother->getMotherObject()->getInertia() +
		mother->getMotherObject()->getMass() * glm::dot(glm::vec2(mother->getMotherObject()->getCenter() - mMotherObject->getCenter()),
			glm::vec2(mother->getMotherObject()->getCenter() - mMotherObject->getCenter()))
		);
	mMotherObject->setInertia(inertia);

	// 質量の変更
	float mass = mMotherObject->getMass() + mother->getMotherObject()->getMass();
	mMotherObject->setMass(mass);

	// 初期の相対位置、角度の変更
	mother->getMotherObject()->setSMotherAngle(mMotherObject->getAngle());
	mother->getMotherObject()->
		setSMotherOffset(glm::vec2(mother->getMotherObject()->getPosition() - mMotherObject->getPosition()));
	mother->getMotherObject()->setSCombinedAngle(mother->getMotherObject()->getAngle()); 
	mother->getMotherObject()->setIsMother(false);
	for (auto& child : mother->getChildren()) {
		child->setSMotherAngle(mMotherObject->getAngle());
		child->setSMotherOffset(glm::vec2(child->getPosition() - mMotherObject->getPosition()));
		printf("offset: (%f,%f)\n", child->getSMotherOffset().x, child->getSMotherOffset().y);
		child->setSCombinedAngle(child->getAngle());
	}
	
	// 追加
	mChildren.emplace_back(mother->getMotherObject());
	for (auto& child : mother->getChildren()) {
		mChildren.emplace_back(child);
	}

	// 全てのchildに採用
	for (auto& child : mChildren) {
		child->setCenter(center);
		child->setInertia(inertia);
		child->setMass(mass);
		child->setVelocity(glm::vec2(0.0f, 0.0f));
		child->setAngularSpeed(0.0f);
		child->setMotherObject(this);
		child->setIsCombined(true);
	}
}

float MotherObject::solveMoment(float deltaTime) {
	// 回転を計算
	glm::vec2 center = glm::vec2(mMotherObject->getCenter());

	// 角速度
	float angularSpeed = 0.0f;

	for (auto& child : mChildren) {
		// childの位置
		glm::vec2 pos = glm::vec2(child->getPosition());
		glm::vec2 rel = pos - center;
		// 力を計算
		glm::vec2 f = child->getPartialMass() * child->getGravity();

		// モーメントを計算
		float moment = cross2D(rel, f);

		// 角速度を加算
		angularSpeed += moment / mMotherObject->getInertia() * deltaTime;
	}

	return angularSpeed;

}