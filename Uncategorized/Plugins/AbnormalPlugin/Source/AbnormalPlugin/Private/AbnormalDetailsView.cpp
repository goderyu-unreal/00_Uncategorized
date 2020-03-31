#include "AbnormalDetailsView.h"
#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"

TSharedRef<IPropertyTypeCustomization> FAbnormalDetailsView::MakeInstance()
{
    return MakeShareable(new FAbnormalDetailsView());
}

void FAbnormalDetailsView::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder &StructBuilder, IPropertyTypeCustomizationUtils &StructCustomizationUtils)
{
    uint32 NumChildren;
    StructPropertyHandle->GetNumChildren(NumChildren);
    FString Name = StructPropertyHandle->GetPropertyDisplayName().ToString();
    IDetailGroup &Group = StructBuilder.AddGroup(FName(*Name), FText::FromString(Name));

    for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
    {
        TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

        IDetailPropertyRow &Property = Group.AddPropertyRow(ChildHandle);
        if (ChildHandle->GetProperty()->GetName() == "AdditiveTransform"
        || ChildHandle->GetProperty()->GetName() == "PlayRate"
        || ChildHandle->GetProperty()->GetName() == "DelaySeconds")
        {
            continue;
        }

        if (ChildHandle->GetProperty()->GetFName() == "TargetTransformSource")
        {
            TargetTransformSourceProperty = ChildHandle;
            continue;
        }

        if (ChildHandle->GetProperty()->GetFName() == "AbnormalActorType")
        {
            AbnormalActorTypeProperty = ChildHandle;
            continue;
        }


        static const FName AbnormalActorTypeName = "AbnormalActorType";
        // 获取该属性在代码中设置的AbnormalActorType的枚举值
        int32 AbnormalActorType = ChildHandle->GetIntMetaData(AbnormalActorTypeName);
        static const FName TargetTransformName = "TargetTransformSource";
        // 获取该属性在代码中设置的TargetTransformSource的枚举值
        int32 TargetTransform = ChildHandle->GetIntMetaData(TargetTransformName);
        int32 Flag = AbnormalActorType | TargetTransform;
        // 单个属性的可见性只能绑定一个函数，如果绑定多个，会以最后绑定的那个为主
        Property.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAbnormalDetailsView::ArePropertiesVisible, Flag)));
    }
}

EVisibility FAbnormalDetailsView::ArePropertiesVisible(const int32 VisibleType) const
{
    // 获取该属性在代码中设置的AbnormalActorType的枚举值
    uint8 AbnormalActorTypeValue = 0;
    AbnormalActorTypeProperty->GetValue(OUT AbnormalActorTypeValue);
    // 获取该属性在代码中设置的TargetTransformSource的枚举值
    uint8 TargetTransformSourceValue = 0;
    TargetTransformSourceProperty->GetValue(OUT TargetTransformSourceValue);

    // 采用位运算，提取该属性设置的TargetTransformSource的枚举值
    // 如果为0，说明没有设置该枚举值，那么该值不能控制属性的可见性
    if ((VisibleType & ~AbnormalActorTypeValue) == 0)
    {
        // 那么就只参照AbnormalActorType的枚举值来控制属性的可见性
        // VisibleType & ~TargetTransformSourceValue会过滤掉
        // 当前属性中设置的任何一种TargetTransformSource枚举值
        // 这一步会将其使用位运算消除掉，只留下AbnormalActorType
        // 枚举值和设定好的枚举值进行比较
        return ((VisibleType & ~TargetTransformSourceValue) == AbnormalActorTypeValue) ? EVisibility::Visible : EVisibility::Collapsed;
    }
    else
    {
        return ((AbnormalActorTypeValue | TargetTransformSourceValue) == VisibleType) ? EVisibility::Visible : EVisibility::Collapsed;
    }
    
}

