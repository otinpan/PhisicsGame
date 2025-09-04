
#include "object.h"
#include "play.h"

Object::Object(glm::vec3 center, glm::vec3 rgb, GLMesh& mesh, float mass, float restitution, glm::vec3 scale, float angle)
	: mCenter(center)
	, mColor(rgb)
	, mVelocity(glm::vec2(0.0f, 0.0f))
	, mAngle(angle)
	, mScale(scale)
	, mModel(1.0f)
	, mMesh(mesh)
	, mPlay(nullptr)
	, mMass(mass)
	, mRestitution(restitution)
	, mState(ACTIVE)
	, mRadius(0.0f)
	, mGravity(glm::vec2(0.0f, -0.1f))
{
}

Object::~Object() {
	if (getPlay()) {
		getPlay()->removeObject(this);
	}
}

void Object::initialize(class Play* play) {
	mPlay = play;
	if (getPlay()) {
		getPlay()->addObject(this);
	}

	sVertices = getMesh().vertices;

	setSVertices(sVertices);


	glm::mat4 model(1.0f);
	model = glm::translate(model, getCenter());
	model = glm::rotate(model, getAngle(), glm::vec3(0, 0, 1));
	model = glm::scale(model, getScale());
	setModel(model);

	std::vector<glm::vec3> transformedVertices;
	for (const auto& vertex : getSVertices()) {
		transformedVertices.push_back(glm::vec3(model * glm::vec4(vertex, 1.0f)));
	}

	setVertices(transformedVertices);

}

void Object::update(float deltaTime) {
	mVelocity += mGravity * deltaTime;
	setCenter(getCenter() + glm::vec3(getVelocity() * deltaTime, 0.0f));

	glm::mat4 model(1.0f);
	model = glm::translate(model, getCenter());
	model = glm::rotate(model, getAngle(), glm::vec3(0, 0, 1));
	model = glm::scale(model, getScale());
	setModel(model);

	std::vector<glm::vec3> transformedVertices;
	for (const auto& vertex : getSVertices()) {
		transformedVertices.push_back(glm::vec3(model * glm::vec4(vertex, 1.0f)));
	}

	setVertices(transformedVertices);
}

void Object::processInput(const InputState& state) {

}

void Object::draw(Shader& shader) {
}

