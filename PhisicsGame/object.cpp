#include "object.h"
#include "play.h"

Object::Object(glm::vec2 center, glm::vec3 rgb)
	:mCenter(center)
	, mColor(rgb)
	, mVelocity(glm::vec2(0.0f, 0.0f))
	, mAngle(0.0f)
{

}

Object::~Object() {
	if (getPlay()) {
		getPlay()->removeObject(this);
	}
}

void Object::initialize() {
	if (getPlay()) {
		getPlay()->addObject(this);
	}
}

void Object::update(float deltaTime) {

}

void Object::processInput(const InputState& state) {

}

void Object::draw() {

}