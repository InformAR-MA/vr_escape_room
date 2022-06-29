// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MClient.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UClientGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	MClient* client = nullptr;
	
};
