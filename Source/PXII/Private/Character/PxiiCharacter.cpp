// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacter.h"

DEFINE_LOG_CATEGORY(LogTempCharacter);
// Called when the game starts or when spawned
void APxiiCharacter::BeginPlay()
{
	UE_LOG(LogTempCharacter, Warning, TEXT("Char Init 4-Cpp"));
	Super::BeginPlay();
}