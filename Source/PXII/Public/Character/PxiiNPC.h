// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiCharacterBase.h"
#include "GameFramework/Character.h"
#include "PxiiNPC.generated.h"

UCLASS()
class PXII_API APxiiNPC : public APxiiCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APxiiNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
