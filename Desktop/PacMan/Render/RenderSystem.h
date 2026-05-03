#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d2d1.h>
#include <dwrite.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class Window;
struct Vector4f;

class RenderSystem
{
public:
	RenderSystem(std::shared_ptr<Window> window);

	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator= (const RenderSystem&) = delete;

	~RenderSystem();

	UINT GetBackBufferWidth() const { return mBackBufferWidth; }
	UINT GetBackBufferHeight() const { return mBackBufferHeight; }
	
	ComPtr<ID3D11Resource> LoadTexture(LPCWSTR pFileName, ComPtr<ID3D11ShaderResourceView>* pSRV);

	ComPtr<ID3D11VertexShader> LoadVertexShader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel);
	ComPtr<ID3D11PixelShader> LoadPixelShader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel);
	
	ComPtr<ID3D11Buffer> CreateConstantBuffer(UINT sizeInBytes, D3D11_USAGE usage, UINT CPUAccessFlags, const void* pInitialData);
	
	ComPtr<ID3D11Buffer> CreateStructuredBuffer(UINT numElements, UINT strideInBytes, D3D11_USAGE usage, UINT CPUAccessFlags,
		const void* pInitialData, ComPtr<ID3D11ShaderResourceView>* pSRV, ComPtr<ID3D11UnorderedAccessView>* pUAV);

	ComPtr<ID3D11SamplerState> CreateLinearSampler();

	ComPtr<ID2D1SolidColorBrush> CreateTextColorBrush(const Vector4f& color);
	ComPtr<IDWriteTextFormat> CreateTextFormat(LPCWSTR pFontFamilyName, FLOAT fontSize, DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL);

	void WriteToBuffer(ComPtr<ID3D11Buffer> buffer, const void* pData, UINT sizeInBytes);
	
	void DrawText(LPCWSTR pText, ComPtr<IDWriteTextFormat> textFormat, ComPtr<ID2D1SolidColorBrush> textColorBrush,
		FLOAT textRectLeft, FLOAT textRectTop, FLOAT textRectRight, FLOAT textRectBottom);

	void BeginRenderFrame();
	void EndRenderFrame();

	void ClearRenderTarget(const Vector4f& clearColor);
	
	void SetInputLayout(ComPtr<ID3D11InputLayout> inputLayout);
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	void SetVertexBuffer(ComPtr<ID3D11Buffer> vertexBuffer, UINT vertexStrideInBytes, UINT firstVertexOffsetInBytes);
	void SetIndexBuffer(ComPtr<ID3D11Buffer> indexBuffer, bool isIndex16Bit, UINT firstIndexOffsetInBytes);
	
	void SetVertexShader(ComPtr<ID3D11VertexShader> shader);
	void SetVertexShaderConstantBuffer(UINT slot, ComPtr<ID3D11Buffer> buffer);
	void SetVertexShaderResource(UINT slot, ComPtr<ID3D11ShaderResourceView> srv);

	void SetPixelShader(ComPtr<ID3D11PixelShader> shader);
	void SetPixelShaderConstantBuffer(UINT slot, ComPtr<ID3D11Buffer> buffer);
	void SetPixelShaderResource(UINT slot, ComPtr<ID3D11ShaderResourceView> srv);
	void SetPixelShaderSampler(UINT slot, ComPtr<ID3D11SamplerState> sampler);

	void SetViewport(const D3D11_VIEWPORT* pViewport);
	void DrawInstanced(UINT numVerticesPerInstance, UINT numInstances, UINT startVertexLocation, UINT startInstanceLocation);

private:
	void InitD3DSystem();
	void InitD2DSystem();

private:
	std::shared_ptr<Window> m_Window;
	UINT mBackBufferWidth;
	UINT mBackBufferHeight;
	ComPtr<ID3D11Device> m_D3DDevice;
	ComPtr<ID3D11DeviceContext> m_D3DDeviceContext;
	ComPtr<IDXGISwapChain> m_DXGISwapChain;
	ComPtr<ID3D11RenderTargetView> m_D3DBackBufferRTV;
	D3D_FEATURE_LEVEL m_D3DFeatureLevel;
	ComPtr<ID2D1Factory> m_D2DFactory;
	ComPtr<ID2D1RenderTarget> m_D2DRenderTarget;
	ComPtr<IDWriteFactory> m_DWriteFactory;
};