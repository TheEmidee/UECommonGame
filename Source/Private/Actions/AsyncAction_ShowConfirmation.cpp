// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actions/AsyncAction_ShowConfirmation.h"

#include "Engine/GameInstance.h"
#include "Messaging/CommonGameDialog.h"
#include "Messaging/CommonMessagingSubsystem.h"

UAsyncAction_ShowConfirmation::UAsyncAction_ShowConfirmation( const FObjectInitializer & ObjectInitializer ) :
    Super( ObjectInitializer )
{
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationYesNo( UObject * InWorldContextObject, FText Title, FText Message )
{
    UAsyncAction_ShowConfirmation * Action = NewObject< UAsyncAction_ShowConfirmation >();
    Action->WorldContextObject = InWorldContextObject;
    Action->Descriptor = UCommonGameDialogDescriptor::CreateConfirmationYesNo( Title, Message );
    Action->RegisterWithGameInstance( InWorldContextObject );

    return Action;
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetYesNo( UObject * in_world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    UAsyncAction_ShowConfirmation * action = NewObject< UAsyncAction_ShowConfirmation >();
    action->WorldContextObject = in_world_context_object;
    action->Descriptor = UCommonGameDialogDescriptor::CreateConfirmationYesNo( title, message );
    action->RegisterWithGameInstance( in_world_context_object );
    action->CustomDialogWidget = custom_dialog_widget;

    return action;
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationOkCancel( UObject * InWorldContextObject, FText Title, FText Message )
{
    UAsyncAction_ShowConfirmation * Action = NewObject< UAsyncAction_ShowConfirmation >();
    Action->WorldContextObject = InWorldContextObject;
    Action->Descriptor = UCommonGameDialogDescriptor::CreateConfirmationOkCancel( Title, Message );
    Action->RegisterWithGameInstance( InWorldContextObject );

    return Action;
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetOkCancel( UObject * in_world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    UAsyncAction_ShowConfirmation * action = NewObject< UAsyncAction_ShowConfirmation >();
    action->WorldContextObject = in_world_context_object;
    action->Descriptor = UCommonGameDialogDescriptor::CreateConfirmationOkCancel( title, message );
    action->RegisterWithGameInstance( in_world_context_object );
    action->CustomDialogWidget = custom_dialog_widget;

    return action;
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationCustom( UObject * InWorldContextObject, UCommonGameDialogDescriptor * Descriptor )
{
    UAsyncAction_ShowConfirmation * Action = NewObject< UAsyncAction_ShowConfirmation >();
    Action->WorldContextObject = InWorldContextObject;
    Action->Descriptor = Descriptor;
    Action->RegisterWithGameInstance( InWorldContextObject );

    return Action;
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetCustom( UObject * in_world_context_object, UCommonGameDialogDescriptor * descriptor, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    UAsyncAction_ShowConfirmation * action = NewObject< UAsyncAction_ShowConfirmation >();
    action->WorldContextObject = in_world_context_object;
    action->Descriptor = descriptor;
    action->RegisterWithGameInstance( in_world_context_object );
    action->CustomDialogWidget = custom_dialog_widget;

    return action;
}

void UAsyncAction_ShowConfirmation::Activate()
{
    if ( WorldContextObject && !TargetLocalPlayer )
    {
        if ( UUserWidget * UserWidget = Cast< UUserWidget >( WorldContextObject ) )
        {
            TargetLocalPlayer = UserWidget->GetOwningLocalPlayer< ULocalPlayer >();
        }
        else if ( APlayerController * PC = Cast< APlayerController >( WorldContextObject ) )
        {
            TargetLocalPlayer = PC->GetLocalPlayer();
        }
        else if ( UWorld * World = WorldContextObject->GetWorld() )
        {
            if ( UGameInstance * GameInstance = World->GetGameInstance< UGameInstance >() )
            {
                TargetLocalPlayer = GameInstance->GetPrimaryPlayerController( false )->GetLocalPlayer();
            }
        }
    }

    if ( TargetLocalPlayer )
    {
        if ( UCommonMessagingSubsystem * Messaging = TargetLocalPlayer->GetSubsystem< UCommonMessagingSubsystem >() )
        {
            FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate::CreateUObject( this, &UAsyncAction_ShowConfirmation::HandleConfirmationResult );

            Messaging->ShowConfirmation( Descriptor, CustomDialogWidget, ResultCallback );

            return;
        }
    }

    // If we couldn't make the confirmation, just handle an unknown result and broadcast nothing
    HandleConfirmationResult( ECommonMessagingResult::Unknown );
}

void UAsyncAction_ShowConfirmation::HandleConfirmationResult( ECommonMessagingResult ConfirmationResult )
{
    OnResult.Broadcast( ConfirmationResult );

    SetReadyToDestroy();
}
