# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

A collection of scripts for audio workflow enhancement across Unity (FMOD integration) and Reaper.

## Structure

- **Unity/FMOD/**: C# scripts for FMOD audio control in Unity
  - `FMODButtonEventControl.cs` - Trigger FMOD events on UI button hover/click
  - `FMODSliderParameterControl.cs` - Control FMOD parameters via UI sliders
  - `FMODToggleParameterControl.cs` - Control binary FMOD parameters via toggles
  - `FMODAnimationEventTriggers.cs` - Trigger FMOD events from animation timeline

- **Unity/FMOD/Editor/**: Editor-only tools (excluded from builds)
  - `FMODSceneManager.cs` - Dockable window showing all FMOD references across loaded scenes
  - `FMODSceneScanner.cs` - Reflection-based scanner for detecting FMOD fields
  - `FMODFieldInfo.cs` - Data model for scan results

- **Reaper/**: Lua scripts for Reaper DAW
  - `DRD_Open-Original-Reaper-Project-From-Path-In-BWF-Metadata.lua` - Opens source project from BWF metadata

## Unity/FMOD Scripts

**Requirements**: Unity 2022.3.10f1+, FMOD 2.02.17+

**Key dependencies**:
```csharp
using FMODUnity;  // RuntimeManager, EventReference
using UnityEngine.UI;  // Button, Slider, Toggle
using UnityEngine.EventSystems;  // EventTrigger
```

**Pattern**: All FMOD scripts use serializable structs with arrays for Inspector configuration, linking UI elements to FMOD events/parameters at runtime.

**Important**: FMOD parameter names must exactly match those defined in the FMOD project. Slider/toggle value ranges should match FMOD parameter ranges (no value scaling is implemented).

## Reaper Scripts

**Pattern**: Scripts use `reaper.` API calls. The BWF metadata script expects `BWF:Description` to start with `RPP:` followed by a project path.
