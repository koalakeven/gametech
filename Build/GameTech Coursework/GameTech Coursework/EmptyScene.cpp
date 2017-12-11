#include "EmptyScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\GraphicsPipeline.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\PhysicsNode.h>
#include <ncltech\CollisionDetectionSAT.h>

using namespace CommonUtils;

EmptyScene::EmptyScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_StackHeight(5)
{
}

EmptyScene::~EmptyScene()
{
}

void EmptyScene::OnInitializeScene() 
{

	PhysicsEngine::Instance()->SetPaused(true);

	GraphicsPipeline::Instance()->GetCamera()->SetPosition(Vector3(0.0f, 5.0f, -15.0f));
	GraphicsPipeline::Instance()->GetCamera()->SetYaw(140.f);
	GraphicsPipeline::Instance()->GetCamera()->SetPitch(-20.f);


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
			cube->Physics()->SetElasticity(1.0f);
			cube->Physics()->SetFriction(1.0f);
			this->AddGameObject(cube);
		}
	}
	auto create_cube_tower = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth, cubewidth) * 0.5f;
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				Vector4 color = CommonUtils::GenColor(y*0.2f, 1.0f);
				Vector3 pos = offset + Vector3(x * cubewidth, 1e-3f + y * cubewidth, -0.5f);

				GameObject* cube = BuildCuboidObject(
					"",						// Optional: Name
					pos,					// Position
					halfdims,				// Half-Dimensions
					true,					// Physics Enabled?
					1.f,					// Physical Mass (must have physics enabled)
					true,					// Physically Collidable (has collision shape)
					true,					// Dragable by user?
					color);					// Render color
				this->AddGameObject(cube);
				
			}
		}
	};
	create_cube_tower(Vector3(10.0f, 0.0f, 10.0f), 1.0f);
	create_cube_tower(Vector3(-10.0f, 0.0f, -10.0f), 1.0f);


}

void EmptyScene::OnCleanupScene()
{

	Scene::OnCleanupScene();
}


void EmptyScene::OnUpdateScene(float dt)
{   
	Scene::OnUpdateScene(dt);
	int score = counter(true) * 100 - i * 50;
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   Your score is %d in this game", score);
	//Create a projectile
	GameObject *ball;
	ball = CommonUtils::BuildSphereObject("",
		Vector3(0.0f, 0.0f, 0.0f),				//Position
		1.0f,									//Radius
		true,									//Has Physics Object
		20.0f,									// Inverse Mass = 1 / 1kg mass
		true,									//No Collision Shape Yet
		true,									//Dragable by the user
		CommonUtils::GenColor(0.6f, 1.0f));

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		this->AddGameObject(ball);
		ball->Physics()->SetPosition(GraphicsPipeline::Instance()->GetCamera()->GetPosition());
		ball->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 20.0f));
		ball->Physics()->SetForce(Vector3(0.0f, 0.0f, 15.0f));
		ball->Physics()->SetAngularVelocity(Vector3(0.f, 0.f, -2.0f * PI));
		ball->Physics()->SetOnCollisionCallback(&EmptyScene::ExampleCallbackFunction1);
		CollisionDetectionSAT Colliding;		if (!Colliding.AreColliding()) {			NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "YOU NOT HIT THE TARGET");			i++;		}
	}
}



