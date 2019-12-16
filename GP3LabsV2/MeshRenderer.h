#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class MeshRenderer : public Component
{
	Texture* m_texture;
	
private:
	Model* m_model;
	ShaderProgram* m_program;


public:
	//Inherited via component
	MeshRenderer(Model* model, ShaderProgram* program, Texture* texture);
	virtual void OnUpdate(float delatTime) override;
	virtual void OnRender() override;
	virtual void OnStart() override;

};
