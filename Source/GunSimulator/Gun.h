// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class AShotgun;
class UStaticMeshComponent;
class USceneComponent;
class USoundBase;
class UMaterialInterface;

UCLASS()
class GUNSIMULATOR_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunMeshComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShotgun> ShotgunClass;

	UPROPERTY(EditAnywhere)
	USoundBase* GunFireSFX;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnTriggerFire();


protected:

	virtual void FireBullet();

	virtual bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

	void SpawnDecal(FVector HitLoc, FRotator HitRot, USceneComponent* AttachComp);

	void Fire();

	float cooldown = 0;

	int range = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* DecalMat;

private:

	float timer = 0;

	void PlaySound();

};
