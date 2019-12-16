#pragma once
#include "Component.h"
#include "Camera.h"

class CameraComp : public Component
{
private:



public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnStart() override;

	void Start();

	Camera* m_camera;
	//Inherited via component

};
