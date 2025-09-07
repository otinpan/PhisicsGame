#include "intersect.h"



glm::vec2 LineSegment::pointOnSegment(float t)const {
	return mStart + (mEnd - mStart) * t;
}

glm::vec2 LineSegment::getMinDistancePos(const glm::vec2& p)const {
	// ベクトルの準部
	glm::vec2 ab = mEnd - mStart;
	glm::vec2 ba = -ab;
	glm::vec2 ac = p - mStart;
	glm::vec2 bc = p - mEnd;

	// case1: 点Cから見て点Aが最も近い場合
	if (glm::dot(ab, ac) < 0.0f) {
		return mStart;
	}
	// case2: 点Cから見て点Bが最も近い場合
	if (glm::dot(ba, bc) < 0.0f) {
		return mEnd;
	}
	//case3: cを線分に射影
	else {
		float scalar = glm::dot(ac, ab) / glm::dot(ab, ab);
		glm::vec2 d = glm::vec2(scalar * ab);
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
	// hitboxで粗い判定
	glm::vec2 rel = glm::vec2(A->getCenter() - B->getCenter());
	if ((rel.x * rel.x + rel.y * rel.y) > (A->getHitRadius() + B->getHitRadius()) * (A->getHitRadius() + B->getHitRadius())) {
		return false;
	}
	// 詳しい判定
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
	if ((rel.x * rel.x + rel.y * rel.y) < (c1->getRadius() + c2->getRadius()) * (c1->getRadius() + c2->getRadius())) {
		return false;
	}
	float m1 = c1->getMass();
	float m2 = c2->getMass();

	// 法線ベクトル
	glm::vec2 n = glm::normalize(rel);

	// めり込みの解消
	float overlap = ((c1->getRadius() + c2->getRadius()) - glm::length(rel));
	if (overlap > 0.0f) {
		glm::vec2 offset = n * overlap / 1.95f;
		c1->setCenter(c1->getCenter() - (offset,0.0f));
		c2->setCenter(c2->getCenter() + (offset, 0.0f));
	}

	updateParameters(c1, c2, n, (c1->getCenter() + c2->getCenter()) / 2.0f);

}

bool isIntersectsCP(Object* c, Object* p) {
	// circl中心から一番近い辺のベクトル

	return false;
}

bool isIntersectsPP(Object* p1, Object* p2) {
	return false;
}
void updateParameters(Object* A, Object* B, glm::vec2, glm::vec2) {
	return;
}