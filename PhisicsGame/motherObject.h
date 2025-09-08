#pragma once
#include <vector>
class MotherObject {
public:
	MotherObject(class Object* obj);
	~MotherObject();

	void addChild(class Object* child);
	void setCenter();
	void setPosition();

	class Object* getMotherObject() { return mMotherObject; }
	std::vector<class Object*>& getChildren() { return mChildren; }
private:
	Object* mMotherObject; 
	std::vector<Object*> mChildren;

};