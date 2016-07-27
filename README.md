Description
-----------

fakemultimon allows you to fake the number of screens you have to programs using xinerama, as well as change their ordering.

I use it with xfreerdp, which doesn't support /monitors and /multimon at the same time.

Building
--------

```
make
```

Usage
-----

```
LD_PRELOAD=./fakemultimon.so MULTIMON_MAPPINGS=[number of screens],[mapping from (1)]:[mapping to (1)],[mapping from (2)]:[mapping to (2)],[etc] [program to run] [args]
```

Examples
-------

Limit number of screens to 2, map screen 0 to 1, screen 2 to 0, and run xfreerdp:

```
LD_PRELOAD=./fakemultimon.so MULTIMON_MAPPINGS=2,0:1,2:0 xfreerdp /multimon /v:badger /u:bob /p:bobspass
```
