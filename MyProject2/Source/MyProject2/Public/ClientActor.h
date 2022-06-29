// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MClient.h"
#include "ClientGameInstance.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ClientActor.generated.h"

UCLASS()
class MYPROJECT2_API AClientActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClientActor();
	MClient* client;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FDelegateHandle OnMessageHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
