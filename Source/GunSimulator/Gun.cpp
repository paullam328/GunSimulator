// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMeshComponent->AttachTo(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->AttachTo(GunMeshComponent);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timer < cooldown + 1)
	{
		timer += DeltaTime;
	}
}

void AGun::PlaySound()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunFireSFX, GetActorLocation());
}

void AGun::OnTriggerFire()
{
	if (timer > cooldown)
	{
		Fire();
		timer = 0;
	}
}

void AGun::FireBullet()
{
}

bool AGun::DoTrace(FHitResult * RV_Hit, FCollisionQueryParams * RV_TraceParams)
{
	return false;
}

void AGun::SpawnDecal(FVector HitLoc, FRotator HitRot, USceneComponent* AttachComp)
{
	//ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(HitLoc, FRotator(HitRot.Pitch + 90,0,0));
	UGameplayStatics::SpawnDecalAttached(DecalMat, FVector(1.0f, 25.0f, 25.0f), AttachComp, NAME_None, HitLoc, HitRot, EAttachLocation::KeepWorldPosition, 5.0f);

	/*if (decal != nullptr)
	{
		decal->SetDecalMaterial(DecalMat);
		decal->SetLifeSpan(5.0f);
		decal->GetDecal()->DecalSize = FVector(25.0f, 25.0f, 25.0f);
	}*/
}

void AGun::Fire()
{
	FireBullet();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunFireSFX, GetActorLocation());
}
