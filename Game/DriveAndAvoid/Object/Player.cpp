#include "Player.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

// 燃料の最大値
#define MAX_FUEL 20000

Player::Player() : is_active(false), location(0.0f), box_size(0.0f),
angle(0.0f), speed(0.0f), hp(0.0f), fuel(0.0f),nico(0.0f),niconow(0.0f),nicomax(false),nico_se(NULL), barrier_count(0), barrier(nullptr)
{
	for (int i = 0; i < 3; i++)
	{
		image[i] = NULL;
	}
}

Player::~Player()
{

}

//初期化処理
void Player::Initialize()
{
	is_active = true;
	location = Vector2D(320.0f, 380.0f);
	box_size = Vector2D(31.0f, 60.0f);
	angle = 0.0f;
	speed = 5.0f;
	hp = 1000;
	nico = 0;
	niconow = 0;
	nicomax = false;
	fuel = MAX_FUEL;
	barrier_count = 3;

	//画像の読み込み
	int result = LoadDivGraph("Resource/images/car.bmp", 3, 3, 1, 63, 120, image);
	image[3] = LoadGraph("Resource/images/car1pol.bmp");

	//nicoゲージMax時の音声の読み込み
	nico_se = LoadSoundMem("Resource/se/nico.wav");

	//エラーチェック
	if (result == -1)
	{
		throw("Resource/images/car.bmpがありません\n");
	}

	if (image[3] == -1)
	{
		throw("Resource/images/car1pol.bmpがありません\n");
	}

	if (nico_se == -1)
	{
		throw("Resource/se/nico.wavがありません\n");
	}
}

//更新処理
void Player::Update()
{
	//操作不可状態であれば、自身を回転させる
	if (!is_active)
	{
		angle += DX_PI_F / 24.0f;
		speed = 3.0f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}

	// 爆笑ゲージが使用中じゃない時
	if (niconow != 2) {
		//燃料の消費
		fuel -= speed;
	}

	//移動処理
	Movement();

	//加減速処理
	Acceleration();

	if (InputControl::GetButtonDown(XINPUT_BUTTON_START))
	{
		is_active = false;
	}

	//バリア処理
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B) && barrier_count > 0)
	{
		if (barrier == nullptr)
		{
			barrier_count--;
			barrier = new Barrier;
		}
	}

	//バリアが生成されていたら、更新を行う
	if (barrier != nullptr)
	{
		//バリア時間が経過したか？していたら、削除する
		if (barrier->IsFinished(this->speed))
		{
			delete barrier;
			barrier = nullptr;
		}
	}

	//nicoゲージがたまっているとき、Yを押すと発動
	if (GetNicoNow() == 1)
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_Y))
		{
			DecreaseNicoNow(1);
		}
	}
}

//描画処理
void Player::Draw()
{
	//バリアが生成されていたら、描画を行う
	if (barrier != nullptr)
	{
		barrier->Draw(this->location);
	}

	//プレイヤー画像の描画
	if (niconow == 2)
	{
		DrawRotaGraphF(location.x, location.y, 1.0, angle, image[3], TRUE);
	}
	else
	{
		if (hp <= 200.0f)
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[2], TRUE);
		}
		else if (hp <= 500.0f)
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[0], TRUE);
		}
		else
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[1], TRUE);
		}
	}
}

//終了処理
void Player::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(image[0]);
	DeleteGraph(image[1]);
	DeleteGraph(image[2]);
	DeleteGraph(image[3]);

	//バリアが生成されていたら、削除する
	if (barrier != nullptr)
	{
		delete barrier;
	}
}

//状態設定処理
void Player::SetActive(bool flg)
{
	this->is_active = flg;
}

//体力減少処理
void Player::DecreaseHp(float value)
{
	this->hp += value;
}

//nico増減処理
void Player::DecreaseNico(float value)
{
	this->nico += value;
	// nicoゲージが1000以上の時
	if (nico >= 1000) {
		this->nico = 1000; // nicoを1000にする。
		nicomax = true; // 使用出来るように伝える
		// ここに笑い声1回鳴らす
		PlaySoundMem(nico_se, DX_PLAYTYPE_BACK, TRUE);
	}
	else {
		nicomax = false; // 使用出来ないように伝える
	}
}

//nico中かどうかの確認(0:増加中  1:待機中   2:減少中)
void Player::DecreaseNicoNow(float value)
{
	this->niconow += value;
}


//位置情報取得処理
Vector2D Player::GetLocation() const
{
	return this->location;
}

//当たり判定の大きさ取得処理
Vector2D Player::GetBoxSize() const
{
	return this->box_size;
}

//速さ取得処理
float Player::GetSpeed() const
{
	return this->speed;
}

//燃料取得処理
float Player::GetFuel() const
{
	return this->fuel;
}

//体力取得処理
float Player::GetHp() const
{
	return this->hp;
}

//nico取得処理
float Player::GetNico() const
{
	return this->nico;
}

//nico取得処理
float Player::GetNicoNow() const
{
	return this->niconow;
}

//nicoは満タンか？を取得
bool Player::GetNicoMax() const
{
	if (nico == 1000)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//バリア枚数取得処理
int Player::GetBarriarCount() const
{
	return this->barrier_count;
}

//バリアは有効か？を取得
bool Player::IsBarrier() const
{
	return (barrier != nullptr);
}

//移動処理
void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);
	angle = 0.0f;

	//十字移動処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		move += Vector2D(-1.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		move += Vector2D(1.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP))
	{
		move += Vector2D( 0.0f,-1.0f);
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		move += Vector2D(0.0f, 1.0f);
	}

	move *= speed / 3.f;
	location += move;

	//画面外に行かないように制限する
	if ((location.x < box_size.x) || (location.x >= 640.0f - 180.f) ||
		(location.y < box_size.y) || (location.y >= 480.0f - box_size.y))
	{
		location -= move;
	}
}

//加減速処理
void Player::Acceleration()
{
	//LBボタンが押されたら、減速する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 3.0f)
	{
		speed -= 1.0f;
	}
	//RBボタンが押されたら、加速する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 10.0f)
	{
		speed += 1.0f;
	}
}

//燃料増やす
void Player::AddFuel(float add) {
	fuel += add;
	if (fuel >= MAX_FUEL) {
		fuel = MAX_FUEL;
	}
}