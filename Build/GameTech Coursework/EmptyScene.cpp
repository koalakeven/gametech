#include "EmptyScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\GraphicsPipeline.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\CommonMeshes.h>

EmptyScene::EmptyScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_StackHeight(10)
{
}

EmptyScene::~EmptyScene()
{
}

void EmptyScene::OnInitializeScene() {

	PhysicsEngine::Instance()->SetPaused(true);

	GraphicsPipeline::Instance()->GetCamera()->SetPosition(Vector3(0.0f, 5.0f, -15.0f));
	GraphicsPipeline::Instance()->GetCamera()->SetYaw(140.f);
	GraphicsPipeline::Instance()->GetCamera()->SetPitch(-20.f);

	//Who doesn't love finding some common ground?
	this->AddGameObject(CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(20.0f, 1.0f, 20.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

	const float width_scalar = 1.0f;
	const float height_scalar = 1.0f;

	for (int y = 0; y < m_StackHeight; ++y)
	{
		for (int x = 0; x <= y; ++x)
		{
			Vector4 color = CommonUtils::GenColor(y * 0.2f, 1.0f);
			GameObject* cube = CommonUtils::BuildCuboidObject(
				"",
				Vector3((x * 1.1f - y * 0.5f) * width_scalar, (0.5f + float(m_StackHeight - 1) - y) * height_scalar, -0.5f),
				Vector3(width_scalar * 0.5f, height_scalar * 0.5f, 0.5f),
				true,
				1.f,
				true,
				true,
				color);
			cube->Physics()->SetElasticity(0.0f); //No elasticity (Little cheaty)
			cube->Physics()->SetFriction(1.0f);

			this->AddGameObject(cube);
		}
	}

}

void EmptyScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
}

void EmptyScene::OnUpdateScene(float dt)
{   
	Scene::OnUpdateScene(dt);
	//Create a projectile
	GameObject *ball;
	ball = CommonUtils::BuildSphereObject("",
		Vector3(0.0f, 0.0f, 0.0f),				//Position
		0.7f,									//Radius
		true,									//Has Physics Object
		20.0f,									// Inverse Mass = 1 / 1kg mass
		true,									//No Collision Shape Yet
		false,									//Dragable by the user
		CommonUtils::GenColor(0.6f, 1.0f));


	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		this->AddGameObject(ball);
		ball->Physics()->SetPosition(GraphicsPipeline::Instance()->GetCamera()->GetPosition());
		ball->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 8.0f));
		ball->Physics()->SetForce(Vector3(0.0f, 0.0f, 15.0f));
	}
}

