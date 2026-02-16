# fatpup
Super lightweight portable zero-dependency chess library written in simple C++, thoroughly test-covered. It implements an easy to use chess board and move classes you can use for you chess UI. Chess rules are very complicated if you consider all the scenarios of castling, pinned pieces and such - 9 out of 10 hobby chess projects on GitHub cannot even handle en passant and/or castling properly. Fatpup will get all the moves right and tell you if the position is check/checkmate/stalemate using next to zero CPU/RAM.

It's not a chess engine though, i.e. it won't tell you which move is the best. Fatpup objective is something like an online chess server running on Raspberry Pi handling 1M games simultaneously. Although there's a simple minimax engine that demonstrates how to use the API, as well as dependent github projects focusing on cross-platform chess UI (based on [SDL2](https://github.com/witaly-iwanow/fatpup-ui-sdl) and [SFML2](https://github.com/witaly-iwanow/fatpup-ui-sfml)).

## UCI support
Fatpup now ships with a basic UCI executable (`fatpup_uci`) backed by the built-in minimax engine.

Build:
```
cmake -S . -B build
cmake --build build -j
```

Run:
```
./build/fatpup_uci
```

### Ruy Lopez (standard UCI)
`>` = you type, `<` = engine reply.

```text
> position startpos moves e2e4
> go
< bestmove e7e5

> position startpos moves e2e4 e7e5 g1f3
> go
< bestmove b8c6

> position startpos moves e2e4 e7e5 g1f3 b8c6 f1b5
> go
< bestmove f8d6
```

### Ruy Lopez (extension commands)
`game [white] e2e4` starts a new game as White, plays your first move, and returns engine reply. To play with Black use `game black`.

```text
> game e2e4
< bestmove e7e5

> g1f3
< bestmove b8c6

> f1b5
< bestmove a7a6
```

and now to demonstrate the takeback functionality:
```text
> back
< info string rolling back to b8c6

> back
< info string rolling back to e7e5

> f4f2
< bestmove e5f4

```

![Screenshot](screenshots/chess-game.png)
![Screenshot](screenshots/possible-moves.png)
