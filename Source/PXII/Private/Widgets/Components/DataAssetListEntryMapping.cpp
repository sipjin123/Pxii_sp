// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataAssetListEntryMapping.h"
#include "Utility/PXIILogUtility.h"

TSubclassOf<UPxiiListEntryBase> UDataAssetListEntryMapping::FindListEntryClassByDataObject(UPxiiListDataObjectBase* InDataObject) const
{
	if(!IsValid(InDataObject))
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Invalid data object"), *ThisClass::StaticClass()->GetName());
		return TSubclassOf<UPxiiListEntryBase>();
	}

	for(UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if(TSubclassOf<UPxiiListDataObjectBase> ConvertedDataObjectClass = TSubclassOf<UPxiiListDataObjectBase>(DataObjectClass))
		{
			if(DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<UPxiiListEntryBase>();
}