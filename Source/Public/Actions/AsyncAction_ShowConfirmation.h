// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"

#include "UObject/ObjectPtr.h"
#include "AsyncAction_ShowConfirmation.generated.h"

class UCommonGameDialog;
enum class ECommonMessagingResult : uint8;

class FText;
class UCommonGameDialogDescriptor;
class ULocalPlayer;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommonMessagingResultMCDelegate, ECommonMessagingResult, Result);

/**
* Allows easily triggering an async confirmation dialog in blueprints that you can then wait on the result.
 */
UCLASS()
class UAsyncAction_ShowConfirmation : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowConfirmation* ShowConfirmationYesNo(
		UObject* InWorldContextObject, FText Title, FText Message
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowConfirmation* ShowConfirmationOkCancel(
		UObject* InWorldContextObject, FText Title, FText Message
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowConfirmation* ShowConfirmationCustom(
		UObject* InWorldContextObject, UCommonGameDialogDescriptor* Descriptor
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject" ))
	static UAsyncAction_ShowConfirmation* ShowConfirmationWithCustomWidgetYesNo(
		UObject* InWorldContextObject, FText Title, FText Message, TSubclassOf<UCommonGameDialog> CustomDialogWidget 
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowConfirmation* ShowConfirmationWithCustomWidgetOkCancel(
		UObject* InWorldContextObject, FText Title, FText Message, TSubclassOf<UCommonGameDialog> CustomDialogWidget 
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowConfirmation* ShowConfirmationWithCustomWidgetCustom(
		UObject* InWorldContextObject, UCommonGameDialogDescriptor* descriptor, TSubclassOf<UCommonGameDialog> CustomDialogWidget
	);

	virtual void Activate() override;

public:
	UPROPERTY(BlueprintAssignable)
	FCommonMessagingResultMCDelegate OnResult;

private:
	void HandleConfirmationResult(ECommonMessagingResult ConfirmationResult);
	static UAsyncAction_ShowConfirmation* CreateAction(UObject* InWorldContext, UCommonGameDialogDescriptor* Descriptor, TSubclassOf<UCommonGameDialog> CustomDialogWidget = nullptr);

	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> TargetLocalPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UCommonGameDialogDescriptor> Descriptor;

	UPROPERTY( Transient )
	TSubclassOf< UCommonGameDialog > CustomDialogWidget;
};
