#pragma once

#include"../Utility/Vector2D.h"
#include"Barrier.h"

class Player
{
private:
	bool is_active;       //有効常態か？
	int image;            //画像データ
	Vector2D location;    //位置座標
	Vector2D box_size;    //当たり判定の大きさ
	float angle;          //角度
	float speed;          //速さ
	float hp;             //体力
	float fuel;              //燃料
	float sp;            //spゲージ
	float spnow;
	int barrier_count;    //バリアの枚数
	Barrier* barrier;     //バリア

public:
	Player();
	~Player();

	void Initialize();  //初期化処理
	void Update();      //更新処理
	void Draw();        //描画処理
	void Finalize();    //終了時処理

public:
	void SetActive(bool flg);    //有効フラグ設定
	void DecreaseHp(float value);  //体力減少処理
	void DecreaseSp(float value);  //SP増減処理
	void DecreaseSpNow(float value);  //SP中かどうかの確認(0:増加中  1:待機中   2:減少中)
	Vector2D GetLocation() const;  //位置座標取得処理
	Vector2D GetBoxSize() const;   //当たり判定の大きさ取得
	float GetSpeed() const;          //速さ取得処理
	float GetFuel() const;           //燃料取得
	float GetHp() const;           //体力取得
	float GetSp() const;           //SP取得
	float GetSpNow() const;        //SP状態取得
	int GetBarriarCount() const;   //バリアの枚数取得
	bool IsBarrier() const;          //バリア有効か？を取得
	void AddFuel(float add);		//燃料を増やす処理
	
private:
	void Movement();      //移動処理
	void Acceleration();   //加速処理
};

