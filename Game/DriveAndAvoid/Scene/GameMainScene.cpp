#include "GameMainScene.h"
#include"../Object/RankingData.h"
#include"DxLib.h"
#include<math.h>

GameMainScene::GameMainScene() : high_score(0), back_ground(NULL),
barrier_image(NULL), mileage(0), explosion_flg(false), player(nullptr), enemy(nullptr)
{
	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
	for (int i = 0; i < 58; i++)
	{
		explosion_image[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{

}

//����������
void GameMainScene::Initialize()
{
	//�����_�l��ǂݍ���
	ReadHighScore();

	//�摜�̓ǂݍ���
	back_ground = LoadGraph("Resource/images/back.bmp");
	barrier_image = LoadGraph("Resource/images/barrier.png");
	enemy_image[0] = LoadGraph("Resource/images/Enemy1.png");
	enemy_image[1] = LoadGraph("Resource/images/Enemy2.png");
	enemy_image[2] = LoadGraph("Resource/images/Enemy3.png");
	//BGM�ǂݍ���
	main_bgm = LoadSoundMem("Resource/bgm/MainBGM.wav");
	int explosion = LoadDivGraph("Resource/images/explosion.png", 16, 8, 2, 32, 32, explosion_image);
	PexplosionSE = LoadSoundMem("Resource/se/player_explosion.wav");
	kanSE = LoadSoundMem("Resource/se/kan.wav");


	//�G���[�`�F�b�N
	if (back_ground == -1)
	{
		throw("Resource/images/back.bmp������܂���\n");
	}

	if (enemy_image[0] == -1)
	{
		throw("Resource/images/Enemy1.png������܂���\n");
	}


	if (enemy_image[1] == -1)
	{
		throw("Resource/images/Enemy2.png������܂���\n");
	}

	if (enemy_image[2] == -1)
	{
		throw("Resource/images/Enemy3.png������܂���\n");
	}

	if (explosion == -1)
	{
		throw("Resource/images/explosion.png������܂���\n");
	}

	if (barrier_image == -1)
	{
		throw("Resource/images/car.bmp������܂���\n");
	}
	if (kanSE == -1)
	{
		throw("Resource/se/kan.wav������܂���\n");
	}

	//BGM�Đ�
	PlaySoundMem(main_bgm, DX_PLAYTYPE_LOOP, TRUE);

	//�I�u�W�F�N�g�̐���
	player = new Player;
	enemy = new Enemy* [10];

	//�I�u�W�F�N�g�̏�����
	player->Initialize();

	for (int i = 0; i < 10; i++)
	{
		enemy[i] = nullptr;
	}

	enemy_size[0] = Vector2D(31.0f, 21.f);
	enemy_size[1] = Vector2D(31.0f, 27.f);
	enemy_size[2] = Vector2D(31.0f, 31.f);

	yButtonAlpha = 255;
	yButtonAddAlpha = -30;
}

//�X�V����
eSceneType GameMainScene::Update()
{
	//�v���C���[�̍X�V
	player->Update();

	//�ړ������̍X�V
	mileage += (int)player->GetSpeed
	() + 5;

	//�G�̐�������
	if (mileage / 20 % 100 == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			if (enemy[i] == nullptr)
			{
				int type = GetRand(3) % 3;
				enemy[i] = new Enemy(type, enemy_image[type], enemy_size[type]);
				enemy[i]->Initialize();
				break;
			}
		}
	}

	//�A�C�e���̐�������
	if (mileage / 10 % 500 == 0)
	{
		int index = item.size();
		item.emplace_back();
		item[index].Initialize();
	}

	//nico����ݒ�
	if (player->GetNico() >= 1000 && player->GetNicoNow() == 0)
	{
		player->DecreaseNicoNow(1);
	}

	//�G�̍X�V�Ɠ����蔻��`�F�b�N
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Update(player->GetSpeed());

			if (enemy[i]->IsPlayExplosion() == true) {
				continue;
			}

			if (enemy[i]->GetExFlg() == 2)
			{
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
				continue;
			}

			//��ʊO�ɍs������A�G���폜���ăX�R�A���Z
			if (enemy[i]->GetLocation().y >= 640.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				if (player->GetNicoNow() == 0)     //���1000��nico���Z����
				{
					player->DecreaseNico((enemy[i]->GetType() + 1 )* 100.0f); // ��ʊO�ɍs�����Ԃ̎�ނɂ���āA�|�C���g���ς��B
				}
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;				
			}

			//�����蔻��̊m�F
			if (IsHitCheck(player, enemy[i]))
			{
				// ���΃Q�[�W�g�p�����ǂ����H
				if (player->GetNicoNow() == 2) {
					// �R����500��
					player->AddFuel(500.f);
					// �G��|������A�|�����^�C�v���P���Z
					enemy_count[enemy[i]->GetType()]++;
					enemy[i]->AnimEx();
					//enemy[i]->Finalize();
					//delete enemy[i];
					//enemy[i] = nullptr;
				}
				else {  // ���΃Q�[�W�g�p���Ă��Ȃ����
					player->SetActive(false);
					player->DecreaseHp(-100.0f);
					/* �����ɔ��j����ꂽ�� */
					PlaySoundMem(PexplosionSE,DX_PLAYTYPE_BACK,TRUE);
					enemy[i]->Finalize();
					delete enemy[i];
					enemy[i] = nullptr;
				}
			}
		}
	}

	//�A�C�e���̍X�V�Ɠ����蔻��`�F�b�N
	for (int i = 0; i < item.size(); i++)
	{
		item[i].Update(player->GetSpeed());

		//��ʊO�ɍs������A�G���폜���ăX�R�A���Z
		if (item[i].GetLocation().y >= 640.0f)
		{
			item[i].Finalize();
			item.erase(item.begin() + i);
			i--;
			continue;
		}
		// �����蔻��̊m�F
		if (IsHitCheck(player, item[i])) {
			PlaySoundMem(kanSE, DX_PLAYTYPE_BACK, TRUE);
			// �R����1000��
			player->AddFuel(1000.f);
			item[i].Finalize();
			item.erase(item.begin() + i);
			i--;
		}
	}

	//nico�Q�[�W�����܂�ƌ��葱����
	if (player->GetNicoNow() == 2)
	{
		player->DecreaseNico(-2.0f);
		if (player->GetNico() <= 0)
		{
			player->DecreaseNicoNow(-2);
		}
	}

	//�v���C���[�̔R�����̗͂�0�����Ȃ�A���U���g�ɑJ�ڂ���
	if (player->GetFuel() < 0.0f || player->GetHp() <= 0.0f)
	{
		return eSceneType::E_RESULT;
	}

	if (player->GetNicoMax() == true)
	{
		yButtonAlpha += yButtonAddAlpha;
		if (yButtonAlpha <= 0)
		{
			yButtonAlpha = 0;
			yButtonAddAlpha *= -1;
		}
		else if (yButtonAlpha >= 255)
		{
			yButtonAlpha = 255;
			yButtonAddAlpha *= -1;
		}
	}

	return GetNowScene();
}

//�`�揈��
void GameMainScene::Draw() const
{
	//�w�i�摜�̕`��
	DrawGraph(0, mileage % 480 - 480, back_ground, TRUE);
	DrawGraph(0, mileage % 480, back_ground, TRUE);

	//�G�̕`��
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Draw();
			
		}
	}

	for (int i = 0; i < item.size(); i++) {
		item[i].Draw();
	}

	//�v���C���[�̕`��
	player->Draw();

	

	//UI�̕`��
	DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);
	SetFontSize(16);
	DrawFormatString(510, 20, GetColor(0, 0, 0), "�n�C�X�R�A");
	DrawFormatString(560, 40, GetColor(255, 255, 255), "%08d", high_score);
	DrawFormatString(510, 80, GetColor(0, 0, 0), "��������");
	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(523 + (i * 50), 120, 0.8, 0, enemy_image[i], TRUE, FALSE);
		DrawFormatString(510 + (i * 50), 140, GetColor(255, 255, 255), "%03d", enemy_count[i]);
	}
	DrawFormatString(510, 200, GetColor(0, 0, 0), "���s����");
	DrawFormatString(550, 220, GetColor(255, 255, 255), "%08d", mileage / 10);
	DrawFormatString(510, 240, GetColor(0, 0, 0), "�X�s�[�h");
	DrawFormatString(555, 260, GetColor(255, 255, 255), "%08.1f", player->GetSpeed());

	//�o���A�̖����`��
	for (int i = 0; i < player->GetBarriarCount(); i++) {
		DrawRotaGraph(520 + i * 25, 300, 0.2f, 0, barrier_image, TRUE, FALSE);
	}

	//�R���Q�[�W�̕`��
	float fx = 510.0f;
	float fy = 390.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "FUEL METER");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetFuel() * 100 / 20000), fy +
		40.0f, GetColor(0, 102, 204), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//�̗̓Q�[�W�̕`��
	fx = 510.0f;
	fy = 430.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "PLAYER HP");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetHp() * 100 / 1000), fy + 40.0f, GetColor(255, 0, 0), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//nico�Q�[�W�̕`��
	fx = 510.0f;
	fy = 350.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "nico GAGE");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetNico() * 100 / 1000), fy + 40.0f, GetColor(0, 255, 0), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);
	if (player->GetNicoMax() == true) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, yButtonAlpha);
		DrawFormatStringF(fx + 20.0f, fy + 22.0f, GetColor(255, 0, 0), "Y�������I"); // Y�������悤�ɗU��������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	DrawFormatString(510, 300, 0xffffff, "�]�����F%f", player->GetNico());
}

//�I��������
void GameMainScene::Finalize()
{
	//BGM��~
	StopSoundMem(main_bgm);
	//����������̂���SE�f�[�^�폜
	DeleteSoundMem(kanSE);

	//�X�R�A���v�Z����
	int score = (mileage / 10 * 10);
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 500 * enemy_count[i];
	}

	//���U���g�f�[�^�̏�������
	FILE* fp = nullptr;
	//�t�@�C���I�[�v��
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//�G���[�`�F�b�N
	if (result != 0)
	{
		throw("Resource/dat/result_data.csv���J���܂���\n");
	}

	//�X�R�A��ۑ�
	fprintf(fp, "%d,\n", score);

	//���������Ɠ��_��ۑ�
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d\n", enemy_count[i]);
	}

	// ���s������ۑ�
	fprintf(fp, "%d,\n", mileage / 10 * 10);

	//�t�@�C���N���[�Y
	fclose(fp);

	//���I�m�ۂ����I�u�W�F�N�g���폜����
	player->Finalize();
	delete player;

	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Finalize();
			delete enemy[i];
			enemy[i] = nullptr;
		}
	}

	delete[] enemy;
}

//���݂̃V�[�������擾
eSceneType GameMainScene::GetNowScene() const
{
	return eSceneType::E_MAIN;
}

//�n�C�X�R�A�̓ǂݍ���
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}

//�����蔻�菈���i�v���C���[�ƓG�j
bool GameMainScene::IsHitCheck(Player* p, Enemy* e)
{
	//�v���C���[���o���A��\���Ă�����A�����蔻��𖳎�����
	if (p->IsBarrier())
	{
		return false;
	}

	//�G��񂪂Ȃ���΁A�����蔻��𖳎�����
	if (e == nullptr)
	{
		return false;
	}

	if (e->GetExFlg() == 1)
	{
		return false;
	}

	//�ʒu���̍������擾
	Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//�����蔻��T�C�Y�̑傫�����擾
	Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//�R���W�����f�[�^���ʒu���̍������������Ȃ�A�q�b�g����Ƃ���
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) <
	box_ex.y));
}

//�����蔻�菈���i�v���C���[�ƓG�j
bool GameMainScene::IsHitCheck(Player* p, Item it)
{
	//�ʒu���̍������擾
	Vector2D diff_location = p->GetLocation() - it.GetLocation();

	//�����蔻��T�C�Y�̑傫�����擾
	Vector2D box_ex = p->GetBoxSize() + it.GetBoxSize();

	//�R���W�����f�[�^���ʒu���̍������������Ȃ�A�q�b�g����Ƃ���
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) <
	box_ex.y));
}
