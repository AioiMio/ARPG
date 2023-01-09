# ARPG

## 制作意图
本人非常喜欢血缘和黑暗之魂系列，自己也一直想尝试着做一款类魂的ARPG游戏。另外，随着UE5的发布，我对于UE5的画面表现和一些新功能非常感兴趣，于是就学习了UE5以及Gameplay Ability System的使用，并且以练习虚幻引擎的使用以及求职用作品展示为目的，试着模仿血缘的战斗系统制作了这个Demo项目。

## 开发时长
约2个月

## 开发环境
+ 操作系统：Windows 11
+ CPU：Intel(R) Core(TM) i9-9900K
+ 显卡：Nvidia Geforce RTX 3070
+ 内存：64GB
+ IDE：JetBrains Rider

## 项目说明
+ 本项目采用了C++代码和蓝图相结合的形式制作。角色技能，伤害计算，受击反馈等等的战斗系统主要由C++代码实现，关卡地图和机关的设计主要由编辑器以及蓝图实现
+ 利用虚幻引擎的Gameplay Ability System，实现了角色的技能系统以及伤害计算
+ 利用行为树完成了怪物以及Boss的AI设计
+ 实现了血缘中的枪反以及内脏攻击系统
+ 学习了虚幻引擎的网络同步功能，利用插件暂时实现了通过Steam的网络联机功能（Bug较多）
+ YouTube演示：
    + 单机关卡：https://youtu.be/CO9MMSE4Y_Q
    + 联机Boss战：https://youtu.be/nYpOBNHqQTE

## 按键操作
本项目由于开发时间比较紧，并且专注于战斗系统的开发，有很多功能还未实装，开始界面目前仅支持鼠标操作，游玩部分目前仅支持Xbox手柄操作，使用联机功能需要事先登入Steam账户，关闭游戏需要Alt+F4

| 按键 | 说明 |
| ---- | ---- |
| A | 跳跃 |
| B | 翻滚/奔跑 |
| Y | 互动 |
| RB | 轻攻击 |
| RT | 重攻击/蓄力攻击 |
| LT | 枪械攻击 |
| 左摇杆 | 移动 |
| 左摇杆按下 | 切换锁定 |
| 右摇杆 | 摄像机 |
| 右摇杆按下 | 锁定/解除锁定 |

