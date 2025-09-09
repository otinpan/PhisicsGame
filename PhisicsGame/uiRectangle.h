#include "uiObject.h"

class UIRectangle :public UIObject {
public:
	UIRectangle(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
		float restitution, glm::vec3 scale);
	~UIRectangle();
	void initialize(class UI* ui)override;
	void update(float deltaTime)override;
	void draw(Shader& shader)override;
private:
};