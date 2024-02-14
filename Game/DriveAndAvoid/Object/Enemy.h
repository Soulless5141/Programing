#pragma once

#include"../Utility/Vector2D.h"

class Enemy
{
private:
	int type;           //タイプ
	int image;          //画像
	float speed;        //速さ
	int ex_img[16];			// 爆破画像を入れるデータ
	int ex_flg;			// 爆破フラグ
	int ex_num;			// アニメーション画像を進める変数
	int ex_img_check;	// 中に画像があるか確認する変数
	int ex_se;			// se入れる変数
	Vector2D location;   //位置情報
	Vector2D box_size;   //当たり判定の大きさ

public:
	Enemy(int type, int handle, Vector2D size);
	~Enemy();

	void Initialize();          //初期化処理
	void Update(float speed);   //更新処理
	void Draw() const;          //画像処理
	void Finalize();            //終了時処理
	void AnimEx();				//爆破アニメーション関数
	int GetExFlg();			//爆破フラグをゲットする

	int GetType() const;            //タイプ取得
	bool IsPlayExplosion();
	Vector2D GetLocation() const;   //位置情報の取得
	Vector2D GetBoxSize() const;    //当たり判定の大きさを取得
};

