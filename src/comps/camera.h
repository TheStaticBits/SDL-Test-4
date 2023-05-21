#pragma once

namespace Comps
{
	// Camera entity
	struct Camera 
	{
		float followSpeed;
		Vect<float> offset; // Const offset from the object being followed
		Vect<bool> lockedDirs;
	};
}

namespace Tags
{
	struct CameraFollow {};
}