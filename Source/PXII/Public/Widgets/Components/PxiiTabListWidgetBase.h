// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PxiiWidgetBase.h"
#include "Widgets/Components/PxiiButtonBase.h"
#include "Components/HorizontalBox.h"
#include "PxiiTabListWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiTabListWidgetBase : public UPxiiWidgetBase
{
	GENERATED_BODY()
	
public:
	UPxiiButtonBase* RequestRegisterTab(const FName& InTabDataID);

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	// Debug
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_TabsContainer;

	UPxiiButtonBase* RegisterTab(const FName& TabID);
	// Debug

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Tab List", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Tab List", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPxiiButtonBase> TabButtonEntryWidgetClass;
};
