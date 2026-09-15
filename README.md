# 多人对战 FPS 原型（FPS00）

> Unreal Engine 5.8 · C++ · 38 个源文件 / 3449 行（`.h` + `.cpp`，不含生成的 `Intermediate/`）
> 核心目标：把**服务器权威的战斗结算**与**完全数据驱动的武器系统**做扎实。
> 命名对应：作品集中的展示名为「多人对战 FPS 原型」，代码 / 仓库 / 模块代号为 `FPS00`（模块名 `FPS`）。

个人项目，独立完成架构设计、玩法实现、网络同步与 UI。

---

## 技术要点

| 方向 | 实现 |
|---|---|
| 网络模型 | Server Authoritative + 客户端预测，**13 个 RPC**（`Server_*` / `Client_*` / `NetMulticast`） |
| 弹药一致性 | 客户端开火立即本地扣弹并编号，服务器权威扣减后经 `Multicast_FireWeapon` 下发权威弹药数，客户端用 `Rep_Fire` 按「未确认开火数」纠偏：$A_{\text{display}} = A_{\text{auth}} - S_{\text{pending}}$ |
| 武器系统 | 附着点、1P/3P 蒙太奇、动画集、爆头倍率全部按 `GameplayTag` 存入 `UWeaponData`，**新增武器无需修改任何 C++ 代码** |
| 伤害管线 | 服务器单点结算，修正项集中在 `UCombatComponent::CalculateDamageForHit`；爆头倍率来自 `UWeaponData::GetHeadShotDamageMultiplier(WeaponType)`，未配置时回退默认值 |
| 架构 | 组件化 + 接口解耦：角色只做输入与桥接，能力拆到 `UCombatComponent` / `UHealthComponent` / `UEliminationComponent`，跨模块通过 `IPlayerInterface` 通信 |
| 复制策略 | 按可见性分别使用 `COND_OwnerOnly`（血量、备弹）与 `COND_SkipOwner`（瞄准状态） |
| 击杀播报 | 位掩码枚举 `ESpecialElimType : uint16`（`Headshot = 1<<0` … `LostTheLead = 1<<9`，共 **10 种**，配合 `ENUM_CLASS_FLAGS`），玩家状态侧逐位解码后驱动客户端播报队列 |
| 动画契约 | 蒙太奇动画通知在动画中段触发 → 动画蓝图调用 `Notify_CycleWeapon` / `Notify_ReloadWeapon`（声明在 `IPlayerInterface` 上）→ 角色转发给组件提交状态 |
| 输入 | Enhanced Input，自定义武器射线通道 `ECC_GameTraceChannel1`（`Weapon`），与角色胶囊通道分离 |

---

## 目录结构

```
Source/FPS/
├── Public/
│   ├── Combat/        CombatComponent.h
│   ├── Data/          WeaponData.h、SpecialElimData.h
│   ├── Elimination/   EliminationComponent.h
│   ├── Game/          ShooterGameModeBase.h、ShooterGameStateBase.h
│   ├── Health/        HealthComponent.h
│   ├── Interfaces/    PlayerInterface.h
│   ├── Player/        ShooterPlayerController.h、ShooterPlayerState.h
│   ├── ShooterTypes/  ShooterTypes.h（含 ESpecialElimType）
│   ├── Tags/          ShooterGameplayTags.h
│   ├── UI/            ShooterReticle.h、ReserveAmmo.h、Elims/…
│   └── Weapon/        Weapon.h
└── Private/           与上表对应的实现
Docs/FPS00-设计文档.md  架构图 / 时序图 / 状态机（Mermaid）
Config/                 DefaultEngine.ini 等（含自定义碰撞通道定义）
FPS.uproject            引擎关联 5.8，单 Runtime 模块 FPS
```

依赖模块：`Core` / `CoreUObject` / `Engine` / `InputCore` / `GameplayTags` / `PhysicsCore` / `UMG` / `Slate` / `SlateCore`，私有依赖 `EnhancedInput`。

---

## 构建

1. 安装 **Unreal Engine 5.8**；
2. 右键 `FPS.uproject` → *Generate Visual Studio project files*；
3. 打开生成的 `FPS.sln`，构建 `Development Editor` 配置，或直接用引擎打开 `FPS.uproject`；
4. 联机验证：Play → Number of Players = 2（或在编辑器偏好中启用 *Run Under One Process* 以快速验证）。

---

## 仓库范围

本仓库**仅包含代码**（`Source/`、`Config/`、`Docs/`、`*.uproject`）。`Content/` 资产约 1.5 GB，其中大部分为第三方素材包，未纳入版本管理，因此**克隆后无法直接运行**。

- 代码阅读入口建议：`Source/FPS/Private/Combat/CombatComponent.cpp`（弹药纠偏与伤害结算）、`Source/FPS/Public/Data/WeaponData.h`（数据驱动配置）、`Source/FPS/Public/ShooterTypes/ShooterTypes.h`（击杀位掩码）。
- 演示视频与截图见作品集：<https://dcnzeir0z0se.feishu.cn/docx/AanxdVrWUoTlVzxRowAcQBi3nyh>
