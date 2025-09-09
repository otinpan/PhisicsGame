#include "uiObject.h"

class UITriangle :public UIObject {
public:
	UITriangle(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
		float restitution, glm::vec3 scale);
	~UITriangle();
	void initialize(class UI* ui)override;
	void update(float deltaTime)override;
	void draw(Shader& shader)override;
private:

};