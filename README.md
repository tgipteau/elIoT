# ELIoT

A custom home-automation system written in C: ESP32 boards around the house,
a message broker, and a small web interface to control them.

## Architecture

```
Browser ──HTTP/JSON──► bridge ──TCP (text)──► broker ◄──TCP── ESP32 boards
```

- **ESP32 boards** connect to the broker as clients.
- **Broker** (`broker.c`): central hub. It accepts connections from the boards
  and the bridge, and is meant to forward messages to subscribers by topic.
- **Bridge** (`bridge.c`): serves the web page and translates user actions
  into broker messages. It is a client of the broker, like the boards.
- **Web page** (`html/`): buttons that send JSON commands to the bridge
  (`POST /cmd`).

## Repository contents

| Path | Description |
|---|---|
| `broker.c` | Broker, built on Mongoose |
| `bridge.c` | Web server and broker client, built on Mongoose |
| `html/` | Static files for the web interface |
| `_resources/` | Mongoose source files (`mongoose.c`, `mongoose.h`) |
| `__olds/` | Earlier attempts: abandoned Node.js experiments, and a first broker written with the C standard socket API only |

## How a command travels

1. A button on the page sends a JSON command to the bridge, e.g.
   `{"room": "...", "device": "...", "action": "..."}`.
2. The bridge converts it into a text line in an MQTT-inspired format
   (`PUB topic payload`) and sends it to the broker over TCP.
3. The broker forwards it to the clients subscribed to that topic.

The protocol is deliberately simple: one text message per line, terminated
by `\n`. It is inspired by MQTT but is not MQTT-compatible.

## Status

- Working: bridge serves the page; buttons send `PUB` messages to the broker.
- In progress: the broker currently prints what it receives. Subscriptions
  (`SUB`) and routing by topic are not implemented yet.

## History

1. A first server written with plain sockets, tested with an ESP32 client
   (kept in `__olds/`; a learning project on socket programming).
2. A bridge prototype in Node.js, abandoned.
3. A bridge rewritten in C with Mongoose, to keep a pure C stack.
4. The broker is being rewritten with Mongoose too, for the same reason.

## License note

Mongoose is dual-licensed (GPLv2 or commercial). 
