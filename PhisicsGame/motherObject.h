#pragma once
#include <vector>
class MotherObject {
public:
	MotherObject(class Object* obj);
	~MotherObject();

	void addChild(class Object* child);
	void addMother(class MotherObject* mother);
	float solveMoment(float deltaTime);


	class Object* getMotherObject() { return mMotherObject; }
	std::vector<class Object*>& getChildren() { return mChildren; }
private:
	Object* mMotherObject; 
	std::vector<Object*> mChildren;
};