#include "InventoryWidget.h"
#include "Components/InventoryComponentBase.h"
#include "ItemWidget.h"

void UWInventoryWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
}

void UWInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UWInventoryWidget::Init(UInventoryComponentBase* InventoryComponent)   
{
    if (!IsValid(InventoryComponent))
    {
        return;
    }
    InventoryComp = InventoryComponent;
    InventoryComp->OnAddItemSlotSignature.AddDynamic(this, &UWInventoryWidget::OnAddItemSlot);
    InventoryComp->OnChangeItemSignature.AddDynamic(this, &UWInventoryWidget::OnChangeItemSlot);
    InventoryComp->OnRemoveSlotSignature.AddDynamic(this, &UWInventoryWidget::OnRemoveSlot);
    for (auto& ItemSlot: InventoryComp->GetInventoryStorage())
    {
        UWItemWidget* ItemWidget = CreateWidget<UWItemWidget>(GetWorld(), SlotWidgetClass);
        ItemWidget->Init(InventoryComp, ItemSlot.GetItemGuid());
        ItemWidgets.Add(ItemSlot.GetItemGuid(), ItemWidget);
    }
    SynchronizeProperties();
}

void UWInventoryWidget::OnRemoveSlot(const FGuid& ItemGuid)
{
    if (!ItemGuid.IsValid())
    {
        return;
    }
    if (!ItemWidgets.Contains(ItemGuid))
    {
        return;
    }
    ItemWidgets[ItemGuid]->RemoveFromParent();
    ItemWidgets.Remove(ItemGuid);
}

void UWInventoryWidget::OnAddItemSlot(const FGuid& ItemGuid)
{
    UWItemWidget* SlotWidget = CreateWidget<UWItemWidget>(GetWorld(), SlotWidgetClass);
    SlotWidget->Init(InventoryComp, ItemGuid);
    ItemWidgets.Add(ItemGuid, SlotWidget);
}

void UWInventoryWidget::OnChangeItemSlot(const FGuid& ItemGuid)
{
    if (!ItemGuid.IsValid())
    {
        return;
    }
    if (!ItemWidgets.Contains(ItemGuid))
    {
        return;
    }
    ItemWidgets[ItemGuid]->RefreshItemSlot();
}