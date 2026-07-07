// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/PxiiTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Layout/Margin.h"
#include "Components/HorizontalBoxSlot.h"

UPxiiButtonBase* UPxiiTabListWidgetBase::RequestRegisterTab(const FName& InTabDataID)
{
	return RegisterTab(InTabDataID);
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

UPxiiButtonBase* UPxiiTabListWidgetBase::RegisterTab(const FName& TabID)
{
	UPxiiButtonBase* CreatedTabButton = CreateWidget<UPxiiButtonBase>(GetWorld(), TabButtonEntryWidgetClass);


	UHorizontalBoxSlot* ChildSlot = HorizontalBox_TabsContainer->AddChildToHorizontalBox(CreatedTabButton);

	CreatedTabButton->SetShowToolTip(true);
	CreatedTabButton->SetButtonText(FText::FromName(TabID));
	CreatedTabButton->SetButtonToolTipText(FText::FromString(TEXT("Page for ") + TabID.ToString()));

	ChildSlot->SetPadding(FMargin(5.f, 0.f));

	return CreatedTabButton;
}