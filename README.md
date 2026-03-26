# AutorunReloadFonts

# TC-Autorun-ReloadFonts

A lightweight C++14 dynamic-link library (DLL) designed for the **Autorun** plugin in Total Commander. It enables dynamic registration of system fonts and forces Total Commander to recognize them immediately without a restart.

## Features
- **Hot Reload**: Triggers a system-wide `WM_FONTCHANGE` notification, making new fonts selectable in Total Commander dialogs instantly after loading.
- **Zero Overhead**: The library performs its task and does not remain resident in memory (utilizing the Autorun adapter interface).
- **Portable-friendly**: Optimized for portable setups (e.g., PortableApps) where fonts are stored within the application directory.

## Technical Overview
When the `ReloadFonts` function is called:
1. The library issues a `WM_FONTCHANGE` broadcast message via `SendNotifyMessage`. This instructs Windows to update the available font table for all active dialogs.
2. An internal `WM_USER + 21` message is sent to the Total Commander window handle to trigger a UI refresh and configuration update.

## Installation and Configuration

1. Compile the project using Visual Studio 2022+ (MSVC).
2. Place the resulting `AutorunReloadFonts.dll` into your plugins directory (e.g., `%COMMANDER_PATH%\Plugins\wdx\Autorun\Plugins\`).
3. Add the following calls to your **Autorun** configuration file (`autorun.cfg`):

```cfg
# Load the library
LoadLibrary Plugins\AutorunReloadFonts.dll

# Load required fonts using Autorun commands
LoadFont "%COMMANDER_PATH%\Fonts\JetBrainsMono.ttf"
LoadFont "%COMMANDER_PATH%\Fonts\IBMPlexSans.ttf"

# Apply changes
ReloadFonts
```

## Build Requirements
- **ISO C++14 Standard**.
- **Windows SDK**.
- The build architecture (x86/x64) must match the bitness of your Total Commander installation.
