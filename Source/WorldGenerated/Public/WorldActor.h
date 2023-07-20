// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastNoiseLite.h"
#include "ProceduralMeshComponent.h"

#include "GameFramework/Actor.h"
#include "WorldActor.generated.h"

UCLASS()
class WORLDGENERATED_API AWorldActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldActor();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UProceduralMeshComponent * ProceduralMeshComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	int32 Seed;
	UPROPERTY(EditAnywhere)
	float Frequency = 0.2f;

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D * RT = nullptr;
	
	UPROPERTY(EditAnywhere)
	FVector MapOffset;
	
	UPROPERTY(EditAnywhere)
	int32 WorldSize = 1024;
	void GeneratedWorldData();

    FastNoiseLite * NoiseLite = nullptr;

	TArray<FVector> HeightMap;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCreateMeshFinish();
};
