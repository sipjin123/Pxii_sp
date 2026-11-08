// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifies/PXIIAnimNotifyStateSpawnAnimationActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/StaticMeshActor.h"

UPXIIAnimNotifyStateSpawnAnimationActor::UPXIIAnimNotifyStateSpawnAnimationActor() :
	bIsAttached(false),
	bOverrideMaterials(0),
	bActorAnimationLoops(false)
{
	
}

FString UPXIIAnimNotifyStateSpawnAnimationActor::GetNotifyName_Implementation() const
{
	if (MeshToSpawn)
	{
		return ("Spawn " + GetNameSafe(MeshToSpawn));
	}
	else
	{
		return "Spawn Animation Actor (Unset)";
	}
}

void UPXIIAnimNotifyStateSpawnAnimationActor::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}
	
	if (!MeshToSpawn || !MeshComp)
	{
		if (!World->IsPreviewWorld())
		{
			UE_LOG(LogAnimation, Error, TEXT("Attempted to spawn animation actor in animation (%s), but no mesh to spawn was specified."), *GetNameSafe(Animation));
		}
		
		return;
	}
	
	// Only spawn once per mesh
	if (SpawnedActors.Contains(MeshComp))
	{
		return;
	}
	
	UMeshComponent* SpawnedMeshComp = nullptr;
	AActor* SpawnedActor = nullptr;
	
	if (bUseWorldPosition)
	{
		bIsAttached = false;
		SpawnTransform = MeshComp->GetSocketTransform(SocketName);
		
		if (PositionOffset != FVector::ZeroVector)
		{
			SpawnTransform.SetLocation(SpawnTransform.GetLocation() + PositionOffset);
		}
	}
	
	if (MeshToSpawn->IsA(UStaticMesh::StaticClass()))
	{
		if (AStaticMeshActor* SpawnedStaticMeshActor = World->SpawnActorDeferred<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnTransform, MeshComp->GetOwner()))
		{
			SpawnedStaticMeshActor->SetMobility(EComponentMobility::Movable);
			SpawnedStaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(Cast<UStaticMesh>(MeshToSpawn));
			SpawnedActor = SpawnedStaticMeshActor;
			SpawnedMeshComp = SpawnedStaticMeshActor->GetStaticMeshComponent();
		}
	}
	else
	{
		if (ASkeletalMeshActor* SpawnedSkeletalMeshActor = World->SpawnActorDeferred<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, MeshComp->GetOwner()))
		{
			SpawnedSkeletalMeshActor->GetSkeletalMeshComponent()->SetSkeletalMesh(Cast<USkeletalMesh>(MeshToSpawn));
			SpawnedActor = SpawnedSkeletalMeshActor;
			SpawnedMeshComp = SpawnedSkeletalMeshActor->GetSkeletalMeshComponent();
		}
	}
	
	if (SpawnedActor)
	{
		SpawnedActor->SetActorEnableCollision(false);
		if (bIsAttached)
		{
			SpawnedActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		}
		ApplyMaterialOverrides(SpawnedMeshComp);
		
		if (ActorAnimation != nullptr)
		{
			if (USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(SpawnedMeshComp))
			{
				SkeletalMeshComponent->PlayAnimation(ActorAnimation, bActorAnimationLoops);
			}
		}
		
		SpawnedActor->FinishSpawning(SpawnTransform);
		SpawnedActors.Add(MeshComp, SpawnedActor);
	}
}

void UPXIIAnimNotifyStateSpawnAnimationActor::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* SpawnedActor = *SpawnedActors.Find(MeshComp);
	SpawnedActors.Remove(MeshComp);
	
	if (IsValid(SpawnedActor))
	{
		SpawnedActor->SetLifeSpan(1.f);
		SpawnedActor->ForEachComponent(false, [](UActorComponent* InComponent)
		{
			if (UMeshComponent* Comp = Cast<UMeshComponent>(InComponent))
			{
				Comp->SetHiddenInGame(true, false);
			}	
		});
	}
}

void UPXIIAnimNotifyStateSpawnAnimationActor::ApplyMaterialOverrides(UMeshComponent* MeshComp)
{
	if (bOverrideMaterials && IsValid(MeshComp))
	{
		const int32 NumOfOverrideMaterials = FMath::Min(OverrideMaterials.Num(), MeshComp->GetNumMaterials());
		for (int32 i = 0; i < NumOfOverrideMaterials; ++i)
		{
			if (UMaterialInterface* OverrideMaterial = OverrideMaterials[i])
			{
				MeshComp->SetMaterial(i, OverrideMaterial);
			}
		}
		
		if (OverlayMaterial)
		{
			MeshComp->SetOverlayMaterial(OverlayMaterial);
		}
	}
}

#if WITH_EDITOR
bool UPXIIAnimNotifyStateSpawnAnimationActor::CanEditChange(const FProperty* InProperty) const
{
	bool bIsEdiatble = Super::CanEditChange(InProperty);
	
	if (bIsEdiatble && InProperty)
	{
		const FName PropertyName = InProperty->GetFName();
		
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UPXIIAnimNotifyStateSpawnAnimationActor, ActorAnimation) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(UPXIIAnimNotifyStateSpawnAnimationActor, bActorAnimationLoops))
		{
			bIsEdiatble = IsValid(MeshToSpawn) && MeshToSpawn->IsA(USkeletalMesh::StaticClass());
		}
	}
	
	return bIsEdiatble;
}
#endif // WITH_EDITOR
