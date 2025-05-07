// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BP_FuncLib.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UBP_FuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CustomLibrary")
	static void RemoveSplitscreenPlayers();
};
