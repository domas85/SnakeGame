// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_FuncLib.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"


void UBP_FuncLib::RemoveSplitscreenPlayers()
{
	const int32 MaxSplitScreenPlayers = 4;
	ULocalPlayer* PlayersToRemove[MaxSplitScreenPlayers];
	int32 RemIndex = 0;
	UWorld* currentWorld = GEngine->GetWorld();

	for (FConstPlayerControllerIterator Iterator = GEngine->GameViewport->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		// Remove only local split screen players
		APlayerController* Controller = Iterator->Get();
		if (Controller && Controller->IsLocalController() && !Controller->IsPrimaryPlayer())
		{

			ULocalPlayer* ExPlayer = Cast<ULocalPlayer>(Controller->Player);
			if (ExPlayer)
			{
				PlayersToRemove[RemIndex++] = ExPlayer;
				Controller->PawnPendingDestroy(Controller->GetPawn());
				currentWorld = Controller->GetWorld();
			}
		}
	}

	if (!currentWorld)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Possibly Because there is no LocalPlayer"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GEngine->GetWorld() return NULL"));
		return;
	}

	UGameInstance* gameInstance = currentWorld->GetGameInstance();

	for (int32 i = 0; i < RemIndex; ++i)
	{
		gameInstance->RemoveLocalPlayer(PlayersToRemove[i]);
	}

}