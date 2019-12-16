#include "pch.h"
#include "CameraComp.h"
#include "Camera.h"
#include "input.h"

void CameraComp::OnUpdate(float deltaTime)
{

}

void CameraComp::OnRender()
{

}

void CameraComp::Start()
{

}

void CameraComp::OnStart()
{

	m_camera = new Camera(m_entity->GetTransform());

	if (Application::GetInstance()->GetCamera() == nullptr)
	{
		Application::GetInstance()->SetCamera(m_camera);
	}

}