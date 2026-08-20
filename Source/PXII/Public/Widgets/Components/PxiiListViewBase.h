// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "PxiiListEntryMapping.h"
#include "ListEntries/PxiiListEntryString.h"
#include "PxiiListViewBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiListViewBase : public UCommonListView
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetCachedDisplayResolutionListEntry(UPxiiListEntryString* InEntryScalar) { CachedDisplayResolutionListEntry = InEntryScalar; }
	UPxiiListEntryString* GetCachedDisplayResolutionListEntry() const { return CachedDisplayResolutionListEntry; }
	
protected:
	//~ Begin UCommonListView Interface
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	//~ End UCommonListView Interface

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	UPROPERTY(EditAnywhere, Category = "UI | List View")
	UPxiiListEntryMapping* ListEntryMapping;
	
	UPROPERTY()
	UPxiiListEntryString* CachedDisplayResolutionListEntry;
};
