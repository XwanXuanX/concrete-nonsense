# Iterator Model Interface

## General Operator Interface

```
interface Operator {
  open();
  next() -> tuple;
  close();
};
```

## Example

Imagine this query:
```sql
SELECT name FROM users WHERE age > 30;
```

The physical plan would be:
```
Projection(name)
      |
Filter(age > 30)
      |
TableScan(users)
```

#### Table Scan operator

```
class TableScan : Operator {
  def next() {
    return read_next_row_from_table();
  }
  
  def open() {
    // open file descriptors
  }

  def close() {
    // close file descriptors
  }
};
```

#### Filter operator

```
class Filter : Operator {
  Operator child;

  def next() {
    while(true) {
      row = child.next();
      if (row is None)
        return None;
      if (row["age"] > 30)
        return row;
    }
  }

  def open() { // open input }
  def close() { // close input }
};
```

#### Projection operator

```
class Projection : Operator {
  Operator child;

  def next() {
    row = child.next();
    if (row is None)
      return None;
    return {"name" : row["name"]};
  }

  def open() { // open input }
  def close() { // close input }
};
```

#### Driver

```
while(true) {
  row = projection.next();
  if (row is None) {
    break;
  }
  emit(row);
}
```

## Advantage

Flexibility:
- Can compose query plans almost arbitrarily by linking different operators together
- Operator itself does not need to know the type of child operator
- No need to change operator's internal code
