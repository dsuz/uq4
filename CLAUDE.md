# CLAUDE.md

このファイルは、このリポジトリで作業する Claude Code 向けのガイドです。

## プロジェクト概要

- **uq4** — 「アンリアルクエスト4 GAME編」（株式会社ヒストリア）のサンプルプロジェクトをベースにした Unreal Engine プロジェクト。参考: https://historia.co.jp/archives/31826/
- 元サンプル（UE 5.0.3 / Blueprint 中心）を **UE 5.7** に移行し、主要ロジックを **C++ に書き直している**。
- ゲーム内容: 三人称視点のアクションシューティング。右クリックでエイム、左クリックで水鉄砲の弾を発射。スタートゲートを通過するとタイマーが始まり、ゴールゲートで終了するタイムアタック形式。的・ドア・ジャンプパッド・トゲ（即死）などのギミックがある。
- `ReadMe_UQ4.txt` は元サンプル（UE5.0.3 / Blueprint 版）の説明であり、現状の構成とは一致しない部分が多い（`BP_UQ4Player` → C++ の `AUQ4Player` など）。

## 開発環境・ビルド

- エンジン: Unreal Engine 5.7（`uq4.uproject` の `EngineAssociation`）
- 主な開発環境は Windows + Visual Studio 2022（`.vsconfig` 参照）。`.sln` は生成物なので git 管理外。uproject を右クリック →「Generate Visual Studio project files」で生成する。
- ビルドターゲット: `uq4`（Game）、`uq4Editor`（Editor）。C++ モジュールは `uq4` 1つのみ。
- コマンドラインでビルドする場合の例（Windows）:
  ```
  "<UE_5.7>\Engine\Build\BatchFiles\Build.bat" uq4Editor Win64 Development -Project="<path>\uq4.uproject" -WaitMutex
  ```
- **クラウド（Linux）環境には Unreal Engine が入っていないため、ここではビルド・実行・テストはできない。** C++ を変更した場合は、コンパイル確認をユーザーに依頼すること。自動テストは存在しない。

## サブモジュール

- `Content/uq4-licenced-assets` → `https://github.com/dsuz/uq4-licenced-assets.git`（ライセンス付きアセット。Dissolve マテリアル等）
- クローン後は `git submodule update --init` が必要（アクセス権がない場合は空のままになる）。

## ディレクトリ構成

```
uq4.uproject            プロジェクトファイル（プラグイン: GameplayStateTree, MetaHumanCharacter, ModelingToolsEditorMode, AIAssistant）
Config/                 Default*.ini（GameplayTags は DefaultGameplayTags.ini で定義）
Source/uq4/             C++ ソース（ゲームモジュール）
Content/                アセット (.uasset/.umap はバイナリ。直接 git 管理、LFS 未使用)
  BP_*.uasset           C++ クラスを親にした Blueprint（BP_Player, BP_GameMode, BP_Gate, BP_Door, BP_Target, BP_JumpPad, BP_Spike, BP_Bullet など）
  ST_Game / ST_Player   StateTree アセット（ゲーム進行 / プレイヤー状態）
  STT_*                 StateTree タスク（GameStart, GameCompleted, PlayerAim）
  WBP_*                 UMG ウィジェット（Start, Timer）
  UnrealQuest4_Game/    元サンプル由来のアセット（Maps/ThirdPersonMap がデフォルトマップ）
  uq4-licenced-assets/  サブモジュール
```

## C++ アーキテクチャ（Source/uq4）

| クラス | 役割 |
|---|---|
| `AUQ4Player` (`UQ4Player.*`) | プレイヤー。Enhanced Input、`UGameplayCameraComponent`（GameplayCameras）、エイム/フリーラン切替（`EPlayerState`）、射撃（アニメ通知から `ShootProjectile`）、スライディング、`TakeDamage`→`Die`（ラグドール化） |
| `AThirdPersonGameMode` | `UStateTreeComponent` を持ち、ゲーム進行を StateTree で管理。タイマー計測、`SetStateTag` で StateTree にイベント送信、`OnGameStart`/`OnGameFinished` デリゲート |
| `AGate` | スタート/ゴールゲート。オーバーラップ時に `TagSendToStateOnOverlap` を GameMode 経由で StateTree に送る |
| `ATarget` | 的。往復移動し、弾が当たると破壊されエフェクト/SE、`TargetDoor` を開く |
| `ADoor` | Timeline で開閉するドア |
| `AJumpPad` | 踏むとプレイヤーを跳ね上げるパッド（Timeline アニメーション） |
| `ASpike` | 触れたプレイヤーに致死ダメージ |
| `ABullet` | ProjectileMovement による弾 |
| `UBBoyDaisukeUtilities` | Blueprint 関数ライブラリ（`FloatToTime`: MM:SS.sss 形式） |
| `Enums.h` | `EPlayerState`（FreeRun/Aim）、`EGateType` |

- 状態管理は **StateTree + GameplayTags** に移行中（Issue #10）。タグ: `GameState.InGame`, `GameState.Completed`, `Player.Aim`, `Player.FreeRun`。新しい状態を追加する場合は `Config/DefaultGameplayTags.ini` にタグを追加する。
- 依存モジュール（`uq4.Build.cs`）: Core, CoreUObject, Engine, InputCore, UMG, EnhancedInput, GameplayCameras, GameplayTags, GameplayStateTreeModule。新しいエンジンモジュールを使う場合はここに追加する。
- 一部のアセットは C++ コンストラクタで `ConstructorHelpers::FObjectFinder` によりパス指定でロードしている（例: `/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin`、`/Game/UnrealQuest4_Game/Meshes/SM_WaterGun`）。アセットを移動・リネームする際はこれらのパスも更新すること。

## 作業上の注意

- `.uasset` / `.umap` はバイナリなので、Claude からは内容の読み書き・差分確認ができない。Blueprint / StateTree / Widget の変更が必要な場合は、エディタでの手順をユーザーに説明する。
- `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `*_BuiltData.uasset` は生成物（git 管理外）。
- コーディングスタイル: Unreal の命名規則（`A`/`U`/`F`/`E` プレフィックス、bool は `b` プレフィックス、`TObjectPtr` を使用）、タブインデント。コメントは日本語。
- コミットメッセージは日本語で、関連 Issue 番号を末尾に付ける慣習（例: `... に変えた #10`）。
