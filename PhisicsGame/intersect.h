#pragma once
#include "object.h"

bool isIntersects(Object* A, Object* B);
bool isIntersectsCC(Object* c1, Object* c2);
bool isIntersectsCP(Object* c, Object* p);
bool isIntersectsPP(Object* p1, Object* p2);

void solveCollisionCC(Object* c1, Object* c2);