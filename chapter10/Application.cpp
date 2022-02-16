/*--------------------------------------------------------------
	
	[Application.cpp]
	Author : �o���đ�

---------------------------------------------------------------*/
#include "Application.h"
#include"Graphics.h"
#include"Renderer.h"
#include"Actor.h"

//�E�B���h�E�萔
const unsigned int window_width = 1920;
const unsigned int window_height = 1080;

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	// �A�v���̏I��
	if (msg == WM_DESTROY) 
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�K��̏������s��
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

	//DirectX12���b�p�[������������
	_wrapper.reset(new Graphics(_hwnd));
	_renderer.reset(new Renderer(*_wrapper));
	_actor.reset(new Actor("Model/�����~�N.pmd", *_renderer));
	//_actor->LoadVMDFile("motion/���S�R���_���X.vmd", "pose");
	_actor->LoadVMDFile("motion/2�����[�v�X�e�b�v5.vmd", "pose");
	_wrapper->ExecuteCommand();
	_actor->PlayAnimation();
	return true;
}

void Application::Tick() 
{
	ShowWindow(_hwnd, SW_SHOW);//�E�B���h�E�\��
	MSG msg = {};

	while (true) 
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// �A�v���I��
		if (msg.message == WM_QUIT) {
			break;
		}

		//�S�̂̕`�揀��
		_wrapper->BeginDraw();

		//PMD�p�̕`��p�C�v���C���ɍ��킹��
		_wrapper->CommandList()->SetPipelineState(_renderer->GetPipelineState());
		//���[�g�V�O�l�`����PMD�p�ɍ��킹��
		_wrapper->CommandList()->SetGraphicsRootSignature(_renderer->GetRootSignature());

		_wrapper->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		_wrapper->SetScene();
		
		_actor->Update();
		_actor->Draw();

		_wrapper->EndDraw();

		//�t���b�v
		_wrapper->Swapchain()->Present(0, 0);
	}
}

void Application::End()
{
	//�����N���X�g��񂩂�o�^�������Ă�
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
	//�E�B���h�E�N���X�������o�^
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//�R�[���o�b�N�֐��̎w��
	windowClass.lpszClassName = _T("DirectXTest");//�A�v���P�[�V�����N���X��(�K���ł����ł�)
	windowClass.hInstance = GetModuleHandle(0);//�n���h���̎擾
	RegisterClassEx(&windowClass);//�A�v���P�[�V�����N���X(���������̍�邩���낵������OS�ɗ\������)

	RECT wrc = { 0,0, window_width, window_height };//�E�B���h�E�T�C�Y�����߂�
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�E�B���h�E�̃T�C�Y�͂�����Ɩʓ|�Ȃ̂Ŋ֐����g���ĕ␳����
	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(windowClass.lpszClassName,//�N���X���w��
		_T("DirectX12_���f���A�j���[�V����"),//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,//�^�C�g���o�[�Ƌ��E��������E�B���h�E�ł�
		CW_USEDEFAULT,//�\��X���W��OS�ɂ��C�����܂�
		CW_USEDEFAULT,//�\��Y���W��OS�ɂ��C�����܂�
		wrc.right - wrc.left,//�E�B���h�E��
		wrc.bottom - wrc.top,//�E�B���h�E��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		windowClass.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�ǉ��p�����[�^

}

Application::Application()
{
}
