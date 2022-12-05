#include <DxLib.h>
#include "Config.hpp"
#include "Input.hpp"
#include "Game.hpp"

#ifdef USE_RESOURCE
#include "resource.h"
#endif // USE_RESOURCE

int common::width = 640;
int common::height = 480;
int common::colorbit = 32;
int common::fps = 60;
bool common::windowmode = true;

std::mt19937 common::engine(std::random_device{}());

#ifdef USE_RESOURCE
void MenuItemSelectCallBack(const TCHAR* ItemName, int ItemID)
{
}
#endif // USE_RESOURCE

#ifdef USE_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
int main() { return WinMain(GetModuleHandle(0), NULL, NULL, _Notnull_ SW_SHOWDEFAULT); }
#endif // USE_CONSOLE

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ������
	int count = 0;	// FPS�����p
	SetMainWindowText("Now Loading...");
	SetGraphMode(common::width, common::height, common::colorbit);
	// DxLib�������u���b�N
	//----------------------------------------------------------------<<
	// �E�B���h�E���[�h��ݒ�
	ChangeWindowMode(common::windowmode);
#ifdef USE_RESOURCE
	// �����E�B���h�E���[�h�Ȃ�
	if (common::windowmode)
	{
		// ���j���[�̃��\�[�X��ݒ�
		LoadMenuResource(IDR_MENU1);
		// ���j���[����Ăяo���R�[���o�b�N�֐���ݒ�
		SetMenuItemSelectCallBackFunction(MenuItemSelectCallBack);
	}
#endif // USE_RESOURCE
	// �g�p���镶���R�[�h��UTF-8�ɐݒ�
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	// �t�@�C���̃h���b�O&�h���b�v��L���ɂ���
	SetDragFileValidFlag(TRUE);

	//---------------------------------------------------------------->>
	// ������
	if(DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	// �������u���b�N
	//----------------------------------------------------------------<<
	Game::preset();	

	//---------------------------------------------------------------->>
	// �^�C�g���\��
	SetMainWindowText("");
	// ���C�����[�v
	while(ProcessMessage() != -1)
	{
		// FPS
		count = GetNowCount();
		// �L�[�{�[�h���͏󋵍X�V
		Keyboard::update();
		Mouse::update();
		// �����u���b�N
		//----------------------------------------------------------------<<
		if (Game::update() == Game::Message::quit)
			break;

		//---------------------------------------------------------------->>
		// �`��u���b�N
		//----------------------------------------------------------------<<
		Game::draw();

		//---------------------------------------------------------------->>
		// ��ʕ\��
		ScreenFlip();
		// ��ʏ���
		ClearDrawScreen();
		WaitTimer(1000 / common::fps - GetNowCount() + count);
	}
	// �I������
	SetMainWindowText("Process Quitting...");
	DxLib_End();
	return 0;
}

