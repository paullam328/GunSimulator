// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Engine.h"

AShotgun::AShotgun() : AGun()
{
	cooldown = 1;
	range = 1000;
}

void AShotgun::FireBullet()
{
	Super::FireBullet();

	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);

	//for each bullet (Cartesian's spherical coord system):
	//float yxAngle = 0;
	//float zrAngle = 0;

	FVector fVector = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorForwardVector();
	if (DoTrace_Shotgun(&RV_Hit, &RV_TraceParams, -50, FVector(fVector.X, fVector.Y ,0.f)))
	{
		SpawnDecal(RV_Hit.ImpactPoint, RV_Hit.ImpactNormal.Rotation(), Cast<USceneComponent>(RV_Hit.GetComponent()));
	}


	if (DoTrace_Shotgun(&RV_Hit, &RV_TraceParams, 0, FVector(0, 0.002f, 0.002f)))
	{
		SpawnDecal(RV_Hit.ImpactPoint, RV_Hit.ImpactNormal.Rotation(), Cast<USceneComponent>(RV_Hit.GetComponent()));
	}


	if (DoTrace_Shotgun(&RV_Hit, &RV_TraceParams, 50, FVector(0, 0.002f, 0.002f)))
	{
		SpawnDecal(RV_Hit.ImpactPoint, RV_Hit.ImpactNormal.Rotation(), Cast<USceneComponent>(RV_Hit.GetComponent()));
	}

}

//Spherical coordinates system didn't work as intended...
//Try Rodrigues' Rotation Formula instead
//If not, try rotational matrices
//bool AShotgun::DoTrace_Shotgun(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, float yxAngle, float zrAngle)
bool AShotgun::DoTrace_Shotgun(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, float xyAngle, FVector verticalDir)
{
	Super::DoTrace(RV_Hit, RV_TraceParams);

	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = Muzzle->GetComponentLocation();
	// you need to add a uproperty to the header file for a float PlayerInteractionDistance
	// FVector End = Muzzle->GetComponentLocation() + GetCartesianForwardVector(Muzzle->GetForwardVector(), yxAngle, zrAngle)  * range;
	FVector Spread = GetRodriguesForwardVector(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorForwardVector(), xyAngle, verticalDir);
	float SpreadoutMultiplier = 10; //how far up/horizontal do you want the gun spread to be
	Spread = FVector(Spread.X / (SpreadoutMultiplier / 2), Spread.Y  * SpreadoutMultiplier, Spread.Z);
	FVector End = Muzzle->GetComponentLocation() + Spread * range;

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	FCollisionResponseParams ResponseParams;
	//  do the line trace
	//UKismetSystemLibrary::LineTraceSingle(GetWorld(),Start,End, 
	//bool DidTrace = ActorLineTraceSingle(
	//	*RV_Hit,        //result
	//	Start,        //start
	//	End,        //end
	//	ECC_WorldStatic,    //collision channel
	//	*RV_TraceParams
	//);

	bool DidTrace = GetWorld()->LineTraceSingleByChannel(
		*RV_Hit,
		Start,
		End,
		ECC_WorldStatic,
		*RV_TraceParams,
		ResponseParams
	);
		//	*RV_Hit,        //result
		//	Start,        //start
		//	End,        //end
		//	ECC_WorldStatic,    //collision channel
		//	*RV_TraceParams
		//);

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor(255, 0, 0),
		false,
		0.5f,
		0,
		5
	);

	return DidTrace;
}

#pragma optimize("", off)
FVector AShotgun::GetCartesianForwardVector(FVector fVector, float yxAngle, float zrAngle)
{
	//FVector y = fVector / FMath::Atan(yxAngle);
	//FVector z = FMath::Square(FMath::Cos(zrAngle)) * FMath::Square(fVector) 
	if (yxAngle == 0 && zrAngle == 0)
	{
		return fVector;
	}

	/*float zrRad = UKismetMathLibrary::GetPI() * zrAngle / 180;
	float yxRad = UKismetMathLibrary::GetPI() * yxAngle / 180;
	float radProd = FMath::Sin(zrRad) * FMath::Cos(yxRad);
	FVector p = fVector / radProd;
	p.Normalize();
	return p; //x = p sin(zrAngle) * cos(yxAngle)*/

	FVector y = fVector / FMath::Tan(zrAngle);
	//FVector z = FMath::Sqrt()

	return FVector(0, 0, 0);
}
FVector AShotgun::GetRodriguesForwardVector(FVector fVector, float xyAngle, FVector verticalDir)
{
	FVector rotVector = FVector(0, 0, 0);
	//rotVector
	
	FVector zDirVector = FVector(0, 0, 1);

	//below is for normalized vector in xz-axis
	FVector normalizedVerticalDir = verticalDir;
	normalizedVerticalDir.Normalize();

	//convert the angle below to radian:
	float xyRad = UKismetMathLibrary::GetPI() * xyAngle / 180;

	//assign variables:
	FVector k = zDirVector;
	FVector v = normalizedVerticalDir;
	FVector kvCrossProd = FVector::CrossProduct(k, v); //magnitude(a) * magnitude(b) sin xyAngle * n perpendicular vector
	float kvDotProd = FVector::DotProduct(k, v);
	
	rotVector = v * FMath::Cos(xyRad) + kvCrossProd * FMath::Sin(xyRad) + k * kvDotProd * (1 - FMath::Cos(xyRad));
	
	return rotVector;
}
#pragma optimize("", on)