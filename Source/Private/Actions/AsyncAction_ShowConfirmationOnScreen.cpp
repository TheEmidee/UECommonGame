#include "Actions/AsyncAction_ShowConfirmationOnScreen.h"

#include "Actions/AsyncAction_ShowConfirmation.h"
#include "Messaging/CommonGameDialog.h"

UAsyncAction_ShowConfirmationOnScreen::UAsyncAction_ShowConfirmationOnScreen( const FObjectInitializer & object_initializer ) :
    Super( object_initializer )
{
}

UAsyncAction_ShowConfirmationOnScreen * UAsyncAction_ShowConfirmationOnScreen::ShowConfirmationOnScreenYesNo( UObject * world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > dialogue_widget, int z_order )
{
    return CreateAction( world_context_object, UCommonGameDialogDescriptor::CreateConfirmationYesNo( title, message ), dialogue_widget, z_order );
}

void UAsyncAction_ShowConfirmationOnScreen::Activate()
{
    if ( WorldContextObject && !TargetLocalPlayer )
    {
        if ( const auto * user_widget = Cast< UUserWidget >( WorldContextObject ) )
        {
            TargetLocalPlayer = user_widget->GetOwningLocalPlayer< ULocalPlayer >();
        }
        else if ( const auto * pc = Cast< APlayerController >( WorldContextObject ) )
        {
            TargetLocalPlayer = pc->GetLocalPlayer();
        }
        else if ( const auto * world = WorldContextObject->GetWorld() )
        {
            if ( const auto * game_instance = world->GetGameInstance< UGameInstance >() )
            {
                TargetLocalPlayer = game_instance->GetPrimaryPlayerController( false )->GetLocalPlayer();
            }
        }
    }

    if ( TargetLocalPlayer )
    {
        if ( TargetLocalPlayer->GetSubsystem< UCommonMessagingSubsystem >() )
        {
            FCommonMessagingResultDelegate result_callback = FCommonMessagingResultDelegate::CreateUObject( this, &UAsyncAction_ShowConfirmationOnScreen::HandleConfirmationResult );
            DialogueWidget = CreateWidget< UCommonGameDialog >( TargetLocalPlayer->PlayerController, DialogueWidgetClass );
            DialogueWidget->SetupDialog( Descriptor, result_callback );
            DialogueWidget->AddToViewport( ZOrder );
            DialogueWidget->ActivateWidget();
            return;
        }
    }

    HandleConfirmationResult( ECommonMessagingResult::Unknown );
}

void UAsyncAction_ShowConfirmationOnScreen::HandleConfirmationResult( ECommonMessagingResult confirmation_result )
{
    OnResult.Broadcast( confirmation_result, DialogueWidget );

    SetReadyToDestroy();
}

UAsyncAction_ShowConfirmationOnScreen * UAsyncAction_ShowConfirmationOnScreen::CreateAction( UObject * world_context, UCommonGameDialogDescriptor * descriptor, const TSubclassOf< UCommonGameDialog > & dialogue_widget, int z_order )
{
    auto * action = NewObject< UAsyncAction_ShowConfirmationOnScreen >();
    action->WorldContextObject = world_context;
    action->Descriptor = descriptor;
    action->DialogueWidgetClass = dialogue_widget;
    action->ZOrder = z_order;

    action->RegisterWithGameInstance( world_context );

    return action;
}