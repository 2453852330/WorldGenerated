// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActor.h"

#include "ProceduralMeshComponent.h"


// Sets default values
AWorldActor::AWorldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	RootComponent = ProceduralMeshComponent;
}

// Called when the game starts or when spawned
void AWorldActor::BeginPlay()
{
	Super::BeginPlay();

	GeneratedWorldData();
}

void AWorldActor::GeneratedWorldData()
{
	NoiseLite = new FastNoiseLite();
	NoiseLite->SetSeed(Seed);
	NoiseLite->SetFrequency(Frequency);
	NoiseLite->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseLite->SetFractalType(FastNoiseLite::FractalType_FBm);


	// size 256 * 256 = 65536
	HeightMap.AddZeroed(WorldSize*WorldSize);
	
	for (int i = 0; i < WorldSize; ++i)
	{
		for (int j = 0; j < WorldSize; ++j)
		{
			// base noise range : -1 -> 1
			float base_noise = NoiseLite->GetNoise((float)i,(float)j);
			// 0 -> 1 , 100 is the world height
			HeightMap[i*WorldSize+j].X = i;
			HeightMap[i*WorldSize+j].Y = j;
			HeightMap[i*WorldSize+j].Z = ( base_noise + 1 ) / 2.f;
			
		}
	}

	TArray<FVector> Vertex_Buffer;
	Vertex_Buffer.Reserve(HeightMap.Num());
	
	TArray<int32> Index_Buffer;
	Index_Buffer.Reserve(WorldSize * WorldSize * 6);
	
	for (int32 i = 0 ; i < HeightMap.Num(); ++i)
	{
		FVector Pos = HeightMap[i] * MapOffset;
		Vertex_Buffer.Emplace(Pos);
	}

	for (int i = 0; i < WorldSize-1; ++i)
	{
		for (int j = 0; j < WorldSize-1; ++j)
		{
			int32 p1 = i*WorldSize+j; int32 p2 = i*WorldSize+j+1;
			int32 p3 = (i+1)*WorldSize+j+1 ; int32 p4 = (i+1)*WorldSize+j;
			Index_Buffer.Emplace(p2);
			Index_Buffer.Emplace(p3);
			Index_Buffer.Emplace(p1);
			Index_Buffer.Emplace(p3);
			Index_Buffer.Emplace(p4);
			Index_Buffer.Emplace(p1);
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(0,Vertex_Buffer,Index_Buffer,{},{},{},{},true);

	BP_OnCreateMeshFinish();
}
