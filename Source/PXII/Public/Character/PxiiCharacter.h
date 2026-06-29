// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacterBase.h"
#include "PxiiCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTempCharacter, Log, All);
/**
 * 
 */
UCLASS()
class PXII_API APxiiCharacter : public APxiiCharacterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
