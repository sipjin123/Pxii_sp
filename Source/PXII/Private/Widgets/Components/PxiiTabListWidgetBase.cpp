// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/PxiiTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/PxiiButtonBase.h"

void UPxiiTabListWidgetBase::RequestRegisterTab(const FName& InTabDataID, const FText& InTabDisplayName)
{
	// Registers and adds a new tab to the list that corresponds to a given widget instance. If not present in the linked switcher, it will be added.
	RegisterTab(InTabDataID, TabButtonEntryWidgetClass, nullptr);

	if(UPxiiButtonBase* FoundButton = Cast<UPxiiButtonBase>(GetTabButtonBaseByID(InTabDataID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
		FoundButton->SetButtonToolTipText(FText::FromString(TEXT("Page for ") + InTabDisplayName.ToString()));
	}
}

#if WITH_EDITOR	
void UPxiiTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(
			FText::FromString(TEXT("Variable TabButtonEntryWidgetClass is not assigned in the editor. Please assign a valid class. ") +
				GetClass()->GetName() +
				TEXT(" needs a valid entry widget class.")));
	}
}
#endif