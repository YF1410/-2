#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {

	safe_delete(playerModel);
	safe_delete(groundModel);
	safe_delete(title);
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

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/warningMark.png")) {
		assert(0);
	}
	// 前景スプライト生成

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(2, L"Resources/title.png")) {
		assert(0);
	}
	// 前景スプライト生成
	//タイトル
	title = Sprite::Create(2, { 0.0f,0.0f });
	title->SetSize({ 1280.0f,960.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(13, L"Resources/uiBack.png")) {
		assert(0);
	}
	//回数
	uiBack = Sprite::Create(13, { 0.0f,0.0f });
	uiBack->SetSize({ 1280.0f,960.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(14, L"Resources/GameOver.png")) {
		assert(0);
	}
	//リザルト
	gameOver = Sprite::Create(14, { 0.0f,0.0f });
	gameOver->SetSize({ 1280.0f,960.0f });

	//.objの名前を指定してモデルを読み込む
	playerModel = playerModel->CreateFromObject("punk");
	groundModel = groundModel->CreateFromObject("ground");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	groundObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる

	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, -45.0f, 0 });
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 15.0f, 15.0f, 15.0f });
}

void GameScene::Update() {
	Move();

	playerObj->Update();
	groundObj->Update();
}

void GameScene::resetPos() {
	playerObj->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	//XMFLOAT3 playerPos = playerObj->GetPosition();
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
	groundObj->Draw();
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

void GameScene::Move() {
	XMFLOAT3 playerPos = playerObj->GetPosition();
	XMFLOAT3 groundPos = groundObj->GetPosition();
	XMFLOAT3 cameraEye = Object3d::GetEye();
	XMFLOAT3 cameraTarget = Object3d::GetTarget();

	// オブジェクト移動
	// 移動後の座標を計算
	if (input->PushKey(DIK_W)) {
		playerPos.z += moveAmount;
		cameraEye.z += moveAmount;
		cameraTarget.z += moveAmount;
		playerPos.x -= moveAmount;
		cameraEye.x -= moveAmount;
		cameraTarget.x -= moveAmount;
	}
	else if (input->PushKey(DIK_S)) {
		playerPos.z -= moveAmount;
		cameraEye.z -= moveAmount;
		cameraTarget.z -= moveAmount;
		playerPos.x += moveAmount;
		cameraEye.x += moveAmount;
		cameraTarget.x += moveAmount;
	}
	if (input->PushKey(DIK_D)) {
		playerPos.x += moveAmount;
		cameraEye.x += moveAmount;
		cameraTarget.x += moveAmount;
		playerPos.z += moveAmount;
		cameraEye.z += moveAmount;
		cameraTarget.z += moveAmount;
	}
	else if (input->PushKey(DIK_A)) {
		playerPos.x -= moveAmount;
		cameraEye.x -= moveAmount;
		cameraTarget.x -= moveAmount;
		playerPos.z -= moveAmount;
		cameraEye.z -= moveAmount;
		cameraTarget.z -= moveAmount;
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
}