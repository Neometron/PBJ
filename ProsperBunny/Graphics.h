#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Window.h"
#include "GraphicsTypes.h"
#include "Buffer.h"

namespace HBS_GRAPHICS
{
	enum BufferType { VERTEX, INDICE, MATRIX, DIRECTIONAL_LIGHT };
	enum BufferUse  { CONSTANT, DYNAMIC, STATIC };
	class Graphics
	{
	public:
		Graphics(HBS_WINDOW::Handle, int width, int height);
		~Graphics();
	public:
		void            ClearBackbuffer();
		Buffer          CreateBuffer(BufferType, BufferUse, std::vector<HBS_PB::Subset>);
		HBS_PB::Texture CreateImageTexture(const char*);
		void            Draw(HBS_PB::Matrix&, std::vector<HBS_PB::Subset>, DirectX::XMMATRIX& worldMat, const Buffer, const Buffer);
		void            PresentBackbuffer();
		bool            ndcRender;
		void            TurnZBufferOn();
		void            TurnZBufferOff();
	private:
		void CompileShaders();
		Buffer cbWorld, cbVp, cbEPos;
		Buffer dirLightcb;
		HBS_PB::DirectionalLight dirLight;
		HBS_PB::Texture placeHolderTexture;
#ifdef WIN32
		ID3D11Device           * d3d11_device;
		
		ID3D11DeviceContext    * d3d11_deviceContext;
		IDXGISwapChain         * dxgi_swapChain;

		ID3D11RenderTargetView * d3d11_rtv;
		ID3D11DepthStencilView * d3d11_dsv;

		ID3D11RasterizerState  * noCull;
		ID3D11SamplerState     * colorSampler;

		ID3D11VertexShader     * vsShader, *olVS, *ndcVS;
		ID3D11PixelShader      * psShader, *olPS, *ndcPS;

		ID3D11InputLayout      * inputL;
		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11DepthStencilState* m_depthStencilState;
#endif
	};
};

#endif