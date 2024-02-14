#include "Enemy.h"
#include"DxLib.h"

Enemy::Enemy(int type, int handle, Vector2D size) :type(type), image(handle), speed(0.0f),
location(0.0f), box_size(size)
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
	LoadDivGraph("Resource/images/explosion.png", 16, 8, 2, 100, 100, ex_img);
}

void Enemy::Update(float speed)
{
	////位置情報に移動量を加算する
	//location += Vector2D(0.0f, this->speed + speed - 5);

	if (ex_flg == 1)
	{
		//位置情報に移動量を加算する
		location += Vector2D(0.0f, 3);
		ex_num++;
		if (ex_num >= 16)
		{
			ex_flg = 2;
		}
	}
	else
	{
		//位置情報に移動量を加算する
		location += Vector2D(0.0f, this->speed + speed - 5);
	}
}

void Enemy::Draw() const
{
	if (ex_flg == 1)
	{
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, ex_img[ex_num], TRUE); // アニメーション（仮）
	}
	else
	{
		//敵画像の描画
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
	}
}

void Enemy::Finalize()
{

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
}

// 爆破フラグをゲットする
int Enemy::GetExFlg()
{
	return this->ex_flg;
}