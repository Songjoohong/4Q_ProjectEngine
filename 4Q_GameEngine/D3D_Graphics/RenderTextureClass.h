#pragma once

class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
private:
	ID3D11Texture2D* m_renderTargetTexture = nullptr;;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;;
	ID3D11ShaderResourceView* m_shaderResourceView = nullptr;;
};
