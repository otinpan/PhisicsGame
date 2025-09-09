#include "uiObject.h"

class UICircle :public UIObject {
public:
	UICircle(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
		float restitution, glm::vec3 scale);
	~UICircle();

	void initialize(class UI* ui)override;
	void update(float deltaTime)override;
	void draw(Shader& shader)override;
private:
};