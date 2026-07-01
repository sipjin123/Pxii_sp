#pragma once
#include "PlayerInputManagerBase.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PxiiInputMappingManager.generated.h"

USTRUCT(BlueprintType)
struct FMappingContextEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<const UInputMappingContext> MappingContext = nullptr;

	UPROPERTY()
	int32 Priority = 0;
};


UCLASS()
class PXII_API UPxiiInputMappingManager : public UPlayerInputManagerBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FMappingContextEntry> ActiveMappings;
	
	bool AddMappingContext(const UInputMappingContext* MappingContext, int32 Priority);

	bool RemoveMappingContext(const UInputMappingContext* MappingContext);

	void ClearAllMappings();

	bool HasMappingContext(const UInputMappingContext* MappingContext) const;
};
