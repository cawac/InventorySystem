#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/DataTypes/ItemSlot.h"
#include "InventoryComponentBase.generated.h"

class UItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveSlotSignature, const FGuid&, ItemGuid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddSlotSignature, const FGuid&, ItemGuid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeSlotSignature, const FGuid&, ItemGuid);

/**
 * InventoryСomponent is base class for storing items and interacting with this storage
 */
UCLASS(ClassGroup=(Managers), meta=(BlueprintSpawnableComponent, ShortTooltip="An Inventory is a reusable component that can be added to any actor for storaging items.") )
class INVENTORYSYSTEM_API UInventoryComponentBase: public UActorComponent  
{
    GENERATED_BODY()

public:
    /**
     * Default constructor
     */
    UInventoryComponentBase();

    /**
     * Constructor which specifies size of inventory storage
     */
    //UInventoryComponentBase(const int32 Size): InventorySize(Size) { UInventoryComponentBase(); }
    
    /**
     * Function for validating item data and amount of items
     * @param ItemData Item data to validate
     * @param AmountOf Amount of items to validate
     * @result Result of validation
     */
    static bool ValidateInputs(const UItemData* ItemData, const int32& AmountOf);
    
    /**
     * Virtual function for adding items to storage. For use from blueprints
     * @param ItemData Item data to add
     * @param AmountOf Amount of items to add
     * @return Returns array of guids, which mean guids of added items.
     * Returns empty array if adding all items is impossible
     */
    UFUNCTION(BlueprintCallable)
    virtual TArray<FGuid> AddItems(const UItemData* ItemData, int32 AmountOf);
    
    /**
     * Virtual function for removing items from storage. For use from blueprints
     * @param ItemData Item data to remove
     * @param AmountOf Amount of items to remove
     * @return Returns result of removing (true = items remove), (false = haven't enough items)
     */
    UFUNCTION(BlueprintCallable)
    virtual bool RemoveItems(const UItemData* ItemData, int32 AmountOf);

protected:
    virtual void BeginPlay() override;
    
    /**
     * Add items to existing slots of inventory storage
     * @param ItemData Adding item data
     * @param AmountOf Adding amount of items 
     * @return Returns guids array of added items.
     * Return empty array, if items weren't be added
     */
    TArray<FGuid> AddItemToExistingSlot(const UItemData* ItemData, int32& AmountOf);

    /**
     * Add items to empty slots of inventory storage
     * @param ItemData Adding item data
     * @param AmountOf Adding amount of items
     * @return Returns guids array of added items.
     * Return empty array, if items weren't be added
     */
    TArray<FGuid> AddItemToAvailableSlot(const UItemData* ItemData, int32& AmountOf);
    
    /**
     * Used to create item
     * @return Returns one item slot
     */
    FItemSlot CreateItemSlot(const UItemData* ItemData, int32 AmountOf);

    /**
     * Calculates amount of items with equal data
     * @param ItemData Item data to calculate
     * @return Amount of items
     */
    int32 CalculateAmountOfItem(const UItemData* ItemData) const;

    /**
     * Calculates max amount of items which can be added
     * @param ItemData Item data to calculate
     * @return Amount of space for items
     */
    int32 CalculateSpaceForItem(const UItemData* ItemData) const;

    /**
     * Size of inventory storage
     */
    UPROPERTY(EditDefaultsOnly)
    int32 InventorySize = 0;

    /**
     * Inventory storage of items
     */
    TArray<FItemSlot> InventoryStorage;

public:
    /**
     * Function for get item from item's guid
     * @param Guid Item's guid
     * @return Returns item with that guid, if it exists.
     * Returns nullptr, if it isn't exists
     */
    UFUNCTION(BlueprintCallable)
    FItemSlot GetItemSlot(const FGuid& Guid);

    /**
     * @return Returns inventory storage
     */
    UFUNCTION(BlueprintCallable)
    TArray<FItemSlot> GetInventoryStorage() { return InventoryStorage; }

    /**
     * Function for get item from item's guid
     * @param Guid Item's guid
     * @return Returns item with that guid, if it exists. Returns nullptr, if it isn't exists
     */
    FItemSlot* GetItemSlotByGuid(const FGuid& Guid);

    /**
     * Called when item has been added to storage
     */
    UPROPERTY(BlueprintAssignable)
    FOnAddSlotSignature OnAddItemSlotSignature;

    /**
     * Called when item has been changed 
     */
    UPROPERTY(BlueprintAssignable)
    FOnChangeSlotSignature OnChangeItemSignature;
    
    /**
     * Called when item has been removed from storage
     */
    UPROPERTY(BlueprintAssignable)
    FOnRemoveSlotSignature OnRemoveSlotSignature;
};
