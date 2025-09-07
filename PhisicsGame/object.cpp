
#include "object.h"
#include "play.h"
#include "cup.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

	mAngularSpeed += mAngularAcceleration * deltaTime;
	mAngle += mAngularSpeed * deltaTime;

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

void Object::collisionCup() {
	float percent = 0.95f;

	// cupとの衝突係数
	float e = (float)(getRestitution() + getPlay()->getCup()->getRestitution()) / 2.0f;

	std::vector<glm::vec2> left, right, bottom;
	float left_dis = 0.0f, right_dis = 0.0f, bottom_dis = 0.0f;
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);

	float mCupLeft = getPlay()->getCup()->getLeft();
	float mCupRight = getPlay()->getCup()->getRight();
	float mCupBottom = getPlay()->getCup()->getBottom();

	for (auto& v : mVertices) {
		// 左の壁と衝突
		if (v.x < mCupLeft) {
			left_dis = std::max(left_dis, mCupLeft - v.x);
			left.emplace_back(v);
		}
		// 右の壁と衝突
		if (v.x > mCupRight) {
			right_dis = std::max(right_dis, v.x - mCupRight);
			right.emplace_back(v);
		}
		// 底面と衝突
		if (v.y < mCupBottom) {
			bottom_dis = std::max(bottom_dis, mCupBottom - v.y);
			bottom.emplace_back(v);
		}
	}

	offset.x += left_dis * percent;
	offset.x -= right_dis * percent;
	offset.y += bottom_dis * percent;

	if (bottom.size() == 2) {
		if (abs(mAngularSpeed) < M_PI / 30.0f) {
			setCenter(mCenter + glm::vec3(offset, 0.0f));
			setAngularSpeed(0.0f);
			setVelocity(glm::vec2(0.0f, 0.0f));
			for (auto& v : mVertices) {
				v += glm::vec3(offset, 0.0f);
			}
			return;
		}
	}

	// 速度の更新
	if (left.size() == 1) {
		solveCollisionCup(glm::vec2(1.0f, 0.0f), left[0]);
	}
	else if (left.size() == 2) {
		solveCollisionCup(glm::vec2(1.0f, 0.0f), (left[0] + left[1]) / 2.0f);
	}

	if (right.size() == 1) {
		solveCollisionCup(glm::vec2(-1.0f, 0.0f), right[0]);
	}
	else if (right.size() == 2) {
		solveCollisionCup(glm::vec2(-1.0f, 0.0f), (right[0] + right[1]) / 2.0f);
	}

	if (bottom.size() == 1) {
		solveCollisionCup(glm::vec2(0.0f, 1.0f), bottom[0]);
	}
	else if (bottom.size() == 2) {
		solveCollisionCup(glm::vec2(0.0f, 1.0f), (bottom[0] + bottom[1]) / 2.0f);
	}

	// 位置の更新
	setCenter(mCenter + glm::vec3(offset, 0.0f));
	for (auto& v : mVertices) {
		v += glm::vec3(offset, 0.0f);
	}
}

void Object::solveCollisionCup(glm::vec2 n,glm::vec2 p){
	// 重心から衝突点へのベクトル
	glm::vec2 r = p - glm::vec2(mCenter);

	float e = (float)(getRestitution() + getPlay()->getCup()->getRestitution()) / 2.0f;

	// 衝突前の速度
	glm::vec2 vp = mVelocity + glm::vec2(-r.y * mAngularSpeed, r.x * mAngularSpeed);
	

	// 法線方向の速さ
	float vpn = glm::dot(vp, n);
	if (vpn >= 0.0f)return;

	// impluse
	float jn = -(1.0f + e) * vpn / (1.0f / mMass + (r.x * n.y - r.y * n.x) * (r.x * n.y - r.y * n.x) / mInertia);

	// world座標
	glm::vec2 J = n * jn;

	// 速度の更新
	mVelocity += J / mMass;
	mAngularSpeed += (r.x * J.y - r.y * J.x) / mInertia;
}

void Object::processInput(const InputState& state) {

}

void Object::draw(Shader& shader) {
}

