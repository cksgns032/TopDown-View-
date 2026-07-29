# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

This is an Unreal Engine 5.7 project (`TopViewGame`, project name "Top Down Game Template") built on Epic's stock **Top Down** C++ template. The active game is a top-down action RPG with inventory, equipment, and melee combo combat, built almost entirely as Blueprint content layered on top of the small C++ template base.

- Active level: `Content/TopDown/Lvl_TopDown.umap` (`GameDefaultMap` / `EditorStartupMap` in `Config/DefaultEngine.ini`)
- Active game mode: `Content/TopDown/Blueprints/BP_TopDownGameMode` (`GlobalDefaultGameMode`)
- `Content/Variant_Strategy/` and `Content/Variant_TwinStick/` (and their matching C++ under `Source/TopViewGame/Variant_Strategy`, `Source/TopViewGame/Variant_TwinStick`) are Epic's unused sample template variants shipped alongside the template — they are not part of the active game and generally don't need touching unless the user says otherwise.

## Critical constraint: most gameplay logic is not text-editable

Almost all real gameplay logic (character behavior, item/inventory system, combat combos, AI behavior trees, UI) lives in Blueprint assets (`.uasset`), which are **binary files** — they cannot be opened, read, or edited as text. Only the following are directly editable by Claude Code:

- C++ under `Source/TopViewGame/` (thin base classes only — see below)
- Text config: `Config/*.ini`, `TopViewGame.uproject`, `*.Build.cs`, `*.Target.cs`

When a task requires changing Blueprint graph logic, DataTable rows, widget layouts, or animation blueprints, say so explicitly and describe the change for the user to make in the Unreal Editor — do not guess at or fabricate Blueprint contents. `.uasset`/`.umap` diffs in `git status`/`git diff` are opaque binary changes; don't try to interpret them from the diff text.

## Build / run

There is no CLI build or test suite for this project — it's developed and run through the Unreal Editor.

- Open `TopViewGame.uproject` in Unreal Engine 5.7 to run the editor.
- The C++ module (`Source/TopViewGame`) compiles via UnrealBuildTool, normally triggered by the editor ("Compile" button) or by building `TopViewGame.sln` in Visual Studio (regenerate with "Generate Visual Studio project files" from the `.uproject` right-click menu if `Intermediate/ProjectFiles` is stale or missing).
- There are no automated tests in this repo.

## Repository layout notes

- `Content/1_BluePrint/` is where nearly all custom gameplay logic lives, organized by asset kind:
  - `Actor/` — player character (`BP_TopDownCharacter`), items and weapons (`BP_ItemBase`, `BP_OneHandSword`, `BP_TwoHandSword`)
  - `Monster/`, `AI/` — enemy actors and behavior-tree tasks/services (paired with `Content/4_AI/BB_Monster` / `BT_Monster`)
  - `Controller/` — player controller (`BP_TopDownController`) and monster AI controller
  - `AnimNotify/` — combo/attack anim notifies (`AN_Attack`, `AN_AddCombo`, `AN_ResetCombo`)
  - `DataTable/` — data-driven tables for items (`DT_Item`), equip positions (`DT_Equipe_Pos`), monster stats (`DT_Monster_Stat`), and combo chains (`CHT_*Combo`)
  - `Struct/`, `Enum/` — supporting types for inventory (`BPStruct_InventoryItme`, `BPStruct_ItemData`), equip slots, monster stats, player data, and category/equip-type/mouse-point enums
  - `Motion/` — Motion Matching / Pose Search database assets driving character locomotion
- `Content/2_Animation` … `Content/8_Effect` are asset-type folders (animation, input actions, AI blackboard/tree, UMG widgets for HUD/inventory, materials, meshes, VFX) referenced by the Blueprints above.
- `Content/__ExternalActors__/` and `Content/__ExternalObjects__/` are World Partition per-actor/object data auto-generated whenever the level is saved in-editor — frequent adds/deletes there under `Content/TopDown/...` are normal editor churn, not manual edits.
- `Source/TopViewGame/TopViewGameCharacter.h/.cpp`, `TopViewGamePlayerController.h/.cpp`, `TopViewGameGameMode.h/.cpp` are Epic's near-stock template base classes (top-down camera, point-and-click navigation via Enhanced Input) that the `BP_TopDown*` Blueprints subclass and extend.
- `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/` are generated/local build and editor artifacts — never treat them as source of truth and avoid editing inside them.
