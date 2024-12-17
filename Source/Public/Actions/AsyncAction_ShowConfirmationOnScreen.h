#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintAsyncActionBase.h>

#include "AsyncAction_ShowConfirmationOnScreen.generated.h"

class UCommonGameDialogDescriptor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FResultDelegate, ECommonMessagingResult, Result, UCommonGameDialog *, DialogueWidget );

UCLASS()
class COMMONGAME_API UAsyncAction_ShowConfirmationOnScreen : public UBlueprintAsyncActionBase
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "world_context_object" ) )
    static UAsyncAction_ShowConfirmationOnScreen * ShowConfirmationOnScreenYesNo( UObject * world_context_object, FText title, FText message, TSubclassOf< UCommonGameDialog > dialogue_widget, int z_order = -1 );

    virtual void Activate() override;

private:
    void HandleConfirmationResult( ECommonMessagingResult confirmation_result );
    static UAsyncAction_ShowConfirmationOnScreen * CreateAction( UObject * world_context, UCommonGameDialogDescriptor * descriptor, const TSubclassOf< UCommonGameDialog > & dialogue_widget, int z_order );

public:
    UPROPERTY( BlueprintAssignable )
    FResultDelegate OnResult;

private:
    UPROPERTY( Transient )
    TObjectPtr< UObject > WorldContextObject;

    UPROPERTY( Transient )
    TObjectPtr< ULocalPlayer > TargetLocalPlayer;

    UPROPERTY( Transient )
    TObjectPtr< UCommonGameDialogDescriptor > Descriptor;

    UPROPERTY( Transient )
    int ZOrder = -1;

    UPROPERTY( Transient )
    TSubclassOf< UCommonGameDialog > DialogueWidgetClass;

    UPROPERTY( Transient )
    TObjectPtr< UCommonGameDialog > DialogueWidget;
};
