#pragma once

namespace Comps
{
	// Tag for camera entity
	struct Camera 
	{
		float followSpeed;
		Vect<float> offset;
	};
}