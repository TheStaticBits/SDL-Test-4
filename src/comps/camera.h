#pragma once

namespace Comps
{
	// Camera entity
	struct Camera 
	{
		float followSpeed;
		Vect<float> offset;
		Vect<bool> lockedDirs;
	};
}

namespace Tags
{
	struct CameraFollow {};
}