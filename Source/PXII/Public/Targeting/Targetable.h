#pragma once
#include "UObject/Interface.h"
#include "Targetable.generated.h"

UINTERFACE()
class PXII_API UTargetable : public UInterface
{
	GENERATED_BODY()
	
};

class ITargetable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	FVector GetAimSocketLocation() const;

	UFUNCTION(BlueprintNativeEvent)
	TArray<FVector> GetWeakpointLocations() const;

	UFUNCTION(BlueprintNativeEvent)
	bool IsLockable() const;

	UFUNCTION(BlueprintNativeEvent)
	float GetThreatPriority() const;	
};
