# Algebra

## Physical Algebra

- Defines how the database engine will actually execute that request algorithmically
- Formed by a group of physical operators
- Physical operator = Algorithm
  - e.g. Hash join, merge Join
- System specific
- Can estimate cost (since directly executable)

## Logical Algebra

- More high-level, more abstract
- Defines what queries can be expressed in the data model
- Like a high-level API that all implementation must agree on
  - e.g. JOIN
- Cannot estimate cost (not executable)

## Logical to Physical Mapping

The mapping is complex, done by query optimizer

4 reasons why mapping is complex:
  - A physical operator may implement multiple logical operators
  - A physical operator may implement only part of a logical operator
  - Some physical operators do not exist in logical algebra
  - Some properties that hold for logical operators do not hold for physical counterparts
