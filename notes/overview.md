# 1. Primitive constructs

From Toy lang's syntactic standpoint, a primitive construct is any type whose
definition does not require a record. For example,

```c++
let x: Int = 5; // primitive type
let x: Foo = {name = "foo0", data = []}; // not primitive type
```

## 1.1. Numeric types

## 1.2. Strings

## 1.3. Arrays

## 1.4. Tuples

- Unlike other constructs, a tuple's type is defined by its elements.

```c++
let (x, y) = (5, 1);
let (x: Int, $y) = (5, 3.1415);
```

- A tuple can contain any arbitrary type, including other tuples.

```c++
let ($x: Int, (y: Int[], z)) = (0, ([], "hello"));
```

- Tuples are useful for pattern matching.

```c++
(x, y, z) = f();
```

# 3. Attributes

Attributes are prefixed with '@'. For example, @shared, @alloc, etc.