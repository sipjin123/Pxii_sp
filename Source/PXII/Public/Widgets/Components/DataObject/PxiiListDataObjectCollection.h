// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiListDataObjectCollection : public UPxiiListDataObjectBase
{
	GENERATED_BODY()

public:
	void AddChildListData(UPxiiListDataObjectBase* InChildListData);

	//~ Begin UPxiiListDataObjectBase Interface
	virtual TArray<UPxiiListDataObjectBase*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	//~ End UPxiiListDataObjectBase Interface

private:
	UPROPERTY(Transient)
	TArray<UPxiiListDataObjectBase*> ChildListDataArray;
};
