#include "Render/RenderSystem.h"
#include "Core/Window.h"
#include "Math/Vector4f.h"
#include "Externals/DDSTextureLoader.h"
#include <cwchar>
#include <cstring>
#include <sstream>

namespace
{
	HRESULT CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob);
}

RenderSystem::RenderSystem(std::shared_ptr<Window> window)
	: m_Window(window)
{
	const RECT backBufferRect = m_Window->GetClientRect();

	mBackBufferWidth = backBufferRect.right - backBufferRect.left;
	mBackBufferHeight = backBufferRect.bottom - backBufferRect.top;

	InitD3DSystem();
	InitD2DSystem();
}

RenderSystem::~RenderSystem()
{
	m_D3DDeviceContext->ClearState();
}

ComPtr<ID3D11Resource> RenderSystem::LoadTexture(LPCWSTR pFileName, ComPtr<ID3D11ShaderResourceView>* pSRV)
{
	ComPtr<ID3D11Resource> texture;
	HRESULT result = DirectX::CreateDDSTextureFromFile(m_D3DDevice.Get(), pFileName, texture.GetAddressOf(), (pSRV != nullptr) ? pSRV->GetAddressOf() : nullptr);
	assert(SUCCEEDED(result));
	return texture;
}

ComPtr<ID3D11VertexShader> RenderSystem::LoadVertexShader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel)
{
	ComPtr<ID3DBlob> bytecodeBlob;
	HRESULT result = CompileShaderFromFile(pFileName, nullptr, pEntryPoint, pShaderModel, &bytecodeBlob);
	assert(SUCCEEDED(result));

	ComPtr<ID3D11VertexShader> shader;
	result = m_D3DDevice->CreateVertexShader(bytecodeBlob->GetBufferPointer(), bytecodeBlob->GetBufferSize(), NULL, &shader);
	assert(SUCCEEDED(result));

	return shader;
}

ComPtr<ID3D11PixelShader> RenderSystem::LoadPixelShader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel)
{
	ComPtr<ID3DBlob> bytecodeBlob;
	HRESULT result = CompileShaderFromFile(pFileName, nullptr, pEntryPoint, pShaderModel, &bytecodeBlob);
	assert(SUCCEEDED(result));

	ComPtr<ID3D11PixelShader> shader;
	result = m_D3DDevice->CreatePixelShader(bytecodeBlob->GetBufferPointer(), bytecodeBlob->GetBufferSize(), NULL, &shader);
	assert(SUCCEEDED(result));

	return shader;
}

ComPtr<ID3D11Buffer> RenderSystem::CreateConstantBuffer(UINT sizeInBytes, D3D11_USAGE usage, UINT CPUAccessFlags, const void* pInitialData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeInBytes;
	bufferDesc.Usage = usage;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = CPUAccessFlags;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
		
	D3D11_SUBRESOURCE_DATA resourceData;
	resourceData.pSysMem = pInitialData;
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;
	
	ComPtr<ID3D11Buffer> buffer;
	HRESULT result = m_D3DDevice->CreateBuffer(&bufferDesc, (pInitialData != nullptr) ? &resourceData : nullptr, &buffer);
	assert(SUCCEEDED(result));

	return buffer;
}

ComPtr<ID3D11Buffer> RenderSystem::CreateStructuredBuffer(UINT numElements, UINT strideInBytes, D3D11_USAGE usage, UINT CPUAccessFlags,
	const void* pInitialData, ComPtr<ID3D11ShaderResourceView>* pSRV, ComPtr<ID3D11UnorderedAccessView>* pUAV)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = numElements * strideInBytes;
	bufferDesc.Usage = usage;
	
	bufferDesc.BindFlags = 0;
	if (pSRV != nullptr)
		bufferDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (pUAV != nullptr)
		bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

	bufferDesc.CPUAccessFlags = CPUAccessFlags;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = strideInBytes;

	D3D11_SUBRESOURCE_DATA resourceData;
	resourceData.pSysMem = pInitialData;
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> buffer;
	HRESULT result = m_D3DDevice->CreateBuffer(&bufferDesc, (pInitialData != nullptr) ? &resourceData : nullptr, &buffer);
	assert(SUCCEEDED(result));

	if (pSRV != nullptr)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = DXGI_FORMAT_UNKNOWN;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		viewDesc.Buffer.FirstElement = 0;
		viewDesc.Buffer.NumElements = numElements;

		result = m_D3DDevice->CreateShaderResourceView(buffer.Get(), &viewDesc, &*pSRV);
		assert(SUCCEEDED(result));
	}
	if (pUAV != nullptr)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
		viewDesc.Format = DXGI_FORMAT_UNKNOWN;
		viewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		viewDesc.Buffer.FirstElement = 0;
		viewDesc.Buffer.NumElements = numElements;
		viewDesc.Buffer.Flags = 0;

		result = m_D3DDevice->CreateUnorderedAccessView(buffer.Get(), &viewDesc, &*pUAV);
		assert(SUCCEEDED(result));
	}
	return buffer;
}

ComPtr<ID3D11SamplerState> RenderSystem::CreateLinearSampler()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	ComPtr<ID3D11SamplerState> sampler;
	HRESULT result = m_D3DDevice->CreateSamplerState(&samplerDesc, &sampler);
	assert(SUCCEEDED(result));
	return sampler;
}

ComPtr<ID2D1SolidColorBrush> RenderSystem::CreateTextColorBrush(const Vector4f& color)
{
	ComPtr<ID2D1SolidColorBrush> textColorBrush;
	HRESULT result = m_D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color.m_X, color.m_Y, color.m_Z, color.m_W), &textColorBrush);
	assert(SUCCEEDED(result));

	return textColorBrush;
}

ComPtr<IDWriteTextFormat> RenderSystem::CreateTextFormat(LPCWSTR pFontFamilyName, FLOAT fontSize, DWRITE_FONT_STYLE fontStyle)
{
	ComPtr<IDWriteTextFormat> textFormat;

	HRESULT result = m_DWriteFactory->CreateTextFormat(pFontFamilyName, nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		fontStyle, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);
	assert(SUCCEEDED(result));

	return textFormat;
}

void RenderSystem::WriteToBuffer(ComPtr<ID3D11Buffer> buffer, const void* pData, UINT sizeInBytes)
{
	static const UINT subresourceIndex = 0;
	static const UINT mapFlags = 0;

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT result = m_D3DDeviceContext->Map(buffer.Get(), subresourceIndex, D3D11_MAP_WRITE_DISCARD, mapFlags, &mappedSubresource);
	assert(SUCCEEDED(result));

	std::memcpy(mappedSubresource.pData, pData, sizeInBytes);

	m_D3DDeviceContext->Unmap(buffer.Get(), subresourceIndex);
}

void RenderSystem::DrawText(LPCWSTR pText, ComPtr<IDWriteTextFormat> textFormat, ComPtr<ID2D1SolidColorBrush> textColorBrush,
	FLOAT textRectLeft, FLOAT textRectTop, FLOAT textRectRight, FLOAT textRectBottom)
{
	m_D2DRenderTarget->DrawText(pText, std::wcslen(pText), textFormat.Get(),
		D2D1::RectF(textRectLeft, textRectTop, textRectRight, textRectBottom), textColorBrush.Get());
}

void RenderSystem::BeginRenderFrame()
{
	m_D2DRenderTarget->BeginDraw();
}

void RenderSystem::EndRenderFrame()
{
	m_D2DRenderTarget->EndDraw();
	m_DXGISwapChain->Present(0, 0);
}

void RenderSystem::ClearRenderTarget(const Vector4f& clearColor)
{
	m_D3DDeviceContext->ClearRenderTargetView(m_D3DBackBufferRTV.Get(), &clearColor.m_X);
	m_D3DDeviceContext->OMSetRenderTargets(1, m_D3DBackBufferRTV.GetAddressOf(), nullptr);
}

void RenderSystem::SetInputLayout(ComPtr<ID3D11InputLayout> inputLayout)
{
	m_D3DDeviceContext->IASetInputLayout(inputLayout.Get());
}

void RenderSystem::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_D3DDeviceContext->IASetPrimitiveTopology(topology);
}

void RenderSystem::SetVertexBuffer(ComPtr<ID3D11Buffer> vertexBuffer, UINT vertexStrideInBytes, UINT firstVertexOffsetInBytes)
{
	m_D3DDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStrideInBytes, &firstVertexOffsetInBytes);
}

void RenderSystem::SetIndexBuffer(ComPtr<ID3D11Buffer> indexBuffer, bool isIndex16Bit, UINT firstIndexOffsetInBytes)
{
	m_D3DDeviceContext->IASetIndexBuffer(indexBuffer.Get(), isIndex16Bit ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, firstIndexOffsetInBytes);
}

void RenderSystem::SetVertexShader(ComPtr<ID3D11VertexShader> shader)
{
	m_D3DDeviceContext->VSSetShader(shader.Get(), nullptr, 0);
}

void RenderSystem::SetVertexShaderConstantBuffer(UINT slot, ComPtr<ID3D11Buffer> buffer)
{
	m_D3DDeviceContext->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void RenderSystem::SetVertexShaderResource(UINT slot, ComPtr<ID3D11ShaderResourceView> srv)
{
	m_D3DDeviceContext->VSSetShaderResources(slot, 1, srv.GetAddressOf());
}

void RenderSystem::SetPixelShader(ComPtr<ID3D11PixelShader> shader)
{
	m_D3DDeviceContext->PSSetShader(shader.Get(), nullptr, 0);
}

void RenderSystem::SetPixelShaderConstantBuffer(UINT slot, ComPtr<ID3D11Buffer> buffer)
{
	m_D3DDeviceContext->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void RenderSystem::SetPixelShaderResource(UINT slot, ComPtr<ID3D11ShaderResourceView> srv)
{
	m_D3DDeviceContext->PSSetShaderResources(slot, 1, srv.GetAddressOf());
}

void RenderSystem::SetPixelShaderSampler(UINT slot, ComPtr<ID3D11SamplerState> sampler)
{
	m_D3DDeviceContext->PSSetSamplers(slot, 1, sampler.GetAddressOf());
}

void RenderSystem::SetViewport(const D3D11_VIEWPORT* pViewport)
{
	m_D3DDeviceContext->RSSetViewports(1, pViewport);
}

void RenderSystem::DrawInstanced(UINT numVerticesPerInstance, UINT numInstances, UINT startVertexLocation, UINT startInstanceLocation)
{
	m_D3DDeviceContext->DrawInstanced(numVerticesPerInstance, numInstances, startVertexLocation, startInstanceLocation);
}

namespace
{
	HRESULT CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob)
	{
		UINT compileFlags = 0;
		compileFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
		compileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
		compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ComPtr<ID3DBlob> errorBlob;
		HRESULT result = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			pEntryPoint, pShaderModel, compileFlags, 0, ppBytecodeBlob, &errorBlob);

		if (FAILED(result))
			OutputDebugStringA((LPCSTR)errorBlob->GetBufferPointer());
		
		return result;
	}
}

void RenderSystem::InitD3DSystem()
{
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = mBackBufferWidth;
	swapChainDesc.BufferDesc.Height = mBackBufferHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_Window->GetHWND();
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&swapChainDesc, &m_DXGISwapChain, &m_D3DDevice, &m_D3DFeatureLevel, &m_D3DDeviceContext);
	assert(SUCCEEDED(result));

	ComPtr<ID3D11Texture2D> backBuffer;
	result = m_DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	assert(SUCCEEDED(result));

	result = m_D3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_D3DBackBufferRTV);
	assert(SUCCEEDED(result));
}

void RenderSystem::InitD2DSystem()
{
	D2D1_FACTORY_OPTIONS options;
#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &options, &m_D2DFactory);
	assert(SUCCEEDED(result));
		
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_DWriteFactory.GetAddressOf()));
	assert(SUCCEEDED(result));

	ComPtr<IDXGISurface> backBuffer;
	result = m_DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	assert(SUCCEEDED(result));

	float dpiX, dpiY;
	m_D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	renderTargetProperties.dpiX = dpiX;
	renderTargetProperties.dpiY = dpiY;
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	result = m_D2DFactory->CreateDxgiSurfaceRenderTarget(backBuffer.Get(), &renderTargetProperties, &m_D2DRenderTarget);
	assert(SUCCEEDED(result));
}