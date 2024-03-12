#pragma once

#include <Kismet/BlueprintAsyncActionBase.h>
#include <UObject/ObjectPtr.h>

#include "AsyncAction_ShowConfirmation.generated.h"

enum class ECommonMessagingResult : uint8;

class FText;
class ULocalPlayer;
class UCommonGameDialog;
class UCommonGameDialogDescriptor;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FCommonMessagingResultMCDelegate, ECommonMessagingResult, Result );

/**
 * Allows easily triggering an async confirmation dialog in blueprints that you can then wait on the result.
 */
UCLASS()
class COMMONGAME_API UAsyncAction_ShowConfirmation : public UBlueprintAsyncActionBase
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationYesNo(
        UObject * in_world_context_object,
        FText title,
        FText message );

    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationOkCancel(
        UObject * in_world_context_object,
        FText title,
        FText message );

    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationCustom(
        UObject * in_world_context_object,
        UCommonGameDialogDescriptor * descriptor );

    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationWithCustomWidgetYesNo(
        UObject * in_world_context_object,
        FText title,
        FText message,
        TSubclassOf< UCommonGameDialog > custom_dialog_widget );

    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationWithCustomWidgetOkCancel(
        UObject * in_world_context_object,
        FText title,
        FText message,
        TSubclassOf< UCommonGameDialog > custom_dialog_widget );

    UFUNCTION( BlueprintCallable, BlueprintCosmetic, meta = ( BlueprintInternalUseOnly = "true", WorldContext = "in_world_context_object" ) )
    static UAsyncAction_ShowConfirmation * ShowConfirmationWithCustomWidgetCustom(
        UObject * in_world_context_object,
        UCommonGameDialogDescriptor * descriptor,
        TSubclassOf< UCommonGameDialog > custom_dialog_widget );

    virtual void Activate() override;

public:
    UPROPERTY( BlueprintAssignable )
    FCommonMessagingResultMCDelegate OnResult;

private:
    void HandleConfirmationResult( ECommonMessagingResult confirmation_result );
    static UAsyncAction_ShowConfirmation * CreateAction( UObject * in_world_context, UCommonGameDialogDescriptor * descriptor, TSubclassOf< UCommonGameDialog > custom_dialog_widget = nullptr );

    UPROPERTY( Transient )
    TObjectPtr< UObject > WorldContextObject;

    UPROPERTY( Transient )
    TObjectPtr< ULocalPlayer > TargetLocalPlayer;

    UPROPERTY( Transient )
    TObjectPtr< UCommonGameDialogDescriptor > Descriptor;

    UPROPERTY( Transient )
    TSubclassOf< UCommonGameDialog > CustomDialogWidget;
};