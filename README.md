# VYX AI for Geometry Dash

Android64 Geode mod targeting Geometry Dash **2.2081** and Geode **5.11.2**.

## Implemented in 0.3.0

- Live frame counter driven by the PlayerObject update loop rather than the render/update hook.
- P1 and P2 jump-press counters.
- The frame number assigned to each jump press.
- The number of physics frames between consecutive jump presses for each player.
- A four-entry on-screen input history.
- Separate settings for the frame counter and jump-input tracker.

The input log records when the jump input reaches the game's input hook. The displayed gap is the spacing between that player's recorded jump presses. It is **not** a calculated success-tolerance window. Accurate click-slack analysis, macro import/export, and replay simulation are not implemented yet.

## Install

1. Download the `VYX-AI-Android64` artifact from the successful GitHub Actions run.
2. Install the `.geode` package using Geode's **Install from file** option.
3. Restart Geometry Dash.

The package is for Android 64-bit and is not a standalone APK.

## Build

The GitHub Actions workflow uses the Geode 5.11.2 SDK and builds the Android64 target for Geometry Dash 2.2081.
