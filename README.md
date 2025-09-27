# 🎮 Multiplayer FPS Game (Unreal Engine 5.6, C++)

一个基于 **Unreal Engine 5.6** 开发的多人第一人称射击 (FPS) 游戏项目，使用 **C++ + 蓝图** 完成核心玩法逻辑，重点实现了网络同步、射击系统、角色动画与战斗特效。

---

## ✨ 项目亮点

- **高精度网络同步**
  - 结合 **RPC** 与 **属性复制 (RepNotify)**，同步玩家位置、武器、动画和特效，保证多客户端间一致性。
  - 实现 **客户端预测 + 服务器权威回放 (Server-Side Rewind)**，显著减少网络延迟对命中判定的影响。

- **模块化武器系统**
  - 支持多种武器类型（命中扫描、投射物、霰弹枪等）。
  - 使用组件化设计（`UCombatComponent` / `AWeapon`），解耦输入、动画与逻辑，便于扩展新武器。

- **流畅的角色控制与动画**
  - 使用 **AnimMontage** 与 **AnimInstance** 驱动角色开火、换弹、瞄准动画。
  - 支持骨骼 Socket 对齐、IK 调整，保证手部与武器精准贴合。

- **战斗表现增强**
  - 使用 **Niagara 粒子特效** 实现枪口火焰、弹壳抛出、命中反馈等视觉效果。
  - 动画通知 (AnimNotify) 精准触发音效与粒子特效。

- **性能优化**
  - 通过 `FVector_NetQuantize`、角度压缩等手段优化网络带宽。
  - 使用条件复制 (Replication Conditions) 避免冗余同步，提高多人场景下的运行效率。

---

## 🛠 技术栈

- **引擎**: Unreal Engine 5.6  
- **编程语言**: C++ / Blueprints  
- **核心系统**:  
  - Gameplay Framework（Actor / Pawn / Character / Component）  
  - Networking（RPC / Replication / Server-Side Rewind / Prediction）  
  - Animation（AnimInstance / AnimMontage / IK / Sockets）  
  - VFX（Niagara / AnimNotify）  

---

## 🚀 运行方式

1. 克隆本项目
(https://github.com/cjzxq/Multiplayer-FPS-game)
.sln文件ctrl+F5启动UE即可
