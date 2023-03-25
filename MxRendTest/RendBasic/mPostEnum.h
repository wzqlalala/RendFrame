#pragma once

namespace MDataPost
{
	enum class PostSetType
	{
		Node,

		Element,

		ElementSurface,

		CuttingPlane,

		Part,

	};

	enum class PostRigidWallType
	{
		InfinitelyLargePlaneRigidWall,//无线大平面刚性墙

		LimitedLargePlanesRigidWall,//有限大平面刚性墙

		RectangularRigidWall,//长方体刚性墙

		CylindricalRigidWall,//圆柱状刚性墙

		SphericalRigidWall//球形刚性墙
	};

}

