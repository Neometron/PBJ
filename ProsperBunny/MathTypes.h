#pragma once
#ifdef WIN32
#include <DirectXMath.h>
using namespace DirectX;
#endif

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;

namespace HBS
{
	namespace HBS_MATH
	{
		struct Vector
		{

			Vector()
			{}

#ifdef WIN32
			XMVECTOR vec;
			Vector(XMVECTOR v)
			{
				vec = v;
			}
			Vector(float x, float y, float z, float w)
			{
				vec = XMVectorSet(x, y, z, w);
			}
			Vector operator-(Vector& op2)
			{
				return this->vec - op2.vec;
			}
#endif
		};

		inline bool areFloatsEqual(float a, float b, float epsilon) { return (fabs(a - b) <= epsilon * max(1.0f, max(a, b))); }
	}
}