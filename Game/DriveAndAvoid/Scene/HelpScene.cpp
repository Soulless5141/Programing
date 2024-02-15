#include "HelpScene.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

HelpScene::HelpScene() :background_image(NULL),operation_flg(false),help_bgm(NULL)
{

}

HelpScene::~HelpScene()
{

}

//初期化処理
void HelpScene::Initialize()
{
	//画像の読み込み
	background_image = LoadGraph("Resource/images/Title.png");

	//BGMの読み込み
	help_bgm = LoadSoundMem("Resource/bgm/HelpBGM.wav");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Title.pngがありません\n");
	}

	if (help_bgm == -1)
	{
		throw("Resource/bgm/HelpBGM.wavがありません\n");
	}
}

//更新処理
eSceneType HelpScene::Update()
{
	//BGM再生
	PlaySoundMem(help_bgm, DX_PLAYTYPE_LOOP, FALSE);

	if (InputControl::GetButtonDown(XINPUT_BUTTON_A) && operation_flg == false)
	{
		operation_flg = true;
	}
	else if (InputControl::GetButtonDown(XINPUT_BUTTON_A) && operation_flg == true)
	{
		operation_flg = false;
	}
	//Bボタンが押されたら、タイトルに戻る
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
	}
	return GetNowScene();
}

//描画処理
void HelpScene::Draw() const
{
	//背景画像の描画
	DrawGraph(0, 0, background_image, FALSE);

	if (operation_flg == false)
	{
		//ゲーム説明
		SetFontSize(30);

		DrawString(240, 20, "ヘルプ画面", 0, 0);

		SetFontSize(18);

		DrawString(20, 80, "これは障害物を避けながら", 0, 0);
		DrawString(20, 100, "距離を稼ぐゲームです", 0, 0);
		DrawString(20, 120, "燃料が尽きるか障害物に", 0, 0);
		DrawString(20, 140, "数回当たるとゲームオーバーです。", 0, 0);
		DrawString(20, 160, "敵を避け続けると爆笑ゲージがたまり、", 0, 0);
		DrawString(20, 180, "MAXのときYを押してゲージを発動すると", 0, 0);
		DrawString(20, 200, "敵を倒せるようになります！", 0, 0);
		DrawString(360, 80, "-- Aボタンで操作方法へ --", 0, 0);
		DrawString(360, 120, "-- Bボタンでタイトルへ --", 0, 0);

	}
	else if(operation_flg == true)
	{
		SetFontSize(30);

		DrawString(240, 20, "<操作方法>", 0, 0);

		SetFontSize(18);

		DrawString(20, 80, "十字キーで上下左右移動", 0, 0);
		DrawString(20, 100, "LBで減速、RBで加速", 0, 0);
		DrawString(20, 120, "爆笑ゲージがMAXの時、Yボタンで変身", 0, 0);
		DrawString(20, 140, "変身中は敵に当たると倒すことが出来る", 0, 0);
		DrawString(20, 160, "Bボタンでバリアを合計３回張ることが出来る", 0, 0);
		DrawString(20, 180, "バリア中は、敵をすり抜けられる", 0, 0);

		DrawString(360, 80, "-- Aボタンでヘルプ画面へ --", 0, 0);
		DrawString(360, 120, "-- Bボタンでタイトルへ --", 0, 0);
	}

}

//終了時処理
void HelpScene::Finalize()
{
	//読み込んだ画像の削除
	DeleteGraph(background_image);

	//読み込んだBGMの削除
	DeleteSoundMem(help_bgm);
}

//現在のシーン情報を取得
eSceneType HelpScene::GetNowScene() const
{
	return eSceneType::E_HELP;
}