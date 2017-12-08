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
};