# windows-shutdown.exe

A small, simple Windows utility that shows a minimal UI for locking, sleeping, restarting, or shutting down the system. It supports a lightweight configuration file (`config.txt`) for language, automatic actions, delay, and background color.

<p align="center">
  <img src="windows-shutdown/assets/donate.png" alt="donate" width="60" style="margin-right:2px" />
  <img src="windows-shutdown/assets/config.png" alt="config" width="60" style="margin-right:2px" />
  <img src="windows-shutdown/assets/lock.png" alt="lock" width="60" style="margin-right:2px" />
  <img src="windows-shutdown/assets/sleep.png" alt="sleep" width="60" />
  <img src="windows-shutdown/assets/restart.png" alt="restart" width="60" style="margin-right:2px" />
  <img src="windows-shutdown/assets/shutdown.png" alt="shutdown" width="60" style="margin-right:2px" />
</p>

## Features

- Minimal GUI with large countdown and action labels.
- Actions supported: Sleep, Shutdown, Restart, Lock.
- Optional automatic action after a configurable delay.
- Configurable language (English/Chinese) and background color.
- Creates a `config.txt` next to the executable when missing (UTF-8 with BOM).
- Beautiful Steins;Gate-inspired countdown design! (Normal design is also available.)

<p align="center">
  <img src="draft/example-en.png" alt="NixieTube" width="320" />
</p>

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
