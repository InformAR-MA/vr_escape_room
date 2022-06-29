// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientActor.h"

// Sets default values
AClientActor::AClientActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClientActor::BeginPlay()
{
	Super::BeginPlay();


	UClientGameInstance* instance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance->client == nullptr) {
		client = new MClient("ws://zoezo.xyz:8080", "ws");
		instance->client = client;
	}
	else client = instance->client;

	UWorld* world = GetWorld();

	this->OnMessageHandle = client->OnMessage().AddLambda([world](FString sender, FString data) -> void {
		//FString current = UGameplayStatics::GetWorld();

		UE_LOG(LogTemp, Warning, TEXT("I RAN"));

		if (world == NULL) {
			UE_LOG(LogTemp, Warning, TEXT("SHATTED MYSELF"));
			return;
		}

		try {
			UGameplayStatics::OpenLevel(world, FName(*data));
		}
		catch (const std::exception&) {
			UE_LOG(LogTemp, Warning, TEXT("SOS"));
		}
		});
}

void AClientActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	this->client->OnMessage().Remove(this->OnMessageHandle);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AClientActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

