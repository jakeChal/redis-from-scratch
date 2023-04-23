Building my Redis based on [this awesome tutorial](https://build-your-own.org/redis/)


## Notes

### Chapter 3 - Hello Server/Client

From a high level, the client/server setup is as follows:


- Server
```cpp
fd = socket()
bind(fd, address)   // associate an address to a socket FD
listen(fd)          // accept connections to that address

while True:
    conn_fd = accept(fd)    // on a listening FD: return a
                            // FD for the connection socket
    do_something_with(conn_fd)
    close(conn_fd)
```

- Client

```cpp
fd = socket()
connect(fd, address)    // make a TCP connection to the address
do_something_with(fd)
close(fd)
```