# HashMap - C Implementation of a Hash Map with Linked List

## Overview

Welcome to the `c_map` repository! This C program provides an implementation of a map (dictionary) using linked lists, with a strong emphasis on an efficient hash map structure. The implementation incorporates strategies to handle collisions, ensuring reliable storage and retrieval of key-value pairs in the map.

This project serves as an educational resource for understanding not only linked lists and memory management but also the intricacies of designing a hash map in the C programming language.

### Hash Map Design and Collision Avoidance

The hash map in this implementation employs a well-thought-out design to manage collisions, a common challenge in hash-based data structures. The `getBucket` function calculates the bucket index for a given key, distributing keys across different buckets to mitigate collisions.

In the `__Map_put` method, collisions are handled by checking if the key already exists in the hash map. If a collision occurs, a new entry is created and linked to the existing entries in the same bucket, preventing data loss or overwrites.

The `getBucket` function uses a simple hash function to generate a hash value for each key. This value is then modulated by the number of buckets to determine the bucket index, ensuring a uniform distribution of keys.

## Table of Contents

- [Why Use This Script](#why-use-this-script)
- [Prerequisites](#prerequisites)
- [Languages and Utilities Used](#languages-and-utilities-used)
- [Environments Used](#environments-used)
- [Hash Map Design](#hash-map-design)
- [Functions](#functions)
- [Usage](#usage)

## Why Use This Script

As a junior developer, exploring this script can provide you with insights into:

- **Efficient Hash Map Implementation:** Understand the design choices made to create a hash map that efficiently manages collisions, providing a robust solution for storing and retrieving data.

- **Handling Collisions:** Learn how collisions are handled in the `__Map_put` method, ensuring that multiple keys can coexist within the same bucket without compromising data integrity.

## Prerequisites

Before using this C program, ensure that you have the following:

- A C compiler installed on your system.
- Basic understanding of C programming, including linked lists and memory allocation concepts.
- Review: [C Implementation of a Hash Map with Linked List](https://github.com/infinity-set/hash_map)

## Languages and Utilities Used

- **C**
- **Visual Studio Code**

[<img  alt="C Logo Icon" width="45px" src="https://upload.wikimedia.org/wikipedia/commons/1/18/C_Programming_Language.svg" />][C]
[<img alt="Visual Code Icon" width="45px" src="https://upload.wikimedia.org/wikipedia/commons/9/9a/Visual_Studio_Code_1.35_icon.svg" />][vscode]

[C]: https://learn.microsoft.com/en-us/cpp/c-language/
[vscode]: https://code.visualstudio.com/

## Environments Used

- **Windows**

[<img align="left" alt="Microsoft Icon" width="35px" src="https://upload.wikimedia.org/wikipedia/commons/3/34/Windows_logo_-_2012_derivative.svg" />][windows]

[windows]: https://www.microsoft.com/

<br /><br />

## Hash Map Design

### Hash Map Entry Structure (`MapEntry`)

- `char *key`: Pointer to the key string.
- `int value`: Value associated with the key.
- `struct MapEntry *__prev`: Pointer to the previous map entry.
- `struct MapEntry *__next`: Pointer to the next map entry.

### Function to Calculate Bucket Index (`getBucket`)

- Calculates the bucket index for a given key using a hash function.

### Handling Collisions (`__Map_put`)

- Creates a new entry and links it to existing entries in the same bucket to handle collisions gracefully.
  - **Python Syntax:**
    ```python
    existing_map[key] = value
    ```

## Functions

### `Map_new()`

- Creates a new empty map.
  - **C Syntax:**
    ```c
    Map *newMap = Map_new();
    ```
  - **Python Syntax:**
    ```python
    new_map = dict()
    ```

### `__Map_put(Map *self, const char *key, int value)`

- Adds or updates a map entry in the map, handling collisions efficiently.
  - **C Syntax:**
    ```c
    self->put(self, "example_key", 42);
    ```
  - **Python Syntax:**
    ```python
    existing_map["example_key"] = 42
    ```

### `__Map_get(Map *self, const char *key, int default_value)`

- Gets the value associated with a key in the map.
  - **C Syntax:**
    ```c
    int result = self->get(self, "example_key", -1);
    ```
  - **Python Syntax:**
    ```python
    result = existing_map.get("example_key", -1)
    ```

### `__Map_find(MapEntry *self, const char *key)`

- Finds a map entry in the map based on the key.
  - **C Syntax:**
    ```c
    MapEntry *result = self->find(self->__head, "example_key");
    ```
  - **Python Syntax:**
    ```python
    if "example_key" in existing_map:
        # Node found
    ```

### `__Map_iter(Map *self)`

- Creates a new iterator for the map.
  - **C Syntax:**
    ```c
    MapIterator *iter = self->iter(self);
    ```
  - **Python Syntax:**
    ```python
    iter = iter(existing_map)
    ```

### `__MapIter_next(MapIterator *self)`

- Gets the next map entry using the iterator.
  - **C Syntax:**
    ```c
    MapEntry *current = iter->next(iter);
    ```
  - **Python Syntax:**
    ```python
    current = next(iter)
    ```

### `__MapIter_del(MapIterator *self)`

- Deletes the iterator.
  - **C Syntax:**
    ```c
    iter->del(iter);
    ```
  - **Python Syntax:**
    ```python
    del iter
    ```

### Usage

The `main` function in the provided C code demonstrates the creation, appending, printing, deletion, and iteration of the hash map (`hash_map.c`). Explore and modify the code to suit your learning needs.

 <div align="center">
<br/>
<img src="https://github.com/infinity-set/c_map/assets/142350896/bdc4e4a3-7d9b-4df4-af0c-389af13d085b" height="100%" width="100%" alt="Main function code displayed in C"/>
<br />   
 </div>

 <div align="center">
<br/>
<img src="https://github.com/infinity-set/c_map/assets/142350896/3ddb0839-0cf2-4369-ab91-55e17d38dc78" height="100%" width="100%" alt="Output of the execution displayed."/>
<br />   
 </div>

<br />

Feel free to delve into the codebase to gain a deeper understanding of hash map design, collision handling, and their importance in efficient data storage.

#

⬅️ **[Home](https://github.com/infinity-set)**
