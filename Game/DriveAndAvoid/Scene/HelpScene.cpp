#include "HelpScene.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

HelpScene::HelpScene() :background_image(NULL)
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

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Title.png������܂���\n");
	}
}

//�X�V����
eSceneType HelpScene::Update()
{
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

	//�Q�[������
	SetFontSize(30);

	DrawString(240, 20, "�w���v���", 0, 0);

	SetFontSize(18);

	DrawString(160, 80, "����͏�Q��������Ȃ���", 0, 0);
	DrawString(160, 100, "�������҂��Q�[���ł�", 0, 0);
	DrawString(160, 120, "�R�����s���邩��Q����", 0, 0);
	DrawString(160, 140, "���񓖂���ƃQ�[���I�[�o�[�ł��B", 0, 0);
	DrawString(160, 160, "�G�����������Ɣ��΃Q�[�W�����܂�A", 0, 0);
	DrawString(160, 180, "MAX�̂Ƃ�Y�������ăQ�[�W�𔭓������", 0, 0);
	DrawString(160, 200, "�G��|����悤�ɂȂ�܂��I", 0, 0);

	DrawString(135, 450, "----- B�{�^���������ă^�C�g���֖߂� -----", 0, 0);
}

//�I��������
void HelpScene::Finalize()
{
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);
}

//���݂̃V�[�������擾
eSceneType HelpScene::GetNowScene() const
{
	return eSceneType::E_HELP;
}