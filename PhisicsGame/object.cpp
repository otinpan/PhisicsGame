
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

bool isIntersects(Object* A, Object* B) {
	if (A->getShapeType() == ShapeType::SHAPE_CIRCLE && B->getShapeType() == ShapeType::SHAPE_CIRCLE) {
		isIntersectsCC(A, B);
	}
	if (A->getShapeType() == ShapeType::SHAPE_CIRCLE && B->getShapeType() != ShapeType::SHAPE_CIRCLE) {
		isIntersectsCP(A, B);
	}
	if (A->getShapeType() != ShapeType::SHAPE_CIRCLE && B->getShapeType() == ShapeType::SHAPE_CIRCLE) {
		isIntersectsCP(B, A);
	}
	if (A->getShapeType() != ShapeType::SHAPE_CIRCLE && B->getShapeType() != ShapeType::SHAPE_CIRCLE) {
		isIntersectsPP(A, B);
	}
	return false;
}

bool isIntersectsCC(Object* c1, Object* c2) {
	glm::vec2 rel = c1->getCenter() - c2->getCenter();
	if ((rel.x * rel.x + rel.y * rel.y) <= (c1->getRadius() + c2->getRadius()) * (c1->getRadius() + c2->getRadius())) {
		solveCollisionCC(c1, c2);
		return true;
	}
	return false;
}

bool isIntersectsCP(Object* c, Object* p) {
	return false;
}

bool isIntersectsPP(Object* p1, Object* p2) {
	return false;
}

void solveCollisionCC(Object* c1, Object* c2) {
	glm::vec2 rel = c1->getCenter() - c2->getCenter();
	float m1 = c1->getMass();
	float m2 = c2->getMass();

	//法線ベクトル
	glm::vec2 en = glm::normalize(rel);
	//接線ベクトル
	glm::vec2 et = glm::vec2(-en.y, en.x);

	// めり込みの解消
	float overlap = (c1->getRadius() + c2->getRadius()) - glm::length(rel);
	if (overlap > 0.0f) {
		glm::vec2 offset = en * overlap / 1.9f;
		c1->setCenter(c1->getCenter() + glm::vec3(offset,0.0f));
		c2->setCenter(c2->getCenter() - glm::vec3(offset, 0.0f));
	}

	// 反発係数
	float e = (float)(c1->getRestitution() + c2->getRestitution()) / 2.0f;

	// 速度の分解
	glm::vec2 v1 = glm::vec2(glm::dot(c1->getVelocity(), en), glm::dot(c1->getVelocity(), et));
	glm::vec2 v2 = glm::vec2(glm::dot(c2->getVelocity(), en), glm::dot(c2->getVelocity(), et));

	// 衝突後の速度
	float v1n = ((m1 - e * m2) * v1.x + (1.0f + e) * m2 * v2.x) / (m1 + m2);
	float v2n = ((m2 - e * m1) * v2.x + (1.0f + e) * m1 * v1.x) / (m1 + m2);

	glm::vec2 v1f = glm::vec2(v1n, v1.y);
	glm::vec2 v2f = glm::vec2(v2n, v2.y);

	// 元の座標系に戻す
	glm::vec2 v1_world = en * v1f.x + et * v1f.y;
	glm::vec2 v2_world = en * v2f.x + et * v2f.y;

	c1->setVelocity(v1_world);
	c2->setVelocity(v2_world);
}