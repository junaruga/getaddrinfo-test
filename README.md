# getaddrinfo-test

This repository is to test a behavior of the C library `getaddrinfo` [1] is same with the Ruby's `Addrinfo.getaddrinfo` [2].

## Ruby `Addrinfo.getaddrinfo` method.

On the Ruby 3.0.0 that I tested, if the `/etc/hosts` has setting of both IPv4 (127.0.0.1) and IPv6 (::1), the method returns the 2 `Addrinfo`s of both IPv4 and IPv6.

```
$ cat /etc/hosts
127.0.0.1 localhost localhost.localdomain
::1       localhost localhost.localdomain localhost6 localhost6.localdomain6
```

```
$ irb
irb(main):001:0> require 'socket'
=> true

irb(main):002:0> Addrinfo.getaddrinfo('localhost', 0, nil, :STREAM, nil, Socket::AI_PASSIVE)
=> [#<Addrinfo: ::1 TCP (localhost)>, #<Addrinfo: 127.0.0.1 TCP (localhost)>]
```

As a notable behavior, `Addrinfo.getaddrinfo` method still returns the 2 `Addrinfo`s when only disalbing the settings by `sysctl`.

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=1
$ sudo sysctl -w net.ipv6.conf.default.disable_ipv6=1
$ sudo sysctl -w net.ipv6.conf.lo.disable_ipv6=1
```

```
irb(main):003:0> Addrinfo.getaddrinfo('localhost', 0, nil, :STREAM, nil, Socket::AI_PASSIVE)
=> [#<Addrinfo: ::1 TCP (localhost)>, #<Addrinfo: 127.0.0.1 TCP (localhost)>]
```


However if the `/etc/hosts` has the setting of the only IPv4 like this, the method returns the `Addrinfo` of IPv4.

```
# vi /etc/hosts

# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
# ::1         localhost localhost.localdomain localhost6 localhost6.localdomai
```

```
$ irb
irb(main):001:0> require 'socket'
=> true
irb(main):002:0> Addrinfo.getaddrinfo('localhost', 0, nil, :STREAM, nil, Socket::AI_PASSIVE)
=> [#<Addrinfo: 127.0.0.1 TCP (localhost)>]
```

## C `getaddrinfo` function.

The behavior of the C `getaddrinfo` function is same with the Ruby `Addrinfo.getaddrinfo` method.

```
$ cat /etc/hosts
127.0.0.1 localhost localhost.localdomain
::1       localhost localhost.localdomain localhost6 localhost6.localdomain6
```

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0
$ sudo sysctl -w net.ipv6.conf.default.disable_ipv6=0
$ sudo sysctl -w net.ipv6.conf.lo.disable_ipv6=0
```

```
$ gcc -o main main.c
```

```
$ ./main
addrinfo result AF_INET6
addrinfo result AF_INET
```

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=1
$ sudo sysctl -w net.ipv6.conf.default.disable_ipv6=1
$ sudo sysctl -w net.ipv6.conf.lo.disable_ipv6=1
```

```
$ ./main
addrinfo result AF_INET6
addrinfo result AF_INET
```

```
# vi /etc/hosts

# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
# ::1         localhost localhost.localdomain localhost6 localhost6.localdomai
```

```
$ ./main
addrinfo result AF_INET
```


## References

* [1] See `man 3 getaddrinfo`.
* [2] See `ri Addrinfo.getaddrinfo`.
