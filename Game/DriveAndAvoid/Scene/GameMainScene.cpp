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

//初期化処理
void GameMainScene::Initialize()
{
	//高得点値を読み込む
	ReadHighScore();

	//画像の読み込み
	back_ground = LoadGraph("Resource/images/back.bmp");
	barrier_image = LoadGraph("Resource/images/barrier.png");
	enemy_image[0] = LoadGraph("Resource/images/Enemy1.png");
	enemy_image[1] = LoadGraph("Resource/images/Enemy2.png");
	enemy_image[2] = LoadGraph("Resource/images/Enemy3.png");
	//BGM読み込み
	main_bgm = LoadSoundMem("Resource/bgm/MainBGM.wav");
	int explosion = LoadDivGraph("Resource/images/explosion.png", 16, 8, 2, 32, 32, explosion_image);
	PexplosionSE = LoadSoundMem("Resource/se/player_explosion.wav");
	kanSE = LoadSoundMem("Resource/se/kan.wav");


	//エラーチェック
	if (back_ground == -1)
	{
		throw("Resource/images/back.bmpがありません\n");
	}

	if (enemy_image[0] == -1)
	{
		throw("Resource/images/Enemy1.pngがありません\n");
	}


	if (enemy_image[1] == -1)
	{
		throw("Resource/images/Enemy2.pngがありません\n");
	}

	if (enemy_image[2] == -1)
	{
		throw("Resource/images/Enemy3.pngがありません\n");
	}

	if (explosion == -1)
	{
		throw("Resource/images/explosion.pngがありません\n");
	}

	if (barrier_image == -1)
	{
		throw("Resource/images/car.bmpがありません\n");
	}
	if (kanSE == -1)
	{
		throw("Resource/se/kan.wavがありません\n");
	}

	//BGM再生
	PlaySoundMem(main_bgm, DX_PLAYTYPE_LOOP, TRUE);

	//オブジェクトの生成
	player = new Player;
	enemy = new Enemy* [10];

	//オブジェクトの初期化
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

//更新処理
eSceneType GameMainScene::Update()
{
	//プレイヤーの更新
	player->Update();

	//移動距離の更新
	mileage += (int)player->GetSpeed
	() + 5;

	//敵の生成処理
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

	//アイテムの生成処理
	if (mileage / 10 % 500 == 0)
	{
		int index = item.size();
		item.emplace_back();
		item[index].Initialize();
	}

	//nico上限設定
	if (player->GetNico() >= 1000 && player->GetNicoNow() == 0)
	{
		player->DecreaseNicoNow(1);
	}

	//敵の更新と当たり判定チェック
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

			//画面外に行ったら、敵を削除してスコア加算
			if (enemy[i]->GetLocation().y >= 640.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				if (player->GetNicoNow() == 0)     //上限1000のnico加算処理
				{
					player->DecreaseNico((enemy[i]->GetType() + 1 )* 100.0f); // 画面外に行った車の種類によって、ポイントが変わる。
				}
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;				
			}

			//当たり判定の確認
			if (IsHitCheck(player, enemy[i]))
			{
				// 爆笑ゲージ使用中かどうか？
				if (player->GetNicoNow() == 2) {
					// 燃料を500回復
					player->AddFuel(500.f);
					// 敵を倒したら、倒したタイプを１加算
					enemy_count[enemy[i]->GetType()]++;
					enemy[i]->AnimEx();
					//enemy[i]->Finalize();
					//delete enemy[i];
					//enemy[i] = nullptr;
				}
				else {  // 爆笑ゲージ使用していなければ
					player->SetActive(false);
					player->DecreaseHp(-100.0f);
					/* ここに爆破を入れたい */
					PlaySoundMem(PexplosionSE,DX_PLAYTYPE_BACK,TRUE);
					enemy[i]->Finalize();
					delete enemy[i];
					enemy[i] = nullptr;
				}
			}
		}
	}

	//アイテムの更新と当たり判定チェック
	for (int i = 0; i < item.size(); i++)
	{
		item[i].Update(player->GetSpeed());

		//画面外に行ったら、敵を削除してスコア加算
		if (item[i].GetLocation().y >= 640.0f)
		{
			item[i].Finalize();
			item.erase(item.begin() + i);
			i--;
			continue;
		}
		// 当たり判定の確認
		if (IsHitCheck(player, item[i])) {
			PlaySoundMem(kanSE, DX_PLAYTYPE_BACK, TRUE);
			// 燃料を1000回復
			player->AddFuel(1000.f);
			item[i].Finalize();
			item.erase(item.begin() + i);
			i--;
		}
	}

	//nicoゲージがたまると減り続ける
	if (player->GetNicoNow() == 2)
	{
		player->DecreaseNico(-2.0f);
		if (player->GetNico() <= 0)
		{
			player->DecreaseNicoNow(-2);
		}
	}

	//プレイヤーの燃料か体力が0未満なら、リザルトに遷移する
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

//描画処理
void GameMainScene::Draw() const
{
	//背景画像の描画
	DrawGraph(0, mileage % 480 - 480, back_ground, TRUE);
	DrawGraph(0, mileage % 480, back_ground, TRUE);

	//敵の描画
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

	//プレイヤーの描画
	player->Draw();

	

	//UIの描画
	DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);
	SetFontSize(16);
	DrawFormatString(510, 20, GetColor(0, 0, 0), "ハイスコア");
	DrawFormatString(560, 40, GetColor(255, 255, 255), "%08d", high_score);
	DrawFormatString(510, 80, GetColor(0, 0, 0), "避けた数");
	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(523 + (i * 50), 120, 0.8, 0, enemy_image[i], TRUE, FALSE);
		DrawFormatString(510 + (i * 50), 140, GetColor(255, 255, 255), "%03d", enemy_count[i]);
	}
	DrawFormatString(510, 200, GetColor(0, 0, 0), "走行距離");
	DrawFormatString(550, 220, GetColor(255, 255, 255), "%08d", mileage / 10);
	DrawFormatString(510, 240, GetColor(0, 0, 0), "スピード");
	DrawFormatString(555, 260, GetColor(255, 255, 255), "%08.1f", player->GetSpeed());

	//バリアの枚数描画
	for (int i = 0; i < player->GetBarriarCount(); i++) {
		DrawRotaGraph(520 + i * 25, 300, 0.2f, 0, barrier_image, TRUE, FALSE);
	}

	//燃料ゲージの描画
	float fx = 510.0f;
	float fy = 390.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "FUEL METER");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetFuel() * 100 / 20000), fy +
		40.0f, GetColor(0, 102, 204), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//体力ゲージの描画
	fx = 510.0f;
	fy = 430.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "PLAYER HP");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetHp() * 100 / 1000), fy + 40.0f, GetColor(255, 0, 0), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//nicoゲージの描画
	fx = 510.0f;
	fy = 350.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "nico GAGE");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetNico() * 100 / 1000), fy + 40.0f, GetColor(0, 255, 0), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);
	if (player->GetNicoMax() == true) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, yButtonAlpha);
		DrawFormatStringF(fx + 20.0f, fy + 22.0f, GetColor(255, 0, 0), "Yを押せ！"); // Yを押すように誘導させる
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	DrawFormatString(510, 300, 0xffffff, "躱した：%f", player->GetNico());
}

//終了時処理
void GameMainScene::Finalize()
{
	//BGM停止
	StopSoundMem(main_bgm);
	//メモリ解放のためSEデータ削除
	DeleteSoundMem(kanSE);

	//スコアを計算する
	int score = (mileage / 10 * 10);
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 500 * enemy_count[i];
	}

	//リザルトデータの書き込み
	FILE* fp = nullptr;
	//ファイルオープン
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/result_data.csvが開けません\n");
	}

	//スコアを保存
	fprintf(fp, "%d,\n", score);

	//避けた数と得点を保存
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d\n", enemy_count[i]);
	}

	// 走行距離を保存
	fprintf(fp, "%d,\n", mileage / 10 * 10);

	//ファイルクローズ
	fclose(fp);

	//動的確保したオブジェクトを削除する
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

//現在のシーン情報を取得
eSceneType GameMainScene::GetNowScene() const
{
	return eSceneType::E_MAIN;
}

//ハイスコアの読み込み
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}

//当たり判定処理（プレイヤーと敵）
bool GameMainScene::IsHitCheck(Player* p, Enemy* e)
{
	//プレイヤーがバリアを貼っていたら、当たり判定を無視する
	if (p->IsBarrier())
	{
		return false;
	}

	//敵情報がなければ、当たり判定を無視する
	if (e == nullptr)
	{
		return false;
	}

	if (e->GetExFlg() == 1)
	{
		return false;
	}

	//位置情報の差分を取得
	Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) <
	box_ex.y));
}

//当たり判定処理（プレイヤーと敵）
bool GameMainScene::IsHitCheck(Player* p, Item it)
{
	//位置情報の差分を取得
	Vector2D diff_location = p->GetLocation() - it.GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = p->GetBoxSize() + it.GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) <
	box_ex.y));
}
