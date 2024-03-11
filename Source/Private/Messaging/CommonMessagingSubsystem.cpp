#include "Messaging/CommonMessagingSubsystem.h"

#include "Messaging/CommonGameDialog.h"

#include <Engine/GameInstance.h>
#include <Engine/LocalPlayer.h>
#include <UObject/UObjectHash.h>

class FSubsystemCollectionBase;
class UClass;

void UCommonMessagingSubsystem::Initialize( FSubsystemCollectionBase & Collection )
{
    Super::Initialize( Collection );
}

void UCommonMessagingSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

bool UCommonMessagingSubsystem::ShouldCreateSubsystem( UObject * Outer ) const
{
    if ( !CastChecked< ULocalPlayer >( Outer )->GetGameInstance()->IsDedicatedServerInstance() )
    {
        TArray< UClass * > ChildClasses;
        GetDerivedClasses( GetClass(), ChildClasses, false );

        // Only create an instance if there is no override implementation defined elsewhere
        return ChildClasses.Num() == 0;
    }

    return false;
}

void UCommonMessagingSubsystem::ShowConfirmation( UCommonGameDialogDescriptor * dialog_descriptor, TSubclassOf< UCommonGameDialog > custom_dialog_widget, FCommonMessagingResultDelegate result_callback )
{
}

void UCommonMessagingSubsystem::ShowError( UCommonGameDialogDescriptor * DialogDescriptor, FCommonMessagingResultDelegate ResultCallback )
{
}
