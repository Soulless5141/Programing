#pragma once

#include"SceneBase.h"
#include"../Object/Player.h"
#include"../Object/Enemy.h"
#include"../Object/Item.h"

#include <vector>

class GameMainScene : public SceneBase
{
private:
	int high_score;        //ハイスコア
	int back_ground;       //背景画像
	int barrier_image;     //バリア画像
	int mileage;           //走行距離
	int enemy_image[3];    //敵画像
	int enemy_count[3];    //通り過ぎた敵カウント
	int explosion_image[58];//爆破画像 
	int explosion_flg;		// 爆破フラグ
	int PexplosionSE;     //プレイヤーの爆破SE
	int kanSE;			  //ガソリン缶のSE
	Player* player;        //プレイヤー
	Enemy** enemy;         //敵
	std::vector<Item> item;//アイテム
	Vector2D enemy_size[3];// 敵の当たり判定のサイズ
	int yButtonAlpha;		//「Yを押せ！」の透明度
	int yButtonAddAlpha;	//「Yを押せ！」の透明度の変化値
	int main_bgm;           //メインBGM

public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	//ハイスコア読み込み処理
	void ReadHighScore();
	//当たり判定
	bool IsHitCheck(Player* p, Enemy* e);
	//アイテムの当たり判定
	bool IsHitCheck(Player* p, Item it);
};