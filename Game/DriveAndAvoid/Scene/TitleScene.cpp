#include "TitleScene.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL),
cursor_image(NULL), menu_cursor(0),title_bgm(NULL)
{

}

TitleScene::~TitleScene()
{

}

//初期化処理
void TitleScene::Initialize()
{
	//画像の読み込み
	background_image = LoadGraph("Resource/images/Title.png");
	menu_image = LoadGraph("Resource/images/menu.bmp");
	cursor_image = LoadGraph("Resource/images/cone.bmp");

	//BGM読み込み
	title_bgm = LoadSoundMem("Resource/bgm/TitleBGM.wav");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Title.pngがありません\n");
	}

	if (menu_image == -1)
	{
		throw("Resource/images/menu.bmpがありません\n");
	}

	if (cursor_image == -1)
	{
		throw("Resource/images/cone.bmpがありません\n");
	}

	if (title_bgm == -1)
	{
		throw("Resource/bgm/TitleBGM.wavがありません\n");
	}

	SetFontSize(48);
}

//更新処理
eSceneType TitleScene::Update()
{
	//BGM再生
	PlaySoundMem(title_bgm, DX_PLAYTYPE_LOOP, FALSE);

	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		menu_cursor++;
		//1番下に到達したら、一番上にする
		if (menu_cursor > 3)
		{
			menu_cursor = 0;
		}
	}

	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		menu_cursor--;
		//1番上に到達したら、一番下にする
		if (menu_cursor < 0)
		{
			menu_cursor = 3;
		}
	}

	//カーソル決定（決定した画面に遷移する）
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
		case 0:
			return eSceneType::E_MAIN;
		case 1:
			return eSceneType::E_RANKING_DISP;
		case 2:
			return eSceneType::E_HELP;
		default:
			return eSceneType::E_END;
		}
	}
	//現在のシーンタイプを返す
	return GetNowScene();
}

//描画処理
void TitleScene::Draw() const
{
	//タイトル画像の描画
	DrawGraph(0, 0, background_image, FALSE);

	//メニュー画像の描画
	DrawGraph(90, 230, menu_image, TRUE);

	//カーソル画像の描画
	DrawRotaGraph(60, 250 + menu_cursor * 40, 0.7, DX_PI / 2.0, cursor_image, TRUE);

	//タイトルの描画
	DrawString(50, 100, "ニコちゃんは旅に出る。", 0xFF0000);

}

//終了時処理
void TitleScene::Finalize()
{
	//読み込んだ画像の削除
	DeleteGraph(background_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);

	//BGM停止
	StopSoundMem(title_bgm);
}

//現在のシーン情報を取得
eSceneType TitleScene::GetNowScene() const
{
	return eSceneType::E_TITLE;
}