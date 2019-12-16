#include "pch.h"
#include "SphereShape.h"

SphereShape::SphereShape(btScalar sphereRadius)
{
	m_shape = new btSphereShape(btScalar(sphereRadius));
	m_type = CShapeType::SPHERE;
}