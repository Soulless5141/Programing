#include "Enemy.h"
#include"DxLib.h"

Enemy::Enemy(int type, int handle, Vector2D size) :type(type), image(handle), speed(0.0f),
location(0.0f), box_size(size),ex_flg(0), ex_img_check(NULL),ex_num(0),ex_se(0)
{
	for (int i = 0; i < 16; i++)
	{
		ex_img[i] = NULL;
	}
}

Enemy::~Enemy()
{

}

//初期化処理
void Enemy::Initialize()
{
	//出現させるX座標パターンを取得
	float random_x = (float)(GetRand(4) * 105 + 40);
	//生成位置の設定
	location = Vector2D(random_x, -50.0f);
	//当たり判定の設定
	//box_size = Vector2D(31.0f, 60.0f);
	//速さの設定
	speed = (float)((this->type + 1) * 2) + 2.f;
	//爆破データ読み込み
	ex_img_check = LoadDivGraph("Resource/images/explosion.png", 16, 8, 2, 100, 100, ex_img);
	//敵の音声データ読み込み
	ex_se = LoadSoundMem("Resource/se/enemy_explosion.wav");

	// 画像が入ってるか？
	if (ex_img_check == -1)
	{
		throw("Resource/images/explosion.pngがありません\n");
	}
	// SEが入っているか？
	if (ex_se == -1)
	{
		throw("Resource/se/enemy_explosion.wavがありません\n");
	}

	ex_flg = 0;
}

void Enemy::Update(float speed)
{
	////位置情報に移動量を加算する
	//location += Vector2D(0.0f, this->speed + speed - 5);

	if (ex_flg == 0)
	{
		//位置情報に移動量を加算する
		location += Vector2D(0.0f, this->speed + speed - 5);
	}
	else if (ex_flg == 1)
	{
		//位置情報に移動量を加算する
		location += Vector2D(0.0f, 3);
		ex_num++;
		
		if (ex_num >= 16)
		{
			ex_flg = 2;
		}
	}
}

void Enemy::Draw() const
{
	if (ex_flg == 0)
	{
		//敵画像の描画
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
	}
	else if (ex_flg == 1)
	{
		// アニメーション
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, ex_img[ex_num], TRUE);
	}
}

void Enemy::Finalize()
{
	DeleteSoundMem(ex_se);
}

//敵のタイプを取得
int Enemy::GetType() const
{
	return type;
}

//位置情報を取得
Vector2D Enemy::GetLocation() const
{
	return location;
}

//当たり判定の大きさを取得
Vector2D Enemy::GetBoxSize() const
{
	return box_size;
}

// 爆破のアニメーション処理
void Enemy::AnimEx()
{
	ex_flg = 1;
	ex_num = 0;
	// 音声入れる
	PlaySoundMem(ex_se, DX_PLAYTYPE_BACK, TRUE);
}

// 爆破フラグをゲットする
int Enemy::GetExFlg()
{
	return this->ex_flg;
}

bool Enemy::IsPlayExplosion() {
	return CheckSoundMem(ex_se) == TRUE;
}