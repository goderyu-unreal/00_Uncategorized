# 00_Uncategorized
Uncategorized test demo either Blueprint or C plus plus.

> 所有出现的`LS`字段均代表`Level Sequence`

# Sequencer编辑器

## 混合动画

在LS中，为一个骨骼模型添加多段动画时，混合方式有两种：
1. 将多段动画在Sequencer编辑器中置于同一行，并且前后两者有重叠部分，重叠的部分可以右键选择混合选项，使用什么样的曲线方式来混合
2. 将多段动画在Sequencer编辑器中置于不同行，分别在同一时刻（帧）二者都执行的交界处为各自的动画的**权重(Weight)**属性打关键帧，后设置属性值即可混合。Tips：往往在时间轴前的动画权重从1开始，前者动画结束处设为0；位于后者的动画权重从0开始，前者动画结束处设为1。

