// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PXIIAnimNotifyStateSpawnAnimationActor.generated.h"

/**
 * 
 */
UCLASS(Abstract, Const, HideCategories = Object, CollapseCategories)
class PXII_API UPXIIAnimNotifyStateSpawnAnimationActor : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPXIIAnimNotifyStateSpawnAnimationActor();
	
	virtual FString GetNotifyName_Implementation() const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (AllowedClasses = "/Script/Engine.SkeletalMesh /Script/Engine.StaticMesh"))
	TObjectPtr<UStreamableRenderAsset> MeshToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify")
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditCondition = "!bUseWorldPosition"))
	bool bIsAttached;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify")
	bool bUseWorldPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditCondition = "!bUseWorldPosition", EditConditionHides = "true"))
	FTransform SpawnTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditCondition = "bUseWorldPosition", EditConditionHides = "true"))
	FVector PositionOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify")
	uint32 bOverrideMaterials : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditCondition = "bOverrideMaterials", EditConditionHides = "true", ScriptName = "MaterialOverrides"))
	TArray<TObjectPtr<UMaterialInterface>> OverrideMaterials;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditCondition = "bOverrideMaterials", EditConditionHides = "true"))
	TObjectPtr<UMaterialInterface> OverlayMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditConditionHides))
	TObjectPtr<UAnimationAsset> ActorAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Notify", Meta = (EditConditionHides))
	bool bActorAnimationLoops;
	
private:
	UPROPERTY(Transient)
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, TObjectPtr<AActor>> SpawnedActors;

	void ApplyMaterialOverrides(UMeshComponent* MeshComp);
	
#if WITH_EDITOR
 
public:
	// Disables animation properties if the selected mesh is not a skeletal mesh.
	virtual bool CanEditChange(const FProperty* InProperty) const override;

#endif // WITH_EDITOR
};
