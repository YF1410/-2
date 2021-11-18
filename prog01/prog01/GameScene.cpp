#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {

	safe_delete(playerModel);
	safe_delete(player_attack_befor_Model);
	safe_delete(player_attack_after_Model);
	safe_delete(player_dash_Model);
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
	bossObj = Object3d::Create();
	groundObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる
	//player
	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, -45.0f, 0 });
	//stage
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 2.5f, 2.5f, 2.5f });
	groundObj->SetRotation({ 0, 180.0f, 0 });
	//boss
	bossObj->SetModel(bossModel);
	bossObj->SetPosition({ 30.0f, 0.0f, 50.0f });
	bossObj->SetScale({ 5.0f, 5.0f, 5.0f });
	bossObj->SetRotation({ 0, 180.0f, 0 });
}

void GameScene::Update() {
	Move();
	avoidance();
	if (input->PushKey(DIK_E)&&countdown==60)
	{
		boss_rush_flag = true;
		
	}
	if (boss_rush_flag == true)
	{
		countdown--;
		if (countdown>0)
		{
			bosspose = 1;

		}

	}
	rush();
	playerObj->Update();
	groundObj->Update();
	bossObj->Update();

	if (input->TriggerKey(DIK_Q) && attack_flag == false)
	{
		attack_flag = true;
	}
	if (attack_flag == true)
	{
		frame++;
	}
	if (frame < 24 && attack_flag == true)
	{
		charapose = 1;
	}
	else if (frame >= 24 && frame < 60 && attack_flag == true) {
		charapose = 2;
	}
	else if (frame >= 60 && attack_flag == true) {
		frame = 0;
		attack_flag = false;
		charapose = 0;
	}

	
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
	}
	else if (charapose == 1)
	{
		playerObj->SetModel(player_attack_befor_Model);
	}
	else if (charapose == 2)
	{
		playerObj->SetModel(player_attack_after_Model);
	}
	else if (charapose == 3)
	{
		playerObj->SetModel(player_dash_Model);

	}

	if (bosspose == 0)
	{
		bossObj->SetModel(bossModel);
	}
	else if (bosspose == 1)
	{
		bossObj->SetModel(boss_rush_befor_Model);
	}
	else if (bosspose == 2)
	{
		bossObj->SetModel(boss_rush_after_Model);
	}
	playerObj->Draw();
	groundObj->Draw();
	bossObj->Draw();
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
	XMFLOAT3 cameraEye = Object3d::GetEye();
	XMFLOAT3 cameraTarget = Object3d::GetTarget();

	// オブジェクト移動
	// 移動後の座標を計算
	if (avoidChange == 0) {
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
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
}

void GameScene::avoidance() {
	XMFLOAT3 playerPos = playerObj->GetPosition();
	XMFLOAT3 cameraEye = Object3d::GetEye();
	XMFLOAT3 cameraTarget = Object3d::GetTarget();
	if (input->TriggerKey(DIK_SPACE) && avoidChange == 0) {
		avoidChange = 1;
		startPlayerPos = playerPos;
		startCameraEye = cameraEye;
		startCameraTarget = cameraTarget;
		endPlayerPos = playerPos;
		endCameraEye = cameraEye;
		endCameraTarget = cameraTarget;
	}

	if (avoidChange != 0) {
		nowTime += 0.01;
		timeRate = min(nowTime / endTime, 1);
		charapose = 3;
	}

	if (avoidChange == 1) {
		if (input->PushKey(DIK_W)) {
			avoidChange = 2;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;

		}
		else if (input->PushKey(DIK_S)) {
			avoidChange = 3;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
		}
		if (input->PushKey(DIK_D)) {
			avoidChange = 4;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
		}
		else if (input->PushKey(DIK_A)) {
			avoidChange = 5;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
		}
	}
	if (avoidChange == 2) {
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, timeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, timeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, timeRate);
	}
	if (avoidChange == 3) {
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, timeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, timeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, timeRate);
	}
	if (avoidChange == 4) {
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, timeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, timeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, timeRate);
	}
	if (avoidChange == 5) {
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, timeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, timeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, timeRate);
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);


	if (timeRate == 1) {
		nowTime = 0;
		timeRate = 0;
		avoidChange = false;
		charapose = 0;
	}
}

void GameScene::rush() {
	XMFLOAT3 bossPos = bossObj->GetPosition();
	if (countdown<=0&& rushCange == 0) {
		rushCange = 1;
		startbossPos = bossPos;
		endbossPos = bossPos;
	}

	if (rushCange != 0) {
		nowTime += 0.01;
		timeRate = min(nowTime / rushendTime, 1);
		bossframe++;
		if (bossframe < 34)
		{
			bosspose = 1;
		}
	}

	if (rushCange == 1) {
		if (input->PushKey(DIK_UP)) {
			rushCange = 2;
			endbossPos.z = startbossPos.z + avoidMove;

			endbossPos.x = startbossPos.x - avoidMove;


		}
		else if (input->PushKey(DIK_DOWN)) {
			rushCange = 3;
			endbossPos.z = startbossPos.z - avoidMove;

			endbossPos.x = startbossPos.x + avoidMove;

		}
		if (input->PushKey(DIK_RIGHT)) {
			rushCange = 4;
			endbossPos.z = startbossPos.z + avoidMove;
			endbossPos.x = startbossPos.x + avoidMove;
		}
		else if (input->PushKey(DIK_LEFT)) {
			rushCange = 5;
			endbossPos.z = startbossPos.z - avoidMove;

			endbossPos.x = startbossPos.x - avoidMove;

		}
	}
	if (rushCange == 2) {
		bossPos = easeOutQuint(startbossPos, endbossPos, timeRate);

	}
	if (rushCange == 3) {
		bossPos = easeOutQuint(startbossPos, endbossPos, timeRate);

	}
	if (rushCange == 4) {
		bossPos = easeOutQuint(startbossPos, endbossPos, timeRate);

	}
	if (rushCange == 5) {
		bossPos = easeOutQuint(startbossPos, endbossPos, timeRate);

	}

	// 座標の変更を反映
	bossObj->SetPosition(bossPos);

	if (bossframe >= 34 && bossframe < 80)
	{
		bosspose = 2;
	}
	if (bossframe > 80)
	{
		
	}
	if (timeRate == 1) {
		nowTime = 0;
		timeRate = 0;
		rushCange = false;
		boss_rush_flag = false;
		bossframe = 0;
		bosspose = 0;
		countdown = 60;
	}
}

