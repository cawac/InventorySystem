#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class UInventoryComponentBase;

/**
 * UMG widget which represents item
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=UI)
class INVENTORYSYSTEM_API UWItemWidget: public UUserWidget
{
    GENERATED_BODY()
   
public:
    /**
     * Validating input data
     * @param InventoryComponent Pointer to inventory
     * @param Guid Item's guid
     * @return Returns result of validating
     */
    static bool ValidateItemInfo(const UInventoryComponentBase* InventoryComponent, const FGuid& Guid);
    
    virtual void SynchronizeProperties() override;

    /**
     * Initialize variables of object.
     * @param InventoryComponent Pointer to inventory
     * @param Guid Item's guid
     * @result Object with initialized variables
     */
    UFUNCTION(BlueprintCallable, Category="UI")
    virtual void Init(UInventoryComponentBase* InventoryComponent, const FGuid& Guid);

    /**
     * Refresh image and amount of items in UI
     */
    UFUNCTION(BlueprintCallable, Category="UI")
    virtual void RefreshItemSlot();
    
protected:
    virtual void NativeConstruct() override;

    /**
     * Image of item
     */
    UPROPERTY(BlueprintReadOnly, Category="UI", meta = (BindWidget))
    class UImage* ItemImage = nullptr;

    /**
     * Text with amount of items
     */
    UPROPERTY(BlueprintReadOnly, Category="UI", meta = (BindWidgetOptional))
    class UTextBlock* TextAmountOf = nullptr;

    /**
     * Item guid of representing items
     */
    UPROPERTY()
    FGuid ItemGuid;

    /**
     * Pointer to owner inventory   
     */
    UPROPERTY()
    UInventoryComponentBase* InventoryComp;
};
