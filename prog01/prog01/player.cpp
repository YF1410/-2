#include "player.h"

void player::PlayerMove(Object3d* object, Input* input)
{
	PlayerPos = PlayerObj->GetPosition();
	//�O�ړ�
	if (input->TriggerKey(DIK_W))
	{
		PlayerPos.x += 0.5;
	}
	//���ړ�
	else if (input->TriggerKey(DIK_S))
	{
		PlayerPos.x -= 0.5;
	}
	//���ړ�
	if (input->TriggerKey(DIK_A))
	{
		PlayerPos.y -= 0.5;
	}
	//�E�ړ�
	else if (input->TriggerKey(DIK_D))
	{
		PlayerPos.y += 0.5;
	}
	PlayerObj->SetPosition(PlayerPos);
}

void player::Initialize()
{
	
	

}

void player::Update()
{
	
}

void player::Draw()
{
	PlayerObj->Draw();
}
