### C Parse URI

URI Percent encode/decode written in C with Python bindings.

Lots of inspiration from libevent, libcurl, and a very nice german named Jochen.

#### Installation

```
python setup.py install
```

#### Usage

```
>>>import cparseuri
>>>cparseuri.encode("Hello World")
Hello%20World
>>>cparseuri.decode("Hello%20World")
Hello World

#### Run Tests

```
nosetests
```

