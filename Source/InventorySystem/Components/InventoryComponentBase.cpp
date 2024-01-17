#include "InventoryComponentBase.h"
#include "DataTypes/ItemData.h"
#include "Algo/Accumulate.h"


UInventoryComponentBase::UInventoryComponentBase()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponentBase::BeginPlay()
{
    Super::BeginPlay();
    if (InventorySize == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventorySize must be initialized"));
        UActorComponent::Deactivate();
    }
}

bool UInventoryComponentBase::ValidateInputs(const UItemData* ItemData, const int32& AmountOf)
{
    return IsValid(ItemData) && AmountOf > 0;
}

TArray<FGuid> UInventoryComponentBase::AddItems(const UItemData* ItemData, int32 AmountOf)
{
    if (!ValidateInputs(ItemData, AmountOf))
    {
        return {};
    }
    if (CalculateSpaceForItem(ItemData) < AmountOf)
    {
        return {};    
    }
    TArray<FGuid> AddedItemGuids = AddItemToExistingSlot(ItemData, AmountOf);
    for (FGuid& ItemGuid: AddItemToAvailableSlot(ItemData, AmountOf))
    {
        AddedItemGuids.Add(ItemGuid);
    }
    return AddedItemGuids;
}

bool UInventoryComponentBase::RemoveItems(const UItemData* ItemData, int32 AmountOf)
{
    if (!ValidateInputs(ItemData, AmountOf))
    {
        return false;
    }
    if (CalculateAmountOfItem(ItemData) < AmountOf)
    {
        return false;
    }
    const int32 StackMaxSize = ItemData->GetStackMaxSize();
    for (int32 IndexPos = 0; IndexPos < InventoryStorage.Num(); ++IndexPos)
    {
        if (InventoryStorage[IndexPos].GetItemData() != ItemData)
        {
            continue;
        }
        if (InventoryStorage[IndexPos].GetAmountOf() == StackMaxSize)
        {
            continue;
        }
        if (InventoryStorage[IndexPos].GetAmountOf() < AmountOf)
        {
            const int32 AmountToRemoveInSlot = InventoryStorage[IndexPos].GetAmountOf();
            AmountOf -= AmountToRemoveInSlot;
            OnRemoveSlotSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            InventoryStorage.RemoveAt(IndexPos);
        }
        else if (InventoryStorage[IndexPos].GetAmountOf() == AmountOf)
        {
            const int32 AmountToRemoveInSlot = AmountOf;
            AmountOf -= AmountToRemoveInSlot;
            OnRemoveSlotSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            InventoryStorage.RemoveAt(IndexPos);
            return true;
        }
        else
        {
            const int32 AmountToRemoveInSlot = AmountOf;
            AmountOf -= AmountToRemoveInSlot;
            InventoryStorage[IndexPos].SetAmountOf(InventoryStorage[IndexPos].GetAmountOf() - AmountToRemoveInSlot);
            OnChangeItemSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            return true;
        }
    }
    for (int32 IndexPos = 0; IndexPos < InventoryStorage.Num(); ++IndexPos)
    {
        if (InventoryStorage[IndexPos].GetItemData() != ItemData)
        {
            continue;
        }
        if (InventoryStorage[IndexPos].GetAmountOf() < AmountOf)
        {
            const int32 AmountToRemoveInSlot = InventoryStorage[IndexPos].GetAmountOf();
            AmountOf -= AmountToRemoveInSlot;
            OnRemoveSlotSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            InventoryStorage.RemoveAt(IndexPos);
        }
        else if (InventoryStorage[IndexPos].GetAmountOf() == AmountOf)
        {
            const int32 AmountToRemoveInSlot = AmountOf;
            AmountOf -= AmountToRemoveInSlot;
            OnRemoveSlotSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            InventoryStorage.RemoveAt(IndexPos);
            return true;
        }
        else
        {
            const int32 AmountToRemoveInSlot = AmountOf;
            AmountOf -= AmountToRemoveInSlot;
            InventoryStorage[IndexPos].SetAmountOf(InventoryStorage[IndexPos].GetAmountOf() - AmountToRemoveInSlot);
            OnChangeItemSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            return true;
        }
    }
    return false;
}

TArray<FGuid> UInventoryComponentBase::AddItemToExistingSlot(const UItemData* ItemData, int32& AmountOf)
{
    if (!ValidateInputs(ItemData, AmountOf))
    {
        return {};
    }
    TArray<FGuid> AddedItemsGuid;
    const int32 StackMaxSize = ItemData->GetStackMaxSize();
    for (int32 IndexPos = 0; IndexPos < InventoryStorage.Num(); ++IndexPos)
    {
        if (InventoryStorage[IndexPos].GetItemData() != ItemData)
        {
            continue;
        }
        if (AmountOf > StackMaxSize - InventoryStorage[IndexPos].GetAmountOf())
        {
            const int32 AddedAmountOf = StackMaxSize - InventoryStorage[IndexPos].GetAmountOf(); 
            AmountOf -= AddedAmountOf;
            InventoryStorage[IndexPos].SetAmountOf(StackMaxSize);
            OnChangeItemSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            AddedItemsGuid.Add(InventoryStorage[IndexPos].GetItemGuid());
        }
        else
        {
            const int32 AddedAmountOf = AmountOf;
            AmountOf -= AddedAmountOf;
            InventoryStorage[IndexPos].SetAmountOf(InventoryStorage[IndexPos].GetAmountOf() + AddedAmountOf) ;
            OnChangeItemSignature.Broadcast(InventoryStorage[IndexPos].GetItemGuid());
            AddedItemsGuid.Add(InventoryStorage[IndexPos].GetItemGuid());
            return AddedItemsGuid;
        }
    }
    return AddedItemsGuid;
}

TArray<FGuid> UInventoryComponentBase::AddItemToAvailableSlot(const UItemData* ItemData, int32& AmountOf)
{
    if (!ValidateInputs(ItemData, AmountOf))
    {
        return {};
    }
    TArray<FGuid> AddedItemsGuid;
    const int32 StackMaxSize = ItemData->GetStackMaxSize(); 
    while (AmountOf != 0)
    {
        if (InventoryStorage.Num() == InventorySize)
        {
            return AddedItemsGuid;
        }
        FItemSlot ItemSlot = CreateItemSlot(ItemData, AmountOf);
        InventoryStorage.Add(ItemSlot);
        OnAddItemSlotSignature.Broadcast(ItemSlot.GetItemGuid());
        AddedItemsGuid.Add(ItemSlot.GetItemGuid());
        if (AmountOf < StackMaxSize)
        {
            AmountOf = 0;
            return AddedItemsGuid;
        }
        else
        {
            AmountOf -= StackMaxSize;
        }
    }
    return AddedItemsGuid;
}

FItemSlot UInventoryComponentBase::CreateItemSlot(const UItemData* ItemData, int32 AmountOf) 
{
    if (!ValidateInputs(ItemData, AmountOf))
    {
        return {}; 
    }
    const int32 StackMaxSize = ItemData->GetStackMaxSize();
    if (StackMaxSize >= AmountOf)
    {
        return FItemSlot(this, FGuid::NewGuid(), ItemData, AmountOf);
    }
    else
    {
        return FItemSlot(this, FGuid::NewGuid(), ItemData, StackMaxSize);
    }
}

int32 UInventoryComponentBase::CalculateAmountOfItem(const UItemData* ItemData) const
{
    if (!IsValid(ItemData))
    {
        return -1;
    }
    return Algo::Accumulate(InventoryStorage, 0, [ItemData](const int32 Accum, const FItemSlot& Data)
        { return Data.GetItemData() == ItemData ? Accum + Data.GetAmountOf() : Accum; });
}

int32 UInventoryComponentBase::CalculateSpaceForItem(const UItemData* ItemData) const
{
    if (!IsValid(ItemData))
    {
        return -1;
    }
    const int32 AvailableSpaceInInventory = ItemData->GetStackMaxSize() * (InventorySize - InventoryStorage.Num());
    return AvailableSpaceInInventory + Algo::Accumulate(InventoryStorage, 0,
        [ItemData](const int32 Accum, const FItemSlot& Data)
        { return Data.GetItemData() == ItemData ? Accum + (ItemData->GetStackMaxSize() - Data.GetAmountOf()) : Accum; });
}

FItemSlot UInventoryComponentBase::GetItemSlot(const FGuid& Guid)
{
    return *GetItemSlotByGuid(Guid);
}

FItemSlot* UInventoryComponentBase::GetItemSlotByGuid(const FGuid& Guid)
{
    if (!Guid.IsValid())
    {
        return nullptr;
    }
    for (FItemSlot& Item: InventoryStorage)
    {
        if (Item.GetItemGuid() == Guid)
        {
            return &Item;
        }
    }
    return nullptr;
}