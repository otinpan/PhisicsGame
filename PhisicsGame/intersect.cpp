#include "intersect.h"
#define _USE_MATH_DEFINES
#include <math.h>



glm::vec2 LineSegment::pointOnSegment(float t)const {
	return mStart + (mEnd - mStart) * t;
}

glm::vec2 LineSegment::getMinDistancePos(const glm::vec2& p)const {
	// �x�N�g���̏���
	glm::vec2 ab = mEnd - mStart;
	glm::vec2 ba = -ab;
	glm::vec2 ac = p - mStart;
	glm::vec2 bc = p - mEnd;

	// case1: �_C���猩�ē_A���ł��߂��ꍇ
	if (glm::dot(ab, ac) < 0.0f) {
		return mStart;
	}
	// case2: �_C���猩�ē_B���ł��߂��ꍇ
	if (glm::dot(ba, bc) < 0.0f) {
		return mEnd;
	}
	//case3: c������Ɏˉe
	else {
		float sc = glm::dot(ac, ab) / glm::dot(ab, ab);
		glm::vec2 d = glm::vec2(sc * ab);
		return mStart + d;
	}
}

glm::vec2 LineSegment::getIntersectPos(const LineSegment& other)const {
	float s, t,deno = cross2D(mEnd - mStart, other.mEnd-other.mStart);

	glm::vec2 err(FLT_MAX, FLT_MAX);
	if (deno == 0.0f) {
		return err;
	}

	s = cross2D(other.mStart - mStart, other.mEnd - other.mStart) / deno;
	t = cross2D(mEnd - mStart, mStart - other.mStart) / deno;

	if (s < 0.0f || s>1.0f || t < 0.0f || t>1.0f) {
		return err;
	}

	return pointOnSegment(s);

}


bool isIntersects(Object* A, Object* B) {
	// object�������Ȃ�Փ˂��Ȃ�
	if (A == B)return false;

	// mother�������Ȃ�Փ˂��Ȃ�
	if (A->getIsCombined() && B->getIsCombined()) {
		if (A->getMotherObject() == B->getMotherObject()) {
			return false;
		}
	}

	// hitbox�őe������
	glm::vec2 rel = glm::vec2(A->getPosition() - B->getPosition());
	if ((rel.x * rel.x + rel.y * rel.y) > (A->getHitRadius() + B->getHitRadius()) * (A->getHitRadius() + B->getHitRadius())) {
		return false;
	}
	// �ڂ�������
	if (A->getShapeType() == ShapeType::SHAPE_CIRCLE && B->getShapeType() == ShapeType::SHAPE_CIRCLE) {
		if (isIntersectsCC(A, B)) {
			//CombineObject(A, B);
			return true;
		}
	}
	if (A->getShapeType() == ShapeType::SHAPE_CIRCLE && B->getShapeType() != ShapeType::SHAPE_CIRCLE) {	
		if (isIntersectsCP(A, B)) {
			//CombineObject(A, B);
			return true;
		}
	}
	if (A->getShapeType() != ShapeType::SHAPE_CIRCLE && B->getShapeType() == ShapeType::SHAPE_CIRCLE) {
		if (isIntersectsCP(B, A)) {
			//CombineObject(A, B);
			return true;
		}
	}
	if (A->getShapeType() != ShapeType::SHAPE_CIRCLE && B->getShapeType() != ShapeType::SHAPE_CIRCLE) {
		if (isIntersectsPP(A, B)) {
			//CombineObject(A, B);
			return true;
		}
	}
	return false;
}

bool isIntersectsCC(Object* c1, Object* c2) {
	glm::vec2 rel = c1->getPosition() - c2->getPosition();
	if ((rel.x * rel.x + rel.y * rel.y) > (c1->getRadius() + c2->getRadius()) * (c1->getRadius() + c2->getRadius())) {
		return false;
	}
	float m1 = c1->getMass();
	float m2 = c2->getMass();

	// �@���x�N�g��
	glm::vec2 n = glm::normalize(rel);

	// �߂荞�݂̉���
	float overlap = ((c1->getRadius() + c2->getRadius()) - glm::length(rel));
	if (overlap > 0.0f) {
		glm::vec2 offset = n * overlap / 1.95f;
		c1->updateCenter(glm::vec3(offset, 0.0f));
		c1->updatePosition(glm::vec3(offset, 0.0f));
		c2->updateCenter(glm::vec3(-offset, 0.0f));
		c2->updatePosition(glm::vec3(-offset, 0.0f));
	}

	if (abs(c1->getAngularSpeed()) < M_PI / 1000.0f && abs(c1->getVelocity().x) < 0.01f && abs(c1->getVelocity().y) < 0.01f &&
		abs(c2->getAngularSpeed()) < M_PI / 1000.0f && abs(c2->getVelocity().x) < 0.01f && abs(c2->getVelocity().y) < 0.01f)
	{
		CombineObject(c1, c2);
		return true;
	}

	updateParameters(c1, c2, n, (c1->getPosition() + c2->getPosition()) / 2.0f);
	return true;
}

bool isIntersectsCP(Object* c, Object* p) {
	// circle���S�����ԋ߂��ӂ̃x�N�g��
	glm::vec2 minVec;
	glm::vec2 minPos;
	float minDis = FLT_MAX;
	bool isIntersects = false;

	int size = p->getVertices().size();
	for (int i = 0; i < size; i++) {
		LineSegment s(p->getVertices()[i % size], p->getVertices()[(i + 1) % size]);
		glm::vec2 d = s.getMinDistancePos(c->getPosition()); // circle�̒��S�����ԋ߂��_
		glm::vec2 v = d - glm::vec2(c->getPosition());
		float dis = glm::length(v);
		if (dis <= c->getRadius()) {
			isIntersects = true;
			if (dis < minDis) {
				minPos = d;
				minDis = dis;
				minVec = v;
			}
		}
	}

	if (!isIntersects)return false;

	minVec = glm::normalize(minVec);
	float overlap = c->getRadius() - minDis;
	glm::vec2 offset = minVec * overlap / 1.95f;

	p->updateCenter(glm::vec3(offset, 0.0f));
	p->updatePosition(glm::vec3(offset, 0.0f));
	c->updateCenter(glm::vec3(-offset, 0.0f));
	c->updatePosition(glm::vec3(-offset, 0.0f));

	// ��~���Ă���Ȃ�X�V���Ȃ�
	if (abs(c->getAngularSpeed()) < M_PI / 1000.0f && abs(c->getVelocity().x) < 0.01f && abs(c->getVelocity().y) < 0.01f &&
		abs(p->getAngularSpeed()) < M_PI / 1000.0f && abs(p->getVelocity().x) < 0.01f && abs(p->getVelocity().y) < 0.01f)
	{
		CombineObject(c, p);
		return true;
	}

	updateParameters(c, p, minVec, minPos);
	return true;
}

bool isIntersectsPP(Object* p1, Object* p2) {
	const float EPS = 1e-6;

	glm::vec2 err = { FLT_MAX,FLT_MAX };
	int size1 = p1->getVertices().size();
	int size2 = p2->getVertices().size();
	std::vector<glm::vec2> intersectPoints;
	for (int i = 0; i < size1; i++) {
		LineSegment s1(p1->getVertices()[i % size1], p1->getVertices()[(i + 1) % size1]);
		for (int j = 0; j < size2; j++) {
			LineSegment s2(p2->getVertices()[j % size2], p2->getVertices()[(j + 1) % size2]);
			glm::vec2 intersectPos = s1.getIntersectPos(s2);
			if (intersectPos != err) {
				intersectPoints.emplace_back(intersectPos);
			}
		}
	}

	if (intersectPoints.size() < 2) {
		return false;
	}

	glm::vec2 dir = intersectPoints[1] - intersectPoints[0];
	glm::vec2 n;
	if (dir.x * dir.x + dir.y * dir.y < EPS * EPS) {
		n = p1->getPosition() - p2->getPosition();
		n = glm::normalize(n);
	}
	else {
		dir = glm::normalize(dir);
		n = glm::vec2(-dir.y, dir.x);
	}

	auto proj = [&](Object* obj) {
		float mn = FLT_MAX, mx = -FLT_MAX;
		std::vector<glm::vec3> verts = obj->getVertices();
		for (auto& v : verts) {
			float d = glm::dot(glm::vec2(v), n);
			mn = std::min(mn, d);
			mx = std::max(mx, d);
		}
		return std::pair<float, float> {mn, mx};
	};
	auto proj1 = proj(p1);
	auto proj2 = proj(p2);

	float overlap = std::min(proj1.second - proj2.first, proj2.second - proj1.first);
	if (overlap <= 0.0f) {
		return false;
	}

	// ��p�_
	glm::vec2 fp = (intersectPoints[0] + intersectPoints[1]) / 2.0f;


	// �ʒu�̍X�V
	glm::vec2 offset = n * overlap / 1.95f;
	// �@���x�N�g���ƍ�p�_����d�S�ւ̃x�N�g�����݊p�Ȃ甽�]
	if (glm::dot(n, glm::vec2(p1->getPosition()) - fp) < 0.0f) {
		offset = -offset;
	}


	p1->updateCenter(glm::vec3(offset, 0.0f));
	p1->updatePosition(glm::vec3(offset, 0.0f));

	p2->updateCenter(glm::vec3(-offset, 0.0f));
	p2->updatePosition(glm::vec3(-offset, 0.0f));

	if (abs(p1->getAngularSpeed()) < M_PI / 1000.0f && abs(p1->getVelocity().x) < 0.01f && abs(p1->getVelocity().y) < 0.01f &&
		abs(p2->getAngularSpeed()) < M_PI / 1000.0f && abs(p2->getVelocity().x) < 0.01f && abs(p2->getVelocity().y) < 0.01f)
	{
		CombineObject(p1, p2);
		return true;
	}

	// ���x�̍X�V
	updateParameters(p1, p2, n, fp);

	return true;
}


void updateParameters(Object* A, Object* B, glm::vec2 n, glm::vec2 p) {
	//�d�S����Փ˓X�ւ̃x�N�g��
	glm::vec2 ra = p - glm::vec2(A->getCenter());
	glm::vec2 rb = p - glm::vec2(B->getCenter());

	//�ՓˑO�̑��x
	glm::vec2 va = A->getVelocity() + glm::vec2(-ra.y * A->getAngularSpeed(), ra.x * A->getAngularSpeed());
	glm::vec2 vb = B->getVelocity() + glm::vec2(-rb.y * B->getAngularSpeed(), rb.x * B->getAngularSpeed());

	// �����W��
	float e = (float)(A->getRestitution() + B->getRestitution()) / 2.0f;

	// �@�������̑���
	n = glm::normalize(n);
	float va_n = va.x * n.x + va.y * n.y;
	float vb_n = vb.x * n.x + vb.y * n.y;

	// impulse
	float jn = -(1.0f + e) * (va_n - vb_n) /
		(1.0f / A->getMass() + 1.0f / B->getMass()
			+ (cross2D(ra, n) * cross2D(ra, n)) / A->getInertia()
			+ (cross2D(rb, n) * cross2D(rb, n)) / B->getInertia());

	// world���W��impulse
	glm::vec2 J = n * jn;

	// ���x�̍X�V
	A->updateVelocity( J / A->getMass());
	B->updateVelocity(- J / B->getMass());

	// �p���x�̍X�V
	A->updateAngularSpeed(cross2D(ra, J) / A->getInertia());
	B->updateAngularSpeed(-cross2D(rb, J) / B->getInertia());
}