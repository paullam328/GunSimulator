// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class GUNSIMULATOR_API AShotgun : public AGun
{
	GENERATED_BODY()

public:

	AShotgun();

	virtual void FireBullet() override;

	//bool DoTrace_Shotgun(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, float yxAngle, float zrAngle);

	bool DoTrace_Shotgun(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, float xyAngle, FVector verticalDir);

	FVector GetCartesianForwardVector(FVector fVector, float yxAngle, float zrAngle);

	FVector GetRodriguesForwardVector(FVector fVector, float xyAngle, FVector verticalDir);

	//for each bullet:
	//float yxAngle = 0;
	//float zrAngle = 0;

};
