# Othelo

A Windows console implementation of Othello (Reversi) in C++. Play locally against another player or against the computer, view valid moves, and track results on the scoreboard. The menu supports saving and loading games.

## Build and run

Use a Windows C++ toolchain. From a Visual Studio Developer Command Prompt in the repository directory, compile and run the game with:

```cmd
cl /EHsc /utf-8 Othelo.cpp
Othelo.exe
```

Run it from a writable directory so the game can save its local files. Use the numbered options in the on-screen menu to start or load a game and change modes.
