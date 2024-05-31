// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actions/AsyncAction_ShowConfirmation.h"

#include "Engine/GameInstance.h"
#include "Messaging/CommonGameDialog.h"
#include "Messaging/CommonMessagingSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_ShowConfirmation)

UAsyncAction_ShowConfirmation::UAsyncAction_ShowConfirmation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationYesNo(UObject* InWorldContextObject, FText Title, FText Message)
{
	return CreateAction(InWorldContextObject, UCommonGameDialogDescriptor::CreateConfirmationYesNo(Title, Message));
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationOkCancel(UObject* InWorldContextObject, FText Title, FText Message)
{
	return CreateAction(InWorldContextObject, UCommonGameDialogDescriptor::CreateConfirmationOkCancel(Title, Message));
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationCustom(UObject* InWorldContextObject, UCommonGameDialogDescriptor* Descriptor)
{
	return CreateAction(InWorldContextObject, Descriptor);
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetYesNo(UObject* InWorldContextObject, FText Title, FText Message, TSubclassOf<UCommonGameDialog> CustomDialogWidget)
{
	return CreateAction(InWorldContextObject, UCommonGameDialogDescriptor::CreateConfirmationYesNo(Title, Message), CustomDialogWidget);
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetOkCancel(UObject* InWorldContextObject, FText Title, FText Message, TSubclassOf<UCommonGameDialog> CustomDialogWidget)
{
	return CreateAction(InWorldContextObject, UCommonGameDialogDescriptor::CreateConfirmationOkCancel(Title, Message), CustomDialogWidget);
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetCustom(UObject* InWorldContextObject, UCommonGameDialogDescriptor* Descriptor, TSubclassOf<UCommonGameDialog> CustomDialogWidget)
{
	return CreateAction(InWorldContextObject, Descriptor, CustomDialogWidget);
}

void UAsyncAction_ShowConfirmation::Activate()
{
	if (WorldContextObject && !TargetLocalPlayer)
	{
		if (UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
		{
			TargetLocalPlayer = UserWidget->GetOwningLocalPlayer<ULocalPlayer>();
		}
		else if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
		{
			TargetLocalPlayer = PC->GetLocalPlayer();
		}
		else if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>())
			{
				TargetLocalPlayer = GameInstance->GetPrimaryPlayerController(false)->GetLocalPlayer();
			}
		}
	}

	if (TargetLocalPlayer)
	{
		if (UCommonMessagingSubsystem* Messaging = TargetLocalPlayer->GetSubsystem<UCommonMessagingSubsystem>())
		{
			FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate::CreateUObject(this, &UAsyncAction_ShowConfirmation::HandleConfirmationResult);
			Messaging->ShowConfirmation(Descriptor, ResultCallback);
			return;
		}
	}
	
	// If we couldn't make the confirmation, just handle an unknown result and broadcast nothing
	HandleConfirmationResult(ECommonMessagingResult::Unknown);
}

void UAsyncAction_ShowConfirmation::HandleConfirmationResult(ECommonMessagingResult ConfirmationResult)
{
	OnResult.Broadcast(ConfirmationResult);

	SetReadyToDestroy();
}

UAsyncAction_ShowConfirmation* UAsyncAction_ShowConfirmation::CreateAction(UObject* InWorldContext, UCommonGameDialogDescriptor* Descriptor, TSubclassOf<UCommonGameDialog> CustomDialogWidget)
{
	auto* action = NewObject< UAsyncAction_ShowConfirmation >();
	action->WorldContextObject = InWorldContext;
	action->Descriptor = Descriptor;
	action->CustomDialogWidget = CustomDialogWidget;

	action->RegisterWithGameInstance(InWorldContext);

	return action;
}