#pragma once
#include<Windows.h>
#include<tchar.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include"d3dx12.h"
#include<wrl.h>
#include<memory>

class Dx12Wrapper;
class PMDRenderer;
class PMDActor;
///シングルトンクラス
class Application
{
public:
	///Applicationのシングルトンインスタンスを得る
	static Application& Instance();
	~Application();

	///初期化
	bool Begin();
	///ループ起動
	void Tick();

	///後処理
	void End();
	SIZE GetWindowSize()const;
private:
	//ウィンドウ周り
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	std::shared_ptr<Dx12Wrapper> _wrapper;
	std::shared_ptr<PMDRenderer> _pmdRenderer;
	std::shared_ptr<PMDActor> _pmdActor;

	//ゲーム用ウィンドウの生成
	void CreateGameWindow(HWND &hwnd, WNDCLASSEX &windowClass);

	//↓シングルトンのためにコンストラクタをprivateに
	//さらにコピーと代入を禁止に
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
};

