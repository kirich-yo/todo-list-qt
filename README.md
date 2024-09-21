# todo-list-qt
This is a small pet project that allows you to keep a to-do list.

## Screenshots

![ToDo List on Windows](https://github.com/kirich-yo/todo-list-qt/blob/master/res/screenshots/1.jpg?raw=true)

![ToDo List on Linux](https://github.com/kirich-yo/todo-list-qt/blob/master/res/screenshots/2.jpg?raw=true)

## How to build ToDo List

Open your terminal and run these commands:

```shell
cmake [path_to_todo_list]
make
```

## Linux build troubleshooting

You have to install CMake and Qt framework for a successful building.

Ubuntu/Debian:

```shell
sudo apt install cmake
sudo apt install qt6-base-dev
```

Arch Linux:

```shell
sudo pacman -S cmake
sudo pacman -S qt6-base
```

Fedora/Red Hat Enterprise/CentOS Stream:

```shell
sudo dnf install cmake
sudo dnf install qt6-qtbase-devel
```

## Changelog

1.1:
- Small bugfix

1.0:
- First version
