// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiLoggerBPLibrary.h"

void UPxiiLoggerBPLibrary::PXIIPrintLog(ELogCategory LogCategory, EPXIILogVerbosity Verbosity, FString InString)
{
	switch (Verbosity)
	{
		case EPXIILogVerbosity::Log:
			PXII_LOG(LogCategory, Log, TEXT("%s"), *InString);
			break;

		case EPXIILogVerbosity::Display:
			PXII_LOG(LogCategory, Display, TEXT("%s"), *InString);
			break;

		case EPXIILogVerbosity::Warning:
			PXII_LOG(LogCategory, Warning, TEXT("%s"), *InString);
			break;

		case EPXIILogVerbosity::Error:
			PXII_LOG(LogCategory, Error, TEXT("%s"), *InString);
			break;

		case EPXIILogVerbosity::Verbose:
			PXII_LOG(LogCategory, Verbose, TEXT("%s"), *InString);
			break;

		case EPXIILogVerbosity::VeryVerbose:
			PXII_LOG(LogCategory, VeryVerbose, TEXT("%s"), *InString);
			break;
		case EPXIILogVerbosity::Fatal:
			PXII_LOG(LogCategory, Fatal, TEXT("%s"), *InString);
			break;
		default:
			PXII_LOG(LogCategory, Log, TEXT("%s"), *InString);
			break;
	}
}

void UPxiiLoggerBPLibrary::EnableLogCategories(TArray<ELogCategory> Category)
{
	for(ELogCategory cat : Category)
	{
		EnableLogCategory(cat);
	}
}

void UPxiiLoggerBPLibrary::DisableLogCategories(TArray<ELogCategory> Category)
{
	for(ELogCategory cat : Category)
	{
		DisableLogCategory(cat);
	}
}

void UPxiiLoggerBPLibrary::EnableLogCategory(ELogCategory Category)
{
	PXIILogUtility::Enable(Category);
}

void UPxiiLoggerBPLibrary::DisableLogCategory(ELogCategory Category)
{
	PXIILogUtility::Disable(Category);
}

