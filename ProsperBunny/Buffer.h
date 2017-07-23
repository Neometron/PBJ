#pragma once

#ifdef WIN32
#include <d3d11.h>
#endif

namespace HBS_GRAPHICS
{
	class Buffer
	{
	public:
		Buffer()  {};
		~Buffer() 
		{
//			if (buffer) buffer->Release();
		};
	public:
#ifdef WIN32
		ID3D11Buffer* buffer;
#endif
	};
};