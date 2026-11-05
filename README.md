# VYX AI for Geometry Dash

Android64 Geode mod targeting Geometry Dash **2.2081** and Geode **5.11.2**.

## Version 0.4.0

- Interactive VYX AI control popup opened from the main menu.
- Toggle the live physics-frame counter.
- Toggle the P1/P2 jump-input tracker.
- Toggle the recent four-input history independently.
- HUD updates live as the settings change.

The input log records when the jump input reaches the game's input hook. The displayed gap is the spacing between that player's recorded jump presses. It is **not** a calculated success-tolerance window. Accurate click-slack analysis, macro import/export, and replay simulation are not implemented yet.

## Install

1. Download the `VYX-AI-Android64` artifact from the successful GitHub Actions run.
2. Install the `.geode` package using Geode's **Install from file** option.
3. Restart Geometry Dash.

The package is for Android 64-bit and is not a standalone APK.

## Build

The GitHub Actions workflow uses the Geode 5.11.2 SDK and builds the Android64 target for Geometry Dash 2.2081.
