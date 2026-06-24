# Iterator Model / Volcano Model

Run all physical operators in one process and call each other like normal functions

## Example

```
Print -> Projection -> Selection -> Table Scan
```

The execution:
```
while (auto row = projection.next()) {
  print(row);
}
```

But `projection.next()` may call `selection.next()`, and so on:
```
projection.next() -> selection.next() -> table_scan.next()
```

### Core idea: Pull-based

The root of the query plan asks for one output tuple. To produce that tuple, it calls its child operator, which may recursively call its own child operators.

- **Control flows downward through the operator tree**
  - e.g. `projection` demands from `selection`, which demands from `scan`, always asking the operators in the level below
- **Until a source operator**
  - A leaf node in the operator tree
  - e.g. `Table Scan`
- **Produce one record**
  - e.g. `Table Scan` scanned some files and produced one rowset
- **Record flows back upward through the operator tree**
  - e.g. The rowset was returned by `Table Scan`, and got processed while traversing upward

## Pros

- Simple and modular operator interface
- Operators can be composed into arbitrary query plans
- Avoids expensive IPC via OS
- Pipelined execution

## Cons

- No natural parallelism
- Tuple-at-a-time function call have high overhead
- Poor cache and CPU efficiency compared with vectorized execution
- Hard to exploit SIMD and modern CPU features

