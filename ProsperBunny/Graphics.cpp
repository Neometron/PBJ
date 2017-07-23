#include "Graphics.h"
#include "DDSTextureLoader.h"
#include <iterator>
using namespace HBS_GRAPHICS;

Graphics::Graphics(HBS_WINDOW::Handle handle, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = handle.hWnd;
	swapChainDesc.SampleDesc.Count = 8;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &dxgi_swapChain, &d3d11_device, NULL, &d3d11_deviceContext);

	ID3D11Texture2D* swapTexture;
	dxgi_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&swapTexture);
	d3d11_device->CreateRenderTargetView(swapTexture, 0, &d3d11_rtv);

	ID3D11Texture2D *backbufferTexture;
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textDesc.ArraySize = 1;
	textDesc.MipLevels = 1;
	textDesc.Height = height;
	textDesc.Width = width;
	textDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textDesc.SampleDesc.Count = 8;
	textDesc.SampleDesc.Quality = 0;
	textDesc.MiscFlags = 0;
	textDesc.CPUAccessFlags = 0;

	d3d11_device->CreateTexture2D(&textDesc, 0, &backbufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format		     = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.Texture2D.MipSlice = 0;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	
	d3d11_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	d3d11_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	d3d11_device->CreateDepthStencilView(backbufferTexture, &depthDesc, &d3d11_dsv);

	d3d11_deviceContext->OMSetRenderTargets(1, &d3d11_rtv, d3d11_dsv);
	D3D11_VIEWPORT view;
	ZeroMemory(&view, sizeof(D3D11_VIEWPORT));
	view.Height = height;
	view.Width = width;
	view.MinDepth = 0.1f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;

	d3d11_deviceContext->RSSetViewports(1, &view);

	std::vector<HBS_PB::Subset> emptySubsetList;
	cbWorld    = CreateBuffer(MATRIX, CONSTANT, emptySubsetList);
	cbVp       = CreateBuffer(MATRIX, CONSTANT, emptySubsetList);
	dirLightcb = CreateBuffer(DIRECTIONAL_LIGHT, CONSTANT, emptySubsetList);

	ZeroMemory(&dirLight, sizeof(HBS_PB::DirectionalLight));
	dirLight.direction = DirectX::XMFLOAT4(1.0f, 0.14f, 0.0f, 0.0f);
	dirLight.diffuse  = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	dirLight.ambient  = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f);

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.CullMode = D3D11_CULL_BACK;

	d3d11_device->CreateRasterizerState(&rastDesc, &noCull);

	D3D11_BUFFER_DESC ePosBuffer;
	ZeroMemory(&ePosBuffer, sizeof(D3D11_BUFFER_DESC));
	ePosBuffer.ByteWidth = sizeof(HBS_PB::EyePosition);
	ePosBuffer.Usage = D3D11_USAGE_DEFAULT;
	ePosBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	d3d11_device->CreateBuffer(&ePosBuffer, 0, &cbEPos.buffer);
	colorSampler = 0;
/*
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FrontCounterClockwise = false;
	d3d11_device->CreateRasterizerState(&cmdesc, &noCull);

	tex = CreateImageTexture("wood.dds");*/
	CompileShaders();
	placeHolderTexture = CreateImageTexture("bunnySkin.dds");

	ndcRender = false;
}


Graphics::~Graphics()
{
	if (d3d11_device)                d3d11_device->Release();
	if (d3d11_deviceContext)         d3d11_deviceContext->Release();
	if (d3d11_dsv)                   d3d11_dsv->Release();
	if (dxgi_swapChain)              dxgi_swapChain->Release();
							         
	if (noCull)                      noCull->Release();
	if (vsShader)                    vsShader->Release();
	if (olVS)                        olVS->Release();
	if (ndcVS)                       ndcVS->Release();
	if (psShader)                    psShader->Release();
	if (olPS)                        olPS->Release();
	if (ndcPS)                       ndcPS->Release();
	if (colorSampler)                colorSampler->Release();

	if (inputL)                      inputL->Release();
	if (m_depthDisabledStencilState) m_depthDisabledStencilState->Release();
	if (m_depthStencilState)         m_depthStencilState->Release();

	if (cbWorld.buffer)              cbWorld.buffer->Release();
	if (cbVp.buffer)                 cbVp.buffer->Release();
	if (cbEPos.buffer)               cbEPos.buffer->Release();
	if (dirLightcb.buffer)           dirLightcb.buffer->Release();
}

Buffer Graphics::CreateBuffer(BufferType bt, BufferUse bu, std::vector<HBS_PB::Subset> mesh)
{
	HRESULT           hr;
	D3D11_BUFFER_DESC buffDesc;
	D3D11_SUBRESOURCE_DATA resource;
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&resource, sizeof(D3D11_SUBRESOURCE_DATA));

	if      (bu == STATIC)   buffDesc.Usage = D3D11_USAGE_DEFAULT;
	else if (bu == CONSTANT) buffDesc.Usage = D3D11_USAGE_DEFAULT;
	else                     buffDesc.Usage = D3D11_USAGE_DYNAMIC;

	std::vector<HBS_PB::Vertex> vertices;
	std::vector<int> indices;

	if (bt == VERTEX)
	{
		buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		int byteWidth = 0;

		for (int i = 0; i < mesh.size(); i++)
			for (int j = 0; j < mesh.at(i).vertices.size(); j++)
				vertices.push_back(mesh.at(i).vertices.at(j));

		buffDesc.ByteWidth = sizeof(HBS_PB::Vertex) * vertices.size();
		resource.pSysMem   = &vertices.at(0);
	}
	else if (bt == INDICE)
	{ 
		buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		int byteWidth = 0;
		for (int i = 0; i < mesh.size(); i++)
			for (int j = 0; j < mesh.at(i).indices.size(); j++)
				indices.push_back(mesh.at(i).indices.at(j));

		buffDesc.ByteWidth = sizeof(unsigned int) * indices.size();
		resource.pSysMem = &indices.at(0);
	}
	else if (bt == MATRIX)
	{
		buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	}
	else if (bt == DIRECTIONAL_LIGHT)
	{
		buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDesc.ByteWidth = sizeof(HBS_PB::DirectionalLight);
	}

	Buffer buffer;
	if (bu != CONSTANT) hr = d3d11_device->CreateBuffer(&buffDesc, &resource, &buffer.buffer);
	else                hr = d3d11_device->CreateBuffer(&buffDesc, 0,         &buffer.buffer);

	if (FAILED(hr)) throw std::exception("Buffer Creation Failure", 230);
	return buffer;
}

void Graphics::CompileShaders()
{
	ID3DBlob *code;

	D3DCompileFromFile(L"PBMainVS.hlsl", NULL, NULL, "vsMain", "vs_4_0", 0, 0, &code, NULL);
	d3d11_device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &vsShader);

	D3D11_INPUT_ELEMENT_DESC inputE[] =
	{
		{"POSITION", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"TEXCOORD", NULL, DXGI_FORMAT_R32G32_FLOAT,       0,16, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"NORMAL"  , NULL, DXGI_FORMAT_R32G32B32_FLOAT,    0,24, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"COLOR"   , NULL, DXGI_FORMAT_R32G32B32_FLOAT,    0,36, D3D11_INPUT_PER_VERTEX_DATA, NULL},
	};

	d3d11_device->CreateInputLayout(inputE, 4, code->GetBufferPointer(), code->GetBufferSize(), &inputL);

	D3DCompileFromFile(L"outlineVS.hlsl", NULL, NULL, "vsMain", "vs_4_0", 0, 0, &code, NULL);
	d3d11_device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &olVS);

	D3DCompileFromFile(L"outlinePS.hlsl", NULL, NULL, "psMain", "ps_4_0", 0, 0, &code, NULL);
	d3d11_device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &olPS);

	D3DCompileFromFile(L"ndcVS.hlsl", NULL, NULL, "vsMain", "vs_4_0", 0, 0, &code, NULL);
	d3d11_device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &ndcVS);

	D3DCompileFromFile(L"ndcPS.hlsl", NULL, NULL, "psMain", "ps_4_0", 0, 0, &code, NULL);
	d3d11_device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &ndcPS);

	D3DCompileFromFile(L"PBMainPS.hlsl", NULL, NULL, "psMain", "ps_4_0", 0, 0, &code, NULL);
	d3d11_device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, &psShader);

	if (code) code->Release();
}

HBS_PB::Texture Graphics::CreateImageTexture(const char* imagename)
{
	HBS_PB::Texture txt;
	ID3D11Resource     *rsr;

	std::wstring w;
	std::copy(imagename, imagename + strlen(imagename), std::back_inserter(w));
	const WCHAR *pwcsName = w.c_str();

	DirectX::CreateDDSTextureFromFile(d3d11_device, pwcsName, &rsr, &txt.srv, 0, NULL);

	return txt;
}

void Graphics::ClearBackbuffer()
{
	float clearColor[] = { 0.4, 0.6f, 0.8f, 0.0f };
	d3d11_deviceContext->ClearRenderTargetView(d3d11_rtv, clearColor);
	d3d11_deviceContext->ClearDepthStencilView(d3d11_dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::Draw(HBS_PB::Matrix& mat, std::vector<HBS_PB::Subset> subsets, DirectX::XMMATRIX& worldMat, const Buffer vb, const Buffer ib)
{
	unsigned int stride    = sizeof(HBS_PB::Vertex);
	unsigned int indiceLoc = 0;
	unsigned int vOffset = 0;
	unsigned int iOffset = 0;
	
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranspose(worldMat);
	HBS_PB::EyePosition eyePos;
	eyePos.x = 0.0f;
	eyePos.y = -0.875f;
	eyePos.z = 1.0f;
	eyePos.w = 0.0f;
	/*
	d3d11_deviceContext->RSSetState(noCull);
	d3d11_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d11_deviceContext->IASetInputLayout(inputL);

	d3d11_deviceContext->VSSetShader(olVS, 0, 0);
	d3d11_deviceContext->PSSetShader(olPS, 0, 0);

	d3d11_deviceContext->VSSetConstantBuffers(0, 1, &cbWorld.buffer);
	d3d11_deviceContext->VSSetConstantBuffers(1, 1, &cbVp.buffer);

	for (int i = 0; i < subsets.size(); i++)
	{
		d3d11_deviceContext->IASetIndexBuffer(ib.buffer, DXGI_FORMAT_R32_UINT, iOffset);
		d3d11_deviceContext->IASetVertexBuffers(0, 1, &vb.buffer, &stride, &vOffset);
		d3d11_deviceContext->UpdateSubresource(cbWorld.buffer, NULL, NULL, &worldMatrix, 0, 0);
		d3d11_deviceContext->UpdateSubresource(cbVp.buffer, NULL, NULL, &mat.matrix, 0, 0);

		d3d11_deviceContext->DrawIndexed(subsets.at(i).indices.size(), indiceLoc, 0);

		vOffset += sizeof(HBS_PB::Vertex) * subsets.at(i).vertices.size();
		iOffset += sizeof(unsigned int)   * subsets.at(i).indices.size();
	}

	vOffset = 0;
	iOffset = 0;*/

	//d3d11_deviceContext->RSSetState(0);
	d3d11_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d11_deviceContext->IASetInputLayout(inputL);

	if (!ndcRender)
	{
		d3d11_deviceContext->VSSetShader(vsShader, 0, 0);
		d3d11_deviceContext->PSSetShader(psShader, 0, 0);

		d3d11_deviceContext->VSSetConstantBuffers(0, 1, &cbWorld.buffer);
		d3d11_deviceContext->VSSetConstantBuffers(1, 1, &cbVp.buffer);
		d3d11_deviceContext->PSSetConstantBuffers(0, 1, &dirLightcb.buffer);
		d3d11_deviceContext->PSSetConstantBuffers(1, 1, &cbEPos.buffer);
		d3d11_deviceContext->PSSetSamplers(0, 1, &colorSampler);

		for (int i = 0; i < subsets.size(); i++)
		{
			if (i == 10)
			{
				eyePos.angle = 0.05f;
			}
			else if (i == 27 || i == 30 || i == 31 || i == 35)
			{
				eyePos.angle = -1.0f;
			}
			else
			{
				eyePos.angle = 0.48f;
			}
			d3d11_deviceContext->IASetIndexBuffer(ib.buffer, DXGI_FORMAT_R32_UINT, iOffset);
			d3d11_deviceContext->IASetVertexBuffers(0, 1, &vb.buffer, &stride, &vOffset);

			if (subsets.at(i).tex.srv != nullptr) d3d11_deviceContext->PSSetShaderResources(0, 1, &subsets.at(i).tex.srv);
			else                                  d3d11_deviceContext->PSSetShaderResources(0, 1, &placeHolderTexture.srv);

			d3d11_deviceContext->UpdateSubresource(cbWorld.buffer, NULL, NULL, &worldMatrix, 0, 0);
			d3d11_deviceContext->UpdateSubresource(cbVp.buffer, NULL, NULL, &mat.matrix, 0, 0);
			d3d11_deviceContext->UpdateSubresource(dirLightcb.buffer, NULL, NULL, &dirLight, 0, 0);
			d3d11_deviceContext->UpdateSubresource(cbEPos.buffer, NULL, NULL, &eyePos, 0, 0);

			d3d11_deviceContext->DrawIndexed(subsets.at(i).indices.size(), indiceLoc, 0);

			vOffset += sizeof(HBS_PB::Vertex) * subsets.at(i).vertices.size();
			iOffset += sizeof(unsigned int)   * subsets.at(i).indices.size();
		}
	}
	else
	{
		d3d11_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);

		d3d11_deviceContext->VSSetShader(ndcVS, 0, 0);
		d3d11_deviceContext->PSSetShader(ndcPS, 0, 0);
		d3d11_deviceContext->PSSetSamplers(0, 1, &colorSampler);

		d3d11_deviceContext->VSSetConstantBuffers(0, 1, &cbWorld.buffer);
		d3d11_deviceContext->VSSetConstantBuffers(1, 1, &cbVp.buffer);

		for (int i = 0; i < subsets.size(); i++)
		{
			d3d11_deviceContext->IASetIndexBuffer(ib.buffer, DXGI_FORMAT_R32_UINT, iOffset);
			d3d11_deviceContext->IASetVertexBuffers(0, 1, &vb.buffer, &stride, &vOffset);
			d3d11_deviceContext->PSSetShaderResources(0, 1, &subsets.at(i).tex.srv);

			d3d11_deviceContext->UpdateSubresource(cbWorld.buffer, NULL, NULL, &worldMatrix, 0, 0);
			d3d11_deviceContext->UpdateSubresource(cbVp.buffer, NULL, NULL, &mat.matrix, 0, 0);

			d3d11_deviceContext->DrawIndexed(subsets.at(i).indices.size(), indiceLoc, 0);

			vOffset += sizeof(HBS_PB::Vertex) * subsets.at(i).vertices.size();
			iOffset += sizeof(unsigned int)   * subsets.at(i).indices.size();
		}

		d3d11_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}
}

void Graphics::PresentBackbuffer()
{
	dxgi_swapChain->Present(0, 0);
}