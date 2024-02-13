#include "Item.h"
#include "DxLib.h"

Item::Item() {

}

Item::~Item() {

}

void Item::Initialize() {
	//出現させるX座標パターンを取得
	float random_x = (float)(GetRand(4) * 105 + 40);
	//生成位置の設定
	location = Vector2D(random_x, -50.0f);
	box_size = Vector2D(20.0f, 40.0f);
	angle = 0.0f;
	speed = 6.0f;

	//画像の読み込み
	image = LoadGraph("Resource/images/kan.bmp");

	//エラーチェック
	if (image == -1)
	{
		throw("Resource/images/kan.bmpがありません\n");
	}
}

void Item::Update(float speed) {
	//位置情報に移動量を加算する
	location += Vector2D(0.0f, this->speed + speed - 6);
}

void Item::Draw() const {
	//アイテム画像の描画
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
}

void Item::Finalize() {

}

//位置情報を取得
Vector2D Item::GetLocation() const
{
	return location;
}

//当たり判定の大きさを取得
Vector2D Item::GetBoxSize() const
{
	return box_size;
}