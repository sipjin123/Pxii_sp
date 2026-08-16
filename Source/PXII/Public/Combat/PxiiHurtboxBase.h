// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/PxiiDamageType.h"
#include "Interface/PxiiDamageableInterface.h"
#include "PxiiHurtboxBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPartHit, AActor*, Source, float, Magnitude, int32, Payload);
UCLASS()
class PXII_API APxiiHurtboxBase : public AActor, public IPxiiDamageableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiHurtboxBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamage_Implementation(AActor* Source, float Magnitude, int32 Payload) override;

	virtual EDamagePart GetPartType_Implementation() override;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPartHit OnPartHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomSettings")
	EDamagePart DamagePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomSettings")
	FName BoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomSettings")
	TObjectPtr<UStaticMesh> MeshRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomSettings")
	bool SnapToBone = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomSettings")
	bool IsVisible = true;
	
	virtual APawn* GetPawnOwner_Implementation() override;
};
