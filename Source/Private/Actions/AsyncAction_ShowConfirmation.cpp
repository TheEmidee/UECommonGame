#include "Actions/AsyncAction_ShowConfirmation.h"

#include "Messaging/CommonGameDialog.h"
#include "Messaging/CommonMessagingSubsystem.h"

#include <Engine/GameInstance.h>

UAsyncAction_ShowConfirmation::UAsyncAction_ShowConfirmation( const FObjectInitializer & object_initializer ) :
    Super( object_initializer )
{
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationYesNo( UObject * in_world_context_object, FText title, FText message )
{
    return CreateAction( in_world_context_object, UCommonGameDialogDescriptor::CreateConfirmationYesNo( title, message ) );
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetYesNo( UObject * in_world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    return CreateAction( in_world_context_object, UCommonGameDialogDescriptor::CreateConfirmationYesNo( title, message ), custom_dialog_widget );
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationOkCancel( UObject * in_world_context_object, FText title, FText message )
{
    return CreateAction( in_world_context_object, UCommonGameDialogDescriptor::CreateConfirmationOkCancel( title, message ) );
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetOkCancel( UObject * in_world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    return CreateAction( in_world_context_object, UCommonGameDialogDescriptor::CreateConfirmationOkCancel( title, message ), custom_dialog_widget );
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationCustom( UObject * in_world_context_object, UCommonGameDialogDescriptor * descriptor )
{
    return CreateAction( in_world_context_object, descriptor );
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::ShowConfirmationWithCustomWidgetCustom( UObject * in_world_context_object, UCommonGameDialogDescriptor * descriptor, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    return CreateAction( in_world_context_object, descriptor, custom_dialog_widget );
}

void UAsyncAction_ShowConfirmation::Activate()
{
    if ( WorldContextObject && !TargetLocalPlayer )
    {
        if ( UUserWidget * user_widget = Cast< UUserWidget >( WorldContextObject ) )
        {
            TargetLocalPlayer = user_widget->GetOwningLocalPlayer< ULocalPlayer >();
        }
        else if ( APlayerController * player_controller = Cast< APlayerController >( WorldContextObject ) )
        {
            TargetLocalPlayer = player_controller->GetLocalPlayer();
        }
        else if ( UWorld * world = WorldContextObject->GetWorld() )
        {
            if ( UGameInstance * game_instance = world->GetGameInstance< UGameInstance >() )
            {
                TargetLocalPlayer = game_instance->GetPrimaryPlayerController( false )->GetLocalPlayer();
            }
        }
    }

    if ( TargetLocalPlayer )
    {
        if ( auto * messaging = TargetLocalPlayer->GetSubsystem< UCommonMessagingSubsystem >() )
        {
            auto result_callback = FCommonMessagingResultDelegate::CreateUObject( this, &UAsyncAction_ShowConfirmation::HandleConfirmationResult );

            messaging->ShowConfirmation( Descriptor, CustomDialogWidget, result_callback );

            return;
        }
    }

    // If we couldn't make the confirmation, just handle an unknown result and broadcast nothing
    HandleConfirmationResult( ECommonMessagingResult::Unknown );
}

void UAsyncAction_ShowConfirmation::HandleConfirmationResult( ECommonMessagingResult confirmation_result )
{
    OnResult.Broadcast( confirmation_result );

    SetReadyToDestroy();
}

UAsyncAction_ShowConfirmation * UAsyncAction_ShowConfirmation::CreateAction( UObject * in_world_context, UCommonGameDialogDescriptor * descriptor, TSubclassOf< UCommonGameDialog > custom_dialog_widget )
{
    auto * action = NewObject< UAsyncAction_ShowConfirmation >();
    action->WorldContextObject = in_world_context;
    action->Descriptor = descriptor;
    action->CustomDialogWidget = custom_dialog_widget;

    action->RegisterWithGameInstance( in_world_context );

    return action;
}