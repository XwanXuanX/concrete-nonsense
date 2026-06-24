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

## More Complex Example

Even with binary or ternary operators, the interface remains the same.

```sql
SELECT users.name, orders.amount
FROM users
JOIN orders ON users.id = orders.user_id
WHERE users.age > 30;
```

Query plan:
```
Projection(users.name, orders.amount)
    |
HashJoin(users.id = orders.user_id)
    /                         \
Filter(users.age > 30)        TableScan(orders)
    |
TableScan(users)
```

For now, focus on a simple join algorithm - `Nested loop join`:

```cpp
class NestedLoopJoin : public Operator {
private:
    std::unique_ptr<Operator> left;
    std::unique_ptr<Operator> right;

    std::optional<Tuple> current_left;

public:
    void open() override {
        left->open();
        right->open();

        current_left = left->next();
    }

    std::optional<Tuple> next() override {
        while (current_left.has_value()) {
            while (true) {
                std::optional<Tuple> right_tuple = right->next();

                if (!right_tuple.has_value()) {
                    break;
                }

                if (join_condition(*current_left, *right_tuple)) {
                    return combine(*current_left, *right_tuple);
                }
            }

            // Finished scanning right side for this left tuple.
            // Move to next left tuple and restart the right scan.
            right->close();
            right->open();

            current_left = left->next();
        }

        return std::nullopt;
    }

    void close() override {
        left->close();
        right->close();
    }
};
```

Note that even a complex operator still produces one tuple at a time to its parent.
