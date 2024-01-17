#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWItemWidget;
class UInventoryComponentBase;


/** 
 * UMG widget which represents inventory
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=UI)
class INVENTORYSYSTEM_API UWInventoryWidget: public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void SynchronizeProperties() override;

    /**
     * Init widget with pointer to inventory 
     */
    UFUNCTION(BlueprintCallable, Category="UI")
    virtual void Init(UInventoryComponentBase* InventoryComponent);
 
protected:
    virtual void NativeConstruct() override;

    /**
     * Class of item widget
     */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UWItemWidget> SlotWidgetClass;    
    
    UPROPERTY()
    UInventoryComponentBase* InventoryComp = nullptr;

    UPROPERTY()
    TMap<FGuid, UWItemWidget*> ItemWidgets;

    /**
     * Virtual function which calls when item was removed
     * @param ItemGuid Guid of removed item
     * @result Remove item widget with that guid
     */
    virtual void OnRemoveSlot(const FGuid& ItemGuid);

    /**
     * Virtual function which calls when item was added
     * @param ItemGuid Guid of added item
     * @result Add item widget with that guid
     */
    virtual void OnAddItemSlot(const FGuid& ItemGuid);

    /**
     * Virtual function which calls when item was changed
     * @param ItemGuid Guid of changed items
     * @result Change item widget with that guid
     */
    virtual void OnChangeItemSlot(const FGuid& ItemGuid);
};
