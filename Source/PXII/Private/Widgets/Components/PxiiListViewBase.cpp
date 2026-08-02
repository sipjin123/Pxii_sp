// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/PxiiListViewBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Utility/PXIILogUtility.h"

UUserWidget& UPxiiListViewBase::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if(IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	UPxiiListDataObjectBase* DataObject = Cast<UPxiiListDataObjectBase>(Item);
	if(!DataObject)
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: An item failed to cast to desired class {UPxiiListDataObject}"), *ThisClass::StaticClass()->GetName());
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UPxiiListEntryBase> FoundClass = ListEntryMapping->FindListEntryClassByDataObject(DataObject))
	{
		return GenerateTypedEntry<UPxiiListEntryBase>(FoundClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}

#if WITH_EDITOR	
void UPxiiListViewBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if(!ListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("ListEntryMapping is not assigned a valid data asset")));
	}
}
#endif
