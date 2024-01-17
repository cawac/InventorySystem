#include "ItemWidget.h"
#include "DataTypes/ItemSlot.h"
#include "Components/InventoryComponentBase.h"
#include "DataTypes/ItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UWItemWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    if (!ValidateItemInfo(InventoryComp, ItemGuid))
    {
        return;
    }
    const FItemSlot* ItemSlot = InventoryComp->GetItemSlotByGuid(ItemGuid);
    if (!ItemSlot->IsValid())
    {
        return;
    }
    if (ItemImage == nullptr)
    {
        ItemImage->SetBrushFromTexture(ItemSlot->GetItemData()->GetThumbnail());
    }
    
}

void UWItemWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UWItemWidget::ValidateItemInfo(const UInventoryComponentBase* InventoryComponent, const FGuid& Guid)
{
    if (IsValid(InventoryComponent) && Guid.IsValid())
    {
        return true;
    }
    return false;
}

void UWItemWidget::Init(UInventoryComponentBase* InventoryComponent, const FGuid& Guid)
{
    if (!ValidateItemInfo(InventoryComponent, Guid))
    {
        return;
    }
    InventoryComp = InventoryComponent;
    ItemGuid = Guid;
    SynchronizeProperties();
}

void UWItemWidget::RefreshItemSlot()
{
    SynchronizeProperties();
}