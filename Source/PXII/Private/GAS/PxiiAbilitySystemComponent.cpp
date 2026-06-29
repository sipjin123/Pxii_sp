// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiAbilitySystemComponent.h"
#include "Data/PxiiAbilityData.h"

DEFINE_LOG_CATEGORY(LogTempAbilityComp);
void UPxiiAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPxiiAbilitySystemComponent::GrantAbilityByRow(FName RowName)
{
	if(!AbilityDataTable)return;

	const FPxiiAbilityData* Row=AbilityDataTable->FindRow<FPxiiAbilityData>(RowName,TEXT("GrantAbilityByRow"));

	if(!Row||!Row->AbilityClass)return;

	GiveAbility(FGameplayAbilitySpec(Row->AbilityClass,Row->Level,INDEX_NONE));
}

void UPxiiAbilitySystemComponent::GrantAllAbilities()
{
	if(!AbilityDataTable)return;

	static const FString Context(TEXT("GrantAllAbilities"));

	TArray<FPxiiAbilityData*> Rows;
	AbilityDataTable->GetAllRows(Context,Rows);

	for(const FPxiiAbilityData* Row:Rows)
	{
		if(!Row||!Row->AbilityClass)continue;
		if (LogAbilityInit)
		{
			UE_LOG(LogTempAbilityComp,Warning,TEXT("GiveAbility: %s"),*Row->Name.ToString());
		}
		GiveAbility(FGameplayAbilitySpec(Row->AbilityClass,Row->Level,INDEX_NONE));
	}
}
