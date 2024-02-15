#include "HelpScene.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

HelpScene::HelpScene() :background_image(NULL),operation_flg(false),help_bgm(NULL)
{

}

HelpScene::~HelpScene()
{

}

//����������
void HelpScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Title.png");

	//BGM�̓ǂݍ���
	help_bgm = LoadSoundMem("Resource/bgm/HelpBGM.wav");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Title.png������܂���\n");
	}

	if (help_bgm == -1)
	{
		throw("Resource/bgm/HelpBGM.wav������܂���\n");
	}
}

//�X�V����
eSceneType HelpScene::Update()
{
	//BGM�Đ�
	PlaySoundMem(help_bgm, DX_PLAYTYPE_LOOP, FALSE);

	if (InputControl::GetButtonDown(XINPUT_BUTTON_A) && operation_flg == false)
	{
		operation_flg = true;
	}
	else if (InputControl::GetButtonDown(XINPUT_BUTTON_A) && operation_flg == true)
	{
		operation_flg = false;
	}
	//B�{�^���������ꂽ��A�^�C�g���ɖ߂�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
	}
	return GetNowScene();
}

//�`�揈��
void HelpScene::Draw() const
{
	//�w�i�摜�̕`��
	DrawGraph(0, 0, background_image, FALSE);

	if (operation_flg == false)
	{
		//�Q�[������
		SetFontSize(30);

		DrawString(240, 20, "�w���v���", 0, 0);

		SetFontSize(18);

		DrawString(20, 80, "����͏�Q��������Ȃ���", 0, 0);
		DrawString(20, 100, "�������҂��Q�[���ł�", 0, 0);
		DrawString(20, 120, "�R�����s���邩��Q����", 0, 0);
		DrawString(20, 140, "���񓖂���ƃQ�[���I�[�o�[�ł��B", 0, 0);
		DrawString(20, 160, "�G�����������Ɣ��΃Q�[�W�����܂�A", 0, 0);
		DrawString(20, 180, "MAX�̂Ƃ�Y�������ăQ�[�W�𔭓������", 0, 0);
		DrawString(20, 200, "�G��|����悤�ɂȂ�܂��I", 0, 0);
		DrawString(360, 80, "-- A�{�^���ő�����@�� --", 0, 0);
		DrawString(360, 120, "-- B�{�^���Ń^�C�g���� --", 0, 0);

	}
	else if(operation_flg == true)
	{
		SetFontSize(30);

		DrawString(240, 20, "<������@>", 0, 0);

		SetFontSize(18);

		DrawString(20, 80, "�\���L�[�ŏ㉺���E�ړ�", 0, 0);
		DrawString(20, 100, "LB�Ō����ARB�ŉ���", 0, 0);
		DrawString(20, 120, "���΃Q�[�W��MAX�̎��AY�{�^���ŕϐg", 0, 0);
		DrawString(20, 140, "�ϐg���͓G�ɓ�����Ɠ|�����Ƃ��o����", 0, 0);
		DrawString(20, 160, "B�{�^���Ńo���A�����v�R�񒣂邱�Ƃ��o����", 0, 0);
		DrawString(20, 180, "�o���A���́A�G�����蔲������", 0, 0);

		DrawString(360, 80, "-- A�{�^���Ńw���v��ʂ� --", 0, 0);
		DrawString(360, 120, "-- B�{�^���Ń^�C�g���� --", 0, 0);
	}

}

//�I��������
void HelpScene::Finalize()
{
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);

	//�ǂݍ���BGM�̍폜
	DeleteSoundMem(help_bgm);
}

//���݂̃V�[�������擾
eSceneType HelpScene::GetNowScene() const
{
	return eSceneType::E_HELP;
}