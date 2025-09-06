# windows-shutdown.exe

A small, simple Windows utility that shows a minimal UI for locking, sleeping, restarting, or shutting down the system. It supports a lightweight configuration file (`config.txt`) for language, automatic actions, delay, and background color.

<p align="center">
  <img src="windows-shutdown/assets/config.png" alt="config" width="80" style="margin-right:2px" />
  <img src="windows-shutdown/assets/lock.png" alt="lock" width="80" style="margin-right:2px" />
  <img src="windows-shutdown/assets/restart.png" alt="restart" width="80" style="margin-right:2px" />
  <img src="windows-shutdown/assets/shutdown.png" alt="shutdown" width="80" style="margin-right:2px" />
  <img src="windows-shutdown/assets/sleep.png" alt="sleep" width="80" />
</p>

## Features

- Minimal GUI with large countdown and action labels.
- Actions supported: Sleep, Shutdown, Restart, Lock.
- Optional automatic action after a configurable delay.
- Configurable language (English/Chinese) and background color.
- Creates a `config.txt` next to the executable when missing (UTF-8 with BOM).

## Interactive mode

When `config.txt` is missing or `action=none`:

Double click the executable to run in interactive mode. The program shows a menu with actions to choose from. Click an action button to perform the selected action.

## Timed shutdown (automatic action) ⏱️

When `config.txt` contains a valid `action` key with values like `sleep`, `shutdown`, `restart`, or `lock`, the program runs in automatic mode. Set `delay` (seconds) to control how long the program waits before performing the action.

Example (automatic shutdown after 60 seconds):

```text
action=shutdown
delay=60
```

> Note: `delay` must be a non-negative integer (default is 3 seconds).

## Custom action with custom shortcut icon 🖼️

If you want to create a shortcut for `sleep`, you can first create a shortcut to `windows-shutdown.exe`, then edit the config file next to the EXE to set `action=sleep`(with any desired delay). Save the config file, then use that shortcut to run the program with the sleep action.

And all icons of the actions are included in the executable.

## `config.txt` — detailed reference 🛠️

When the program runs it looks for `config.txt` next to the executable. If missing, the program will create a default file in UTF-8 with BOM.

Valid keys (case-sensitive):

- `language` — UI language. Allowed values:

  - `zh` — Chinese (default uses system language to decide)
  - `en` — English

- `action` — Automatic action mode. Allowed values:

  - `none` — (default) show menu and wait for user selection
  - `sleep` — put system to sleep after `delay` seconds
  - `shutdown` — shutdown the system after `delay` seconds
  - `restart` — restart the system after `delay` seconds
  - `lock` — lock the workstation after `delay` seconds

- `instruction` — whether to show the small instruction text under the countdown. Allowed values:

  - `show` — show instruction text (default)
  - `hide` — hide instruction text

- `delay` — seconds to wait before performing the automatic action. Must be a non-negative integer. Default: 4

- `backgroundColor` — background color of the UI. Accepted formats:
  - `#RRGGBBAA` — full ARGB hex (example: `#11223344`) where the last two hex digits are alpha
  - `#RRGGBB` — RGB hex (example: `#112233`), alpha defaults to a semi-transparent value

Behavior and warnings:

- Invalid or malformed values are ignored and the defaults are used. The application collects warnings and displays them in the UI when `config.txt` contains invalid entries.
- Accepted color examples:
  - `#00000034` — black with semi-transparency (default)
  - `#FF0000FF` — red with full alpha (opaque)

### Example `config.txt` files

Interactive, English UI with semi-transparent black background:

language=en
action=none
instruction=show
delay=4
backgroundColor=#00000034

Automatic sleep in 30 seconds, hide instruction:

language=zh
action=sleep
instruction=hide
delay=30
backgroundColor=#00000034

## Where to find the config file

The application writes or reads `config.txt` from the same folder as the executable. If you run the app from a release build folder, place `config.txt` next to the `.exe` in that folder.

## Troubleshooting

- If an action like shutdown or restart fails, the program will display an error message: the process may need elevated privileges to perform system shutdown/restart.
- If `config.txt` contains invalid lines, the invalid entries are skipped and a warning appears in the UI.

## Building / Running (developer notes)

This project uses CMake. There are example build folders (`build/`, `build-release/`) in the repo. Typical steps on Windows with Ninja:

- Configure and build (Release):

# run these in PowerShell in the repo root

cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --config Release

After build, copy or run the produced `windows-shutdown.exe` from `x64/Release/` or your chosen output folder.

## License & Credits

This README documents the small utility in this repository. See source files for more implementation details. 🧰
