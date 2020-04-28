# Multi-threaded TCP Server

Client / Server programs to demonstrate socket programming.

This is a simple implementation of a multi-threaded TCP Server handling multiple clients.

## ⚙️ Compile

Open up a command-line shell and use `make` inside the repository

Recommended `make` version is [GNU Make 4.3](https://www.gnu.org/software/make/)

```bash
$ make
✔ client.c : client
✔ server.c : server
```

## ⚡️ Run

To run the server, execute `./bin/server`:

```bash
$ ./bin/server
✔ Socket successfully created.
✔ Socket options have been set.
✔ Socket successfully binded.
Server is listening on 127.0.0.1:8080..
```

To run the client, execute `./bin/client`:

```bash
$ ./bin/client
✔ Socket successfully created.
✔ Connected to server (127.0.0.1:8080).
CLIENT> 
```

## 👍 Contribute

No guidelines, feel free to submit any issue or PR.

<details>
  <summary>📚 Notes</summary>

### Socket 🔌
A socket is an internal endpoint for sending or receiving data within a node on a computer network.


### Thread 🕸
A thread is an entity within a process that consists of the schedulable part of the process. A fork() duplicates all the threads of a process.
</details>