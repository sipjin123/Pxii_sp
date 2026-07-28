// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PxiiPlayerCombatComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPxiiYinYangType : uint8
{
	Yin,
	Yang
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiPlayerCombatComponent : public UPxiiCombatComponent
{
	GENERATED_BODY()
public:
	UPxiiPlayerCombatComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GrantYinYang(EPxiiYinYangType Type, float Amount, bool bIsPercent);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_YinYang;
};
