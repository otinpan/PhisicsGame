
#include "object.h"
#include "play.h"
#include "cup.h"
#define _USE_MATH_DEFINES
#include <math.h>

Object::Object(glm::vec3 center, glm::vec3 rgb, GLMesh& mesh, float mass, float restitution, glm::vec3 scale, float angle)
	: mCenter(center)
	, mPosition(center)
	, mColor(rgb)
	, mVelocity(glm::vec2(0.0f, 0.0f))
	, mAngle(angle)
	, mScale(scale)
	, mModel(1.0f)
	, mMesh(mesh)
	, mPlay(nullptr)
	, mMotherObject(nullptr)
	, mMass(mass)
	, mRestitution(restitution)
	, mState(ACTIVE)
	, mRadius(0.0f)
	, mGravity(glm::vec2(0.0f, 0.0f))
	, mIsCombined(false)
	, mIsMother(false)
	, sMotherAngle(0.0f)
	, sMotherOffset(glm::vec2(0.0f, 0.0f))
	, sCombinedAngle(0.0f)
	, mPartialMass(0.0f)
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
	mPlay->setHaveObject(this);

	sVertices = getMesh().vertices;

	setSVertices(sVertices);


	glm::mat4 model(1.0f);
	model = glm::translate(model, getPosition());
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
	// 合体しているとき
	if (mIsCombined) {
		if (mIsMother) {
			float w=mMotherObject->solveMoment(deltaTime);
			updateAngularSpeed(w);
			setCenter(mCenter + glm::vec3(getVelocity() * deltaTime, 0.0f));
			setPosition(mCenter);
		}
		else {

			//printf("velocityC: (%f,%f), angularspeedC : %f\n", getVelocity().x, getVelocity().y, getAngularSpeed());
			
			float angle = mMotherObject->getMotherObject()->getAngle() - sMotherAngle;
			glm::vec3 relativePos = glm::vec3(
				rotateAround(
					sMotherOffset,
					glm::vec2(0.0f, 0.0f),
					angle
				),
			0.0f);

			setPosition(mMotherObject->getMotherObject()->getPosition() + relativePos);
			setAngle(angle + sCombinedAngle);
		}
	}
	else {
		setCenter(mCenter + glm::vec3(getVelocity() * deltaTime, 0.0f));
		setPosition(mCenter);
	}


	glm::mat4 model(1.0f);
	model = glm::translate(model, getPosition());
	model = glm::rotate(model, getAngle(), glm::vec3(0, 0, 1));
	model = glm::scale(model, getScale());
	setModel(model);

	std::vector<glm::vec3> transformedVertices;
	for (const auto& vertex : getSVertices()) {
		transformedVertices.push_back(glm::vec3(model * glm::vec4(vertex, 1.0f)));
	}

	setVertices(transformedVertices);
}

void Object::updateCenter(glm::vec3 offset) {
	glm::vec3 center = mCenter + offset;

	//合体しているとき
	if (mIsCombined) {
		mMotherObject->getMotherObject()->setCenter(center);
		for (auto& child : mMotherObject->getChildren()) {
			child->setCenter(center);
		}
	}
	else {
		mCenter += offset;
	}
}

void Object::updatePosition(glm::vec3 offset) {

	// 合体しているとき
	if (mIsCombined) {
		mMotherObject->getMotherObject()->setPosition(offset + mMotherObject->getMotherObject()->getPosition());
		auto verts = mMotherObject->getMotherObject()->getVertices();
		for (auto& v : verts) {
			v += offset;
		}
		mMotherObject->getMotherObject()->setVertices(verts);

		for (auto& child : mMotherObject->getChildren()) {
			child->setPosition(offset + child->getPosition());
			auto verts = child->getVertices();
			for (auto& v : verts) {
				v += offset;
			}
			child->setVertices(verts);
		}
	}
	else {
		mPosition += offset;
		for (auto& v : mVertices) {
			v += offset;
		}
	}
}

void Object::updateVelocity(glm::vec2 offset) {
	glm::vec2 velocity = mVelocity + offset;
	// 合体しているとき
	if (mIsCombined) {
		mMotherObject->getMotherObject()->setVelocity(velocity);
		for (auto& child : mMotherObject->getChildren()) {
			child->setVelocity(velocity);
		}
	}
	else {
		mVelocity += offset;
	}
}

void Object::updateAngularSpeed(float offset) {
	float angularSpeed = mAngularSpeed + offset;
	// 合体しているとき
	if (mIsCombined) {
		mMotherObject->getMotherObject()->setAngularSpeed(angularSpeed);
		for (auto& child : mMotherObject->getChildren()) {
			child->setAngularSpeed(angularSpeed);
		}
	}
	else {
		mAngularSpeed += offset;
	}
}


void Object::collisionCup() {

	float percent = 1.00f;

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

	// 停止条件
	if (bottom.size() == 2) {
		if (abs(mAngularSpeed) < M_PI / 30.0f) {
			updateCenter(glm::vec3(offset, 0.0f));
			updatePosition(glm::vec3(offset, 0.0f));
			updateVelocity(-mVelocity);
			updateAngularSpeed(-mAngularSpeed);
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
	updateCenter(glm::vec3(offset, 0.0f));
	updatePosition(glm::vec3(offset, 0.0f));
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
	float jn = -(1.0f+e ) * vpn / (1.0f / mMass + (r.x * n.y - r.y * n.x) * (r.x * n.y - r.y * n.x) / mInertia);

	// world座標
	glm::vec2 J = n * jn;

	// 速度の更新
	updateVelocity( J / mMass);
	updateAngularSpeed((r.x * J.y - r.y * J.x) / mInertia);
}

void Object::processInput(const InputState& state) {

}

void Object::draw(Shader& shader) {
}


void CombineObject(Object* obj1, Object* obj2) {
	// どちらも単体の場合
	if (!obj1->getIsCombined() && !obj2->getIsCombined()) {
		MotherObject* mother = new MotherObject(obj1);
		mother->addChild(obj2); //centerの更新、obj2の相対位置の設定
	}
	// obj1が合体していてobj2が単体の場合
	else if (obj1->getIsCombined() && !obj2->getIsCombined()) {
		obj1->getMotherObject()->addChild(obj2);
	}
	// obj2が合体していてobj1が単体の場合
	else if (!obj1->getIsCombined() && obj2->getIsCombined()) {
		obj2->getMotherObject()->addChild(obj1);
	}
	// どちらも合体している場合
	else {
		// すでに同じmotherに合体しているなら何もしない
		if (obj1->getMotherObject() == obj2->getMotherObject()) {
			return;
		}
		if (obj1->getMotherObject()->getChildren().size() > obj2->getMotherObject()->getChildren().size()) {
			obj1->getMotherObject()->addMother(obj2->getMotherObject());
			//delete obj2->getMotherObject();
		}
		else {
			obj2->getMotherObject()->addMother(obj1->getMotherObject());
			//delete obj1->getMotherObject();
		}
	}
}