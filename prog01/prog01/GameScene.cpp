#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	safe_delete(playerModel);
	safe_delete(groundModel);
	safe_delete(bossModel);
	safe_delete(title);
	safe_delete(uiBack);
	safe_delete(gameOver);
	safe_delete(player_attack_befor_Model);
	safe_delete(player_attack_after_Model);
	safe_delete(player_dash_Model);
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
	//player
	playerModel = playerModel->CreateFromObject("player");
	player_attack_befor_Model = player_attack_befor_Model->CreateFromObject("player_attack_befor");
	player_attack_after_Model = player_attack_after_Model->CreateFromObject("player_attack_after");
	player_dash_Model = player_dash_Model->CreateFromObject("player_dash");
	//boss
	bossModel = bossModel->CreateFromObject("boss");
	boss_rush_befor_Model = boss_rush_befor_Model->CreateFromObject("boss_rush_befor");
	boss_rush_after_Model = boss_rush_after_Model->CreateFromObject("boss_rush_after");
	//stage
	groundModel = groundModel->CreateFromObject("ground");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	groundObj = Object3d::Create();
	bossObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる

	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, -45.0f, 0 });
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 5.0f, 5.0f, 5.0f });
	groundObj->SetRotation({ 0, 180.0f, 0 });
	bossObj->SetModel(bossModel);
	bossObj->SetPosition({ 0.0f, 0.0f, 0.0f });
	bossObj->SetScale({ 5.0f, 5.0f, 5.0f });
	bossObj->SetRotation({ 0, 135.0f, 0 });

	playerPos = playerObj->GetPosition();
	playerRot = playerObj->GetRotation();
	bossPos = bossObj->GetPosition();
	bossRot = bossObj->GetRotation();
	cameraEye = Object3d::GetEye();
	cameraTarget = Object3d::GetTarget();
}

void GameScene::Update() {
	Move();
	bossRotation();
	avoidance();
	playerAttack();
	collision();
	bossAttack();

	playerObj->Update();
	groundObj->Update();
	bossObj->Update();
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
	if (charapose == 0)
	{
		playerObj->SetModel(playerModel);
	} else if (charapose == 1)
	{
		playerObj->SetModel(player_attack_befor_Model);
	} else if (charapose == 2)
	{
		playerObj->SetModel(player_attack_after_Model);
	} else if (charapose == 3)
	{
		playerObj->SetModel(player_dash_Model);
	}

	if (bosspose == 0)
	{
		bossObj->SetModel(bossModel);
	} else if (bosspose == 1)
	{
		bossObj->SetModel(boss_rush_befor_Model);
	} else if (bosspose == 2)
	{
		bossObj->SetModel(boss_rush_after_Model);
	}

	if (playerHp > 0) {
		playerObj->Draw();
	}
	groundObj->Draw();
	if (bossHp > 0) {
		bossObj->Draw();
	}
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
	// オブジェクト移動
	// 移動後の座標を計算
	if (playerMode == 0) {
		if (input->PushKey(DIK_W)) {
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerRot.y = -45.0f;
			if (input->PushKey(DIK_D)) {
				playerPos.z += moveAmount/2;
				cameraEye.z += moveAmount/2;
				cameraTarget.z += moveAmount/2;
				playerPos.x += moveAmount;
				cameraEye.x += moveAmount;
				cameraTarget.x += moveAmount;
				playerRot.y = 0.0f;
			} else if (input->PushKey(DIK_A)) {
				playerPos.x -= moveAmount/2;
				cameraEye.x -= moveAmount/2;
				cameraTarget.x -= moveAmount/2;
				playerPos.z -= moveAmount;
				cameraEye.z -= moveAmount;
				cameraTarget.z -= moveAmount;
				playerRot.y = -90.0f;
			}
		} 
		else if (input->PushKey(DIK_S)) {
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerRot.y = 135.0f;
			if (input->PushKey(DIK_D)) {
				playerPos.x += moveAmount/2;
				cameraEye.x += moveAmount/2;
				cameraTarget.x += moveAmount/2;
				playerPos.z += moveAmount;
				cameraEye.z += moveAmount;
				cameraTarget.z += moveAmount;
				playerRot.y = 90.0f;
			} else if (input->PushKey(DIK_A)) {
				playerPos.z -= moveAmount/2;
				cameraEye.z -= moveAmount/2;
				cameraTarget.z -= moveAmount/2;
				playerPos.x -= moveAmount;
				cameraEye.x -= moveAmount;
				cameraTarget.x -= moveAmount;
				playerRot.y = 180.0f;
			}
		} 
		else if (input->PushKey(DIK_D)) {
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerRot.y = 45.0f;
		} 
		else if (input->PushKey(DIK_A)) {
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerRot.y = -135.0f;
		}
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	playerObj->SetRotation(playerRot);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
}

void GameScene::avoidance() {
	if (input->TriggerKey(DIK_SPACE) && playerMode == 0) {
		playerMode = 1;
		startPlayerPos = playerPos;
		startCameraEye = cameraEye;
		startCameraTarget = cameraTarget;
		endPlayerPos = playerPos;
		endCameraEye = cameraEye;
		endCameraTarget = cameraTarget;
	}

	if (playerMode != 0) {
		avoidNowTime += 0.01f;
		avoidTimeRate = min(avoidNowTime / avoidEndTime, 1);
	}

	if (playerMode == 1) {
		if (input->PushKey(DIK_W)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			} else if (input->PushKey(DIK_A)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			}
		} else if (input->PushKey(DIK_S)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			} else if (input->PushKey(DIK_A)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			}
		} else if (input->PushKey(DIK_D)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
		} else if (input->PushKey(DIK_A)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
		}
	}
	if (playerMode == 2) {
		charapose = 3;
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, avoidTimeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, avoidTimeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, avoidTimeRate);
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);

	if (avoidTimeRate == 1) {
		avoidNowTime = 0;
		avoidTimeRate = 0;
		playerMode = 0;
		charapose = 0;
	}
}

void GameScene::playerAttack() {
	if (input->TriggerKey(DIK_Q) && attack_flag == false)
	{
		attack_flag = true;
		playerMode = 3;
	}
	if (attack_flag == true)
	{
		frame++;
	}

	if (frame < 24 && attack_flag == true)
	{
		charapose = 1;
	} else if (frame >= 24 && frame < 60 && attack_flag == true) {
		charapose = 2;

	} else if (frame >= 60 && attack_flag == true) {
		frame = 0;
		attack_flag = false;
		charapose = 0;
	}

	if (frame == 48 && collisionFlag == true) {
		bossHp -= 1;
	}
}

void GameScene::bossAttack() {
	int count = 0;
	if (input->TriggerKey(DIK_E) && countDown == 60) {
		boss_rush_flag = true;
	}

	if (boss_rush_flag == true)
	{
		countDown--;
		if (countDown > 0)
		{
			bosspose = 1;
		}
	}

	if (rushChange != 0) {
		rushNowTime += 0.01f;
		rushTimeRate = min(rushNowTime / rushEndTime, 1);
		bossframe++;
		if (bossframe < 34)
		{
			bosspose = 1;
		}
	}

	if (countDown <= 0 && rushChange == 0 && count == 0) {
		rushChange = 1;
		startBossPos = bossPos;
		endBossPos = playerPos;
		count += 1;
	}


	if (rushChange == 1) {
		bossPos = easeOutQuint(startBossPos, endBossPos, rushTimeRate);
	}

	if (bossframe >= 34 && bossframe < 80) {
		bosspose = 2;
	}

	// 座標の変更を反映
	bossObj->SetPosition(bossPos);

	if (rushTimeRate == 1) {
		rushNowTime = 0;
		rushTimeRate = 0;
		rushChange = false;
		boss_rush_flag = false;
		bossframe = 0;
		bosspose = 0;
		countDown = 60;
		count = 0;
	}
}

void GameScene::collision() {
	playerRadius = 15.0f;
	bossRadius = 15.0f;
	collisionX = playerPos.x - bossPos.x;
	collisionZ = playerPos.z - bossPos.z;
	Collision = sqrtf((collisionX * collisionX) + (collisionZ * collisionZ));

	if (Collision <= playerRadius + bossRadius) {
		collisionFlag = true;
	}
	else {
		collisionFlag = false;
	}
}

void GameScene::bossRotation() {
	XMFLOAT3 bossPos = bossObj->GetPosition();

	XMFLOAT3 playerPos = playerObj->GetPosition();

	float angleY = angle->PosForAngle(bossPos.x, bossPos.z, playerPos.x, playerPos.z);

	bossObj->SetRotation({ 0,XMConvertToDegrees(angleY), 0 });
}