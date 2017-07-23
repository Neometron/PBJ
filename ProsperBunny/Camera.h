#pragma once
#include "Graphics.h"
#include "MathTypes.h"
#include "Buffer.h"
#include "GraphicsTypes.h"

namespace HBS_PB
{
	struct triangleStructure
	{
		float distanceToHit;
		float tr1a;
		float tr1b;
		float tr1c;

		triangleStructure() {}
		triangleStructure(float tr1, float tr2, float tr3, float dist)
		{
			tr1a          = tr1;
			tr1b          = tr2;
			tr1c          = tr3;
			distanceToHit = dist;
		}
	};
	class Camera
	{
	public:
		Camera(HBS_GRAPHICS::Graphics&, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
		~Camera();
	public:
		const             Matrix getViewProj() const;
		void              pickRayVector(float mouseX, float mouseY, HBS::HBS_MATH::Vector& pickRayInWorldSpacePos, HBS::HBS_MATH::Vector& pickRayInWorldSpaceDir);
		triangleStructure pick(HBS::HBS_MATH::Vector& pickRayInWorldSpacePos, HBS::HBS_MATH::Vector& pickRayInWorldSpaceDir, const std::vector<HBS_PB::Subset>& mesh, Matrix& worldSpace);

		void              updateCameraLocation(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	private:
		bool              PointInTriangle(HBS::HBS_MATH::Vector& triV1, HBS::HBS_MATH::Vector& triV2, HBS::HBS_MATH::Vector& triV3, HBS::HBS_MATH::Vector& point);
	private:
		HBS_GRAPHICS::Graphics& graphics;
		HBS_GRAPHICS::Buffer    projCB;
		HBS_GRAPHICS::Buffer    viewCB;

		float camYaw;
		float camPitch;
		float moveBackForward;
		float moveLeftRight;

		Matrix projMat;
		Matrix viewMat;

		HBS::HBS_MATH::Vector DefaultForward;
		HBS::HBS_MATH::Vector DefaultRight;
		HBS::HBS_MATH::Vector camForward;
		HBS::HBS_MATH::Vector camRight;
		HBS::HBS_MATH::Vector camUp;
		HBS::HBS_MATH::Vector camPosition;
		HBS::HBS_MATH::Vector camTarget;
	};
}