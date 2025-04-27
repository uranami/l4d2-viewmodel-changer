# ViewModel Plugin for Left 4 Dead 2

This is a simple clientside plugin for **Left 4 Dead 2** that allows players to customize their weapon viewmodel position in real-time using console commands.

## 🔧 Features

- Adjust your viewmodel position directly in-game
- Works client-side without requiring server modifications

## 📥 Installation

1. go to the [Releases](../../releases) section of this repository.
2. Download the latest version's **ViewModel.dll** and **ViewModel.vdf**.
3. Place both files into your L4D2 addons folder:

```
<L4D2 game folder>/left4dead2/addons/
```

Example:
```
C:\Program Files (x86)\Steam\steamapps\common\Left 4 Dead 2\left4dead2\addons
```

4. Launch the game. The plugin will load automatically.

## 🎮 How to Use

Once youre in-game, open the console and use the following commands to adjust the weapon position:

```
viewmodel_offset_x <value>
viewmodel_offset_y <value>
viewmodel_offset_z <value>
```
Try changing the numbers to move the weapon left or right, up or down, or closer and farther on your screen until it looks the way you like.

## 🧩 Notes

- This plugin only works if your game is started with -insecure, or if you join a server with VAC disabled (started with -insecure).
- Make sure the game is not running while copying the plugin files.
## 💬 Feedback

Found a bug or have suggestions? Feel free to open an [Issue](../../issues) or start a discussion!
