/*--------------------------------------------------------------
	
	[Application.h]
	Author : �o���đ�

---------------------------------------------------------------*/
#pragma once
#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "d3dx12.h"
#include <wrl.h>
#include <memory>

class Graphics;
class Renderer;
class Actor;
///�V���O���g���N���X
class Application
{
public:
	///Application�̃V���O���g���C���X�^���X�𓾂�
	static Application& Instance();
	~Application();

	///������
	bool Begin();
	///���[�v�N��
	void Tick();

	///�㏈��
	void End();
	SIZE GetWindowSize()const;
private:
	//�E�B���h�E����
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	std::shared_ptr<Graphics> _wrapper;
	std::shared_ptr<Renderer> _renderer;
	std::shared_ptr<Actor> _actor;

	//�Q�[���p�E�B���h�E�̐���
	void CreateGameWindow(HWND &hwnd, WNDCLASSEX &windowClass);

	//���V���O���g���̂��߂ɃR���X�g���N�^��private��
	//����ɃR�s�[�Ƒ�����֎~��
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
};

