#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <nclgl\NCLDebug.h>


//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	EmptyScene(const std::string& friendly_name);
	int m_StackHeight;
	virtual ~EmptyScene();

	virtual void OnInitializeScene() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdateScene(float dt) override;
	static bool ExampleCallbackFunction1(PhysicsNode* self, PhysicsNode* collidingObject) {
		counter();
		NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "YOU HIT THE TARGET");
		return true;
	}

	static int counter(bool get = false) {
		static int function_called_counter = 0;
		if (!get) ++function_called_counter;
		return function_called_counter;
	}
	int i = 0;
};