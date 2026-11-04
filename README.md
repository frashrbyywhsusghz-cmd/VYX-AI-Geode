# VYX AI for Geometry Dash

Android-first Geode mod project targeting Geometry Dash 2.2081+.

## Current status

This repository is the initial project foundation. The first build adds a VYX AI entry button to the Geometry Dash main menu. The AI reader, replay engine, level generator, decoration generator, and online AI service are planned modules and are **not implemented yet**.

## Planned modules

- **AI Reader:** inspect a level and report whether analysis completed.
- **Local analysis:** physics-aware path and collision analysis.
- **Replay:** reproduce a saved input sequence with configurable timing.
- **Level Creator:** generate a gameplay layout from a text prompt and difficulty.
- **Decoration Creator:** generate decoration plans and visual themes from a prompt.
- **Online AI bridge:** optional remote generation service. No API key is embedded in this repository.

## Android build

GitHub Actions builds an Android 64-bit `.geode` package when code is pushed to `main`, and can also be started manually from the Actions tab.

1. Open the **Actions** tab.
2. Select **Build Android64 Geode Mod**.
3. Press **Run workflow**.
4. When it finishes, open the run and download the `VYX-AI-Android64` artifact.
5. Extract the `.geode` file and install it with Geode.

This produces a Geode mod package, not a standalone APK. The Android 32-bit target is not enabled yet.

## Install location

`Android/media/com.geode.launcher/game/geode/mods/`

## Development

The project uses the official Geode SDK and the `geode-sdk/build-geode-mod` GitHub Action. See the [Geode modding documentation](https://docs.geode-sdk.org/).
