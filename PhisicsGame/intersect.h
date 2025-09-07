#pragma once
#include "object.h"
#include <vector>

struct LineSegment {
public:
	LineSegment(glm::vec2 start, glm::vec2 end)
		:mStart(start)
		, mEnd(end)
	{

	}

	glm::vec2 pointOnSegment(float t)const;
	glm::vec2 getMinDistancePos(const glm::vec2& p)const;
	glm::vec2 getIntersectPos(const LineSegment& other)const;

private:
	glm::vec2 mStart;
	glm::vec2 mEnd;
};

bool isIntersects(Object* A, Object* B);
bool isIntersectsCC(Object* c1, Object* c2);
bool isIntersectsCP(Object* c, Object* p);
bool isIntersectsPP(Object* p1, Object* p2);

void updateParameters(Object* A, Object* B,glm::vec2,glm::vec2);

