#pragma once
#include "Utility/PXIILogUtility.h"
#include "Log/PxiiLogData.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "PxiiGameInstance.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPxiiLogData> LogData;

protected:

	virtual void Init() override;
};
