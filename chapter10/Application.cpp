/*--------------------------------------------------------------
	
	[Application.cpp]
	Author : 出合翔太

---------------------------------------------------------------*/
#include "Application.h"
#include"Graphics.h"
#include"Renderer.h"
#include"Actor.h"

//ウィンドウ定数
const unsigned int window_width = 1920;
const unsigned int window_height = 1080;

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	// アプリの終了
	if (msg == WM_DESTROY) 
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
}

Application& Application::Instance()
{
	static Application instance;
	return instance;
}

Application::~Application()
{
}

bool Application::Begin() 
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(_hwnd, _windowClass);

	//DirectX12ラッパー生成＆初期化
	_wrapper.reset(new Graphics(_hwnd));
	_renderer.reset(new Renderer(*_wrapper));
	_actor.reset(new Actor("Model/初音ミク.pmd", *_renderer));
	//_actor->LoadVMDFile("motion/ヤゴコロダンス.vmd", "pose");
	_actor->LoadVMDFile("motion/2分ループステップ5.vmd", "pose");
	_wrapper->ExecuteCommand();
	_actor->PlayAnimation();
	return true;
}

void Application::Tick() 
{
	ShowWindow(_hwnd, SW_SHOW);//ウィンドウ表示
	MSG msg = {};

	while (true) 
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// アプリ終了
		if (msg.message == WM_QUIT) {
			break;
		}

		//全体の描画準備
		_wrapper->BeginDraw();

		//PMD用の描画パイプラインに合わせる
		_wrapper->CommandList()->SetPipelineState(_renderer->GetPipelineState());
		//ルートシグネチャもPMD用に合わせる
		_wrapper->CommandList()->SetGraphicsRootSignature(_renderer->GetRootSignature());

		_wrapper->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		_wrapper->SetScene();
		
		_actor->Update();
		_actor->Draw();

		_wrapper->EndDraw();

		//フリップ
		_wrapper->Swapchain()->Present(0, 0);
	}
}

void Application::End()
{
	//もうクラス使わんから登録解除してや
	UnregisterClass(_windowClass.lpszClassName, _windowClass.hInstance);
}

SIZE Application::GetWindowSize()const
{
	SIZE ret;
	ret.cx = window_width;
	ret.cy = window_height;
	return ret;
}

void Application::CreateGameWindow(HWND &hwnd, WNDCLASSEX &windowClass)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);
	//ウィンドウクラス生成＆登録
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	windowClass.lpszClassName = _T("DirectXTest");//アプリケーションクラス名(適当でいいです)
	windowClass.hInstance = GetModuleHandle(0);//ハンドルの取得
	RegisterClassEx(&windowClass);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)

	RECT wrc = { 0,0, window_width, window_height };//ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(windowClass.lpszClassName,//クラス名指定
		_T("DirectX12_モデルアニメーション"),//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
		CW_USEDEFAULT,//表示X座標はOSにお任せします
		CW_USEDEFAULT,//表示Y座標はOSにお任せします
		wrc.right - wrc.left,//ウィンドウ幅
		wrc.bottom - wrc.top,//ウィンドウ高
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		windowClass.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//追加パラメータ

}

Application::Application()
{
}
