// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PxiiPillarActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjHealthChanged, int32, CurrentHealth);
UCLASS()
class PXII_API APxiiPillarActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiPillarActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	int32 HitCount=0;

	UPROPERTY(BlueprintReadWrite,Category="Combat")
	AActor* LastInteractor;

	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TArray<AActor*> HitActors;

	UFUNCTION(BlueprintCallable)
	void UpdateLastInteractor(AActor* NewActor) { LastInteractor = NewActor; }
	
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	int32 MaxHitCount=0;

	UFUNCTION(BlueprintCallable,Category="Combat")
	void SetHitCount(int32 NewValue);

	UFUNCTION(BlueprintCallable,Category="Combat")
	void SetMaxHitCount(int32 NewValue);

	UFUNCTION(BlueprintImplementableEvent,Category="Combat")
	void OnHitCountChanged(int32 NewValue);

	UFUNCTION(BlueprintImplementableEvent,Category="Combat")
	void OnMaxHitCountChanged(int32 NewValue);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FObjHealthChanged ObjHealthChanged;
};
