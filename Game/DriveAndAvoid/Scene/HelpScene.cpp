#include "HelpScene.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

HelpScene::HelpScene() :background_image(NULL)
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

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Title.pngがありません\n");
	}
}

//更新処理
eSceneType HelpScene::Update()
{
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

	//ゲーム説明
	SetFontSize(30);

	DrawString(240, 20, "ヘルプ画面", 0, 0);

	SetFontSize(18);

	DrawString(160, 80, "これは障害物を避けながら", 0, 0);
	DrawString(160, 100, "距離を稼ぐゲームです", 0, 0);
	DrawString(160, 120, "燃料が尽きるか障害物に", 0, 0);
	DrawString(160, 140, "数回当たるとゲームオーバーです。", 0, 0);
	DrawString(160, 160, "敵を避け続けると爆笑ゲージがたまり、", 0, 0);
	DrawString(160, 180, "MAXのときYを押してゲージを発動すると", 0, 0);
	DrawString(160, 200, "敵を倒せるようになります！", 0, 0);

	DrawString(135, 450, "----- Bボタンを押してタイトルへ戻る -----", 0, 0);
}

//終了時処理
void HelpScene::Finalize()
{
	//読み込んだ画像の削除
	DeleteGraph(background_image);
}

//現在のシーン情報を取得
eSceneType HelpScene::GetNowScene() const
{
	return eSceneType::E_HELP;
}