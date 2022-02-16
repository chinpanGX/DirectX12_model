/*--------------------------------------------------------------

	[Renderer.h]
	Author : 出合翔太

---------------------------------------------------------------*/
#pragma once
#include<d3d12.h>
#include<vector>
#include<wrl.h>
#include<memory>

class Graphics;
class Actor;
class Renderer
{
	friend Actor;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Renderer(Graphics& graphics);
	~Renderer();
	ID3D12PipelineState* GetPipelineState();
	ID3D12RootSignature* GetRootSignature();
private:
	Graphics& _wrapper;

	ComPtr< ID3D12PipelineState> _pipeline = nullptr;//PMD用パイプライン
	ComPtr< ID3D12RootSignature> _rootSignature = nullptr;//PMD用ルートシグネチャ

	//PMD用共通テクスチャ(白、黒、グレイスケールグラデーション)
	ComPtr<ID3D12Resource> _whiteTex = nullptr;
	ComPtr<ID3D12Resource> _blackTex = nullptr;
	ComPtr<ID3D12Resource> _gradTex = nullptr;

	ID3D12Resource* CreateDefaultTexture(size_t width,size_t height);
	ID3D12Resource* CreateWhiteTexture();//白テクスチャの生成
	ID3D12Resource*	CreateBlackTexture();//黒テクスチャの生成
	ID3D12Resource*	CreateGrayGradationTexture();//グレーテクスチャの生成

	//パイプライン初期化
	HRESULT CreateGraphicsPipelineForPMD();
	//ルートシグネチャ初期化
	HRESULT CreateRootSignature();

	bool CheckShaderCompileResult(HRESULT result , ID3DBlob* error=nullptr);
};

