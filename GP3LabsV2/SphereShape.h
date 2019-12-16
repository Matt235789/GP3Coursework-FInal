#pragma once
#include "pch.h"
#include "CollisionShape.h"

class SphereShape : public CollisionShape
{
private:
	float sphereRadius;

public:
	SphereShape(btScalar);
};
