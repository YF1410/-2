#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {

	safe_delete(playerModel);
	safe_delete(playerJumpLModel);
	safe_delete(playerJumpRModel);
	safe_delete(skydomeModel);
	safe_delete(groundModel);
	safe_delete(largeCarLModel);
	safe_delete(miniCarLModel);
	safe_delete(truckLModel);
	safe_delete(playerObj);
	safe_delete(playerJumpLObj);
	safe_delete(playerJumpRObj);
	safe_delete(skydomeObj);
	safe_delete(groundObj);
	for (int i = 0; i < 2; i++) {
		safe_delete(largeCarLObj[i]);
		safe_delete(miniCarLObj[i]);
		safe_delete(truckLObj[i]);
		safe_delete(largeCarRObj[i]);
		safe_delete(miniCarRObj[i]);
		safe_delete(truckRObj[i]);
	}
	safe_delete(warningMarkLT);
	safe_delete(warningMarkLM);
	safe_delete(warningMarkLB);
	safe_delete(warningMarkRT);
	safe_delete(warningMarkRM);
	safe_delete(warningMarkRB);
	safe_delete(title);
	safe_delete(noOnes0);
	safe_delete(noOnes1);
	safe_delete(noOnes2);
	safe_delete(noOnes3);
	safe_delete(noOnes4);
	safe_delete(noOnes5);
	safe_delete(noOnes6);
	safe_delete(noOnes7);
	safe_delete(noOnes8);
	safe_delete(noOnes9);
	safe_delete(noTens0);
	safe_delete(noTens1);
	safe_delete(noTens2);
	safe_delete(noTens3);
	safe_delete(noTens4);
	safe_delete(noTens5);
	safe_delete(noTens6);
	safe_delete(noTens7);
	safe_delete(noTens8);
	safe_delete(noTens9);
	safe_delete(uiBack);
	safe_delete(gameOver);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->titleAudio = audio;
	this->playAudio = audio;
	this->gameoverAudio = audio;
	this->carAudio = audio;
	this->dangerAudio = audio;
	this->deadAudio = audio;
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);
	playerModel = playerModel->CreateFromObject("player");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 0.0f, 0.0f, 0.0f });
	// 前景スプライト生成
	
	

	
	
}

void GameScene::Update() {
	//移動処理
	XMFLOAT3 PlayerPos = playerObj->GetPosition();
	
	//前移動
	if (input->PushKey(DIK_W))
	{
		PlayerPos.y += 0.5;
	}
	//後ろ移動
	else if (input->PushKey(DIK_S))
	{
		PlayerPos.y -= 0.5;
	}
	//左移動
	if (input->PushKey(DIK_A))
	{
		PlayerPos.x -= 0.5;
	}
	//右移動
	else if (input->PushKey(DIK_D))
	{
		PlayerPos.x += 0.5;
	}
	playerObj->SetPosition(PlayerPos);
	playerObj->Update();
}





void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブクジェクトの描画
	playerObj->Draw();
	
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 前景スプライトの描画
	
	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}