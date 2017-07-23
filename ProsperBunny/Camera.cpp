#include "Camera.h"
using namespace HBS_PB;

Camera::Camera(HBS_GRAPHICS::Graphics& graphics, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) : graphics(graphics)
{
	DefaultForward.vec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); //default forward is positive Z
	DefaultRight.vec   = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); //default right is positive X
	camForward.vec     = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	camRight.vec       = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	camUp.vec          = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //cam up end point is +1 Y
	camPosition.vec    = XMVectorSet(0.0f, 26, -32.0f, 0.0f); //initial poisition of camera
	camTarget.vec      = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); //our lookat target, origin

	camYaw = 0.0f;
	camPitch = 0.7f;

	moveBackForward = 0.0f;
	moveLeftRight = 0.0f;

	Matrix camRotationMatrix;
	camRotationMatrix.matrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget.vec = XMVector3TransformCoord(DefaultForward.vec, camRotationMatrix.matrix);
	camTarget.vec = XMVector3Normalize(camTarget.vec);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight.vec = XMVector3TransformCoord(DefaultRight.vec, RotateYTempMatrix);
	camUp.vec = XMVector3TransformCoord(camUp.vec, RotateYTempMatrix);
	camForward.vec = XMVector3TransformCoord(DefaultForward.vec, RotateYTempMatrix);

	camPosition.vec += moveLeftRight*camRight.vec;
	camPosition.vec += moveBackForward*camForward.vec;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget.vec = camPosition.vec + camTarget.vec;

	viewMat.matrix = XMMatrixLookAtLH(camPosition.vec, camTarget.vec, camUp.vec);
	//viewMat.matrix = XMMatrixTranspose(viewMat.matrix);

	//projMat.mat = XMMatrixOrthographicOffCenterLH(1920.0f, 1080.0f, 0.0f, 0.0f, 0.1f, 100.0f);
	//projMat.mat = XMMatrixTranspose(projMat.mat);

	projMat.matrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1920.0f / 1080.0f, 0.1f, 100.0f);
	//projMat.matrix = XMMatrixTranspose(projMat.matrix);

	std::vector<Subset> subsets;
	viewCB = graphics.CreateBuffer(HBS_GRAPHICS::BufferType::MATRIX, HBS_GRAPHICS::BufferUse::CONSTANT, subsets);
	projCB = graphics.CreateBuffer(HBS_GRAPHICS::BufferType::MATRIX, HBS_GRAPHICS::BufferUse::CONSTANT, subsets);
}

Camera::~Camera()
{
	projCB.~Buffer();
	viewCB.~Buffer();
}

void Camera::updateCameraLocation(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	camPosition.vec = XMVectorSet(posX, posY, posZ, 0.0f); //initial poisition of camera
	camTarget.vec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); //our lookat target, origin

	camYaw = 0.0f;
	camPitch = 0.7f;

	moveBackForward = 0.0f;
	moveLeftRight = 0.0f;

	Matrix camRotationMatrix;
	camRotationMatrix.matrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget.vec = XMVector3TransformCoord(DefaultForward.vec, camRotationMatrix.matrix);
	camTarget.vec = XMVector3Normalize(camTarget.vec);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight.vec = XMVector3TransformCoord(DefaultRight.vec, RotateYTempMatrix);
	camUp.vec = XMVector3TransformCoord(camUp.vec, RotateYTempMatrix);
	camForward.vec = XMVector3TransformCoord(DefaultForward.vec, RotateYTempMatrix);

	camPosition.vec += moveLeftRight*camRight.vec;
	camPosition.vec += moveBackForward*camForward.vec;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget.vec = camPosition.vec + camTarget.vec;

	viewMat.matrix = XMMatrixLookAtLH(camPosition.vec, camTarget.vec, camUp.vec);

	//projMat.mat = XMMatrixOrthographicOffCenterLH(1920.0f, 1080.0f, 0.0f, 0.0f, 0.1f, 100.0f);
	//projMat.mat = XMMatrixTranspose(projMat.mat);

	projMat.matrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1920.0f / 1080.0f, 0.1f, 100.0f);
}

const Matrix Camera::getViewProj() const
{
	Matrix vp;
	Matrix P;
	P.matrix = DirectX::XMMatrixTranspose(projMat.matrix);
	Matrix V;
	V.matrix = DirectX::XMMatrixTranspose(viewMat.matrix);

	vp.matrix = P.matrix * V.matrix;
	return vp;
}

void Camera::pickRayVector(float mouseX, float mouseY, HBS::HBS_MATH::Vector& pickRayInWorldSpacePos, HBS::HBS_MATH::Vector& pickRayInWorldSpaceDir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT4X4 m1;
	float PRVecX, PRVecY, PRVecZ;

	XMStoreFloat4x4(&m1, projMat.matrix);

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (((2.0f * mouseX) /  1920) - 1) / m1(0, 0);
	PRVecY = -(((2.0f * mouseY) / 1080) - 1) / m1(1, 1);
	PRVecZ = 1.0f;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, viewMat.matrix);    //Inverse of View Space matrix is World space matrix

	pickRayInWorldSpacePos.vec = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir.vec = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}

triangleStructure Camera::pick(HBS::HBS_MATH::Vector& pickRayInWorldSpacePos, HBS::HBS_MATH::Vector& pickRayInWorldSpaceDir, const std::vector<HBS_PB::Subset>& mesh, Matrix& worldSpace)
{
	//Loop through each triangle in the object
	for (int i = 0; i < mesh.size(); i++)
	{ 
		for (int j = 0; j < mesh.at(i).indices.size()/3; j++)
		{
			//Triangle's vertices V1, V2, V3
			XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//Temporary 3d floats for each vertex
			HBS_PB::Vertex tV1, tV2, tV3;

			//Get triangle 
			tV1 = mesh.at(i).vertices[mesh.at(i).indices[(j * 3) + 0]];
			tV2 = mesh.at(i).vertices[mesh.at(i).indices[(j * 3) + 1]];
			tV3 = mesh.at(i).vertices[mesh.at(i).indices[(j * 3) + 2]];

			tri1V1 = XMVectorSet(tV1.pos.x, tV1.pos.y, tV1.pos.z, 0.0f);
			tri1V2 = XMVectorSet(tV2.pos.x, tV2.pos.y, tV2.pos.z, 0.0f);
			tri1V3 = XMVectorSet(tV3.pos.x, tV3.pos.y, tV3.pos.z, 0.0f);

			//Transform the vertices to world space
			tri1V1 = XMVector3TransformCoord(tri1V1, worldSpace.matrix);
			tri1V2 = XMVector3TransformCoord(tri1V2, worldSpace.matrix);
			tri1V3 = XMVector3TransformCoord(tri1V3, worldSpace.matrix);

			//Find the normal using U, V coordinates (two edges)
			XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			U = tri1V2 - tri1V1;
			V = tri1V3 - tri1V1;

			//Compute face normal by crossing U, V
			faceNormal = XMVector3Cross(U, V);
			faceNormal = XMVector3Normalize(faceNormal);

			//Calculate a point on the triangle for the plane equation
			XMVECTOR triPoint = tri1V1;

			//Get plane equation ("Ax + By + Cz + D = 0") Variables
			float tri1A = XMVectorGetX(faceNormal);
			float tri1B = XMVectorGetY(faceNormal);
			float tri1C = XMVectorGetZ(faceNormal);
			float tri1D = (-tri1A*XMVectorGetX(triPoint) - tri1B*XMVectorGetY(triPoint) - tri1C*XMVectorGetZ(triPoint));

			//Now we find where (on the ray) the ray intersects with the triangles plane
			float ep1, ep2, t = 0.0f;
			float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
			XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			ep1 = (XMVectorGetX(pickRayInWorldSpacePos.vec) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos.vec) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos.vec) * tri1C);
			ep2 = (XMVectorGetX(pickRayInWorldSpaceDir.vec) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir.vec) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir.vec) * tri1C);

			//Make sure there are no divide-by-zeros
			if (ep2 != 0.0f)
				t = -(ep1 + tri1D) / (ep2);

			if (t > 0.0f)    //Make sure you don't pick objects behind the camera
			{
				//Get the point on the plane
				planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos.vec) + XMVectorGetX(pickRayInWorldSpaceDir.vec) * t;
				planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos.vec) + XMVectorGetY(pickRayInWorldSpaceDir.vec) * t;
				planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos.vec) + XMVectorGetZ(pickRayInWorldSpaceDir.vec) * t;

				pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

				//Call function to check if point is in the triangle
				if (PointInTriangle(HBS::HBS_MATH::Vector(tri1V1), HBS::HBS_MATH::Vector(tri1V2), HBS::HBS_MATH::Vector(tri1V3), HBS::HBS_MATH::Vector(pointInPlane)))
				{
					//Return the distance to the hit, so you can check all the other pickable objects in your scene
					//and choose whichever object is closest to the camera
					triangleStructure ts(tri1A, tri1B, tri1C, t / 2.0f);
					return ts;
				}
			}
		}
	}
	//return the max float value (near infinity) if an object was not picked
	triangleStructure ts(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	return ts;
}

bool Camera::PointInTriangle(HBS::HBS_MATH::Vector& triV1, HBS::HBS_MATH::Vector& triV2, HBS::HBS_MATH::Vector& triV3, HBS::HBS_MATH::Vector& point)
{
	XMVECTOR cp1 = XMVector3Cross((triV3.vec - triV2.vec), (point.vec - triV2.vec));
	XMVECTOR cp2 = XMVector3Cross((triV3.vec - triV2.vec), (triV1.vec - triV2.vec));
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((triV3.vec - triV1.vec), (point.vec - triV1.vec));
		cp2 = XMVector3Cross((triV3.vec - triV1.vec), (triV2.vec - triV1.vec));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV2.vec - triV1.vec), (point.vec - triV1.vec));
			cp2 = XMVector3Cross((triV2.vec - triV1.vec), (triV3.vec - triV1.vec));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}