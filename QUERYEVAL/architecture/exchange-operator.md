# Exchange Operator

Traditional iterator model:
- Does not take advantage of parallel execution
- Has high tuple-at-a-time function call overhead

Exchange Operator:
- Introduces parallelism in a modular way
- Provides clean physical algebra for parallel plans

## Core Idea

Imagine this simple sequential plan:
```
Projection
    |
Filter
    |
TableScan
```

How do we run `Filter` and `TableScan` in parallel (while keeping the iterator interface)?

Insert a `Exchange` operator:
```
Projection
    |
Filter
    |
Exchange
    |
TableScan
```

The `Exchange` operator internally starts another thread that runs the `TableScan` and pushes produced tuples into a queue.
```
Thread 1: Filter consumes tuples
Thread 2: TableScan produces tuples
Queue:   connects them
```

## Producer/Consumer Queue

The exchange operator is basically a producer/consumer queue.

```
Thread A:
    Filter
      |
    Exchange consumer side

Thread B:
    Exchange producer side
      |
    TableScan
```



```mermaid
graph TD
    %% Define Context Styling
    classDef context1 fill:#d1e7dd,stroke:#0f5132,stroke-width:2px,color:#0f5132;
    classDef context2 fill:#fff3cd,stroke:#664d03,stroke-width:2px,color:#664d03;
    classDef context3 fill:#cff4fc,stroke:#087990,stroke-width:2px,color:#087990;
    classDef context4 fill:#f8d7da,stroke:#842029,stroke-width:2px,color:#842029;
    classDef exchange fill:#e2e3e5,stroke:#41464b,stroke-width:3px,stroke-dasharray: 5 5,color:#41464b;

    %% ==========================================
    %% EXECUTION CONTEXT 4
    %% ==========================================
    subgraph C4 [Execution Context 4]
        Op9[Projection]
    end
    class C4,Op9 context4;

    %% ==========================================
    %% EXCHANGE 3
    %% ==========================================
    Ex3["EXCHANGE 3 <br> (Ctx 3 -> Ctx 4)"]
    class Ex3 exchange;

    %% ==========================================
    %% EXECUTION CONTEXT 3
    %% ==========================================
    subgraph C3 [Execution Context 3]
        Op8[Aggregation]
        Op7[Hash Join]
        
        Op8 --> Op7
    end
    class C3,Op8,Op7 context3;

    %% ==========================================
    %% EXCHANGES 1 & 2
    %% ==========================================
    Ex1["EXCHANGE 1 <br> (Ctx 1 -> Ctx 3)"]
    Ex2["EXCHANGE 2 <br> (Ctx 2 -> Ctx 3)"]
    class Ex1,Ex2 exchange;

    %% ==========================================
    %% EXECUTION CONTEXT 1
    %% ==========================================
    subgraph C1 [Execution Context 1]
        Op4[Aggregation]
        Op3[Table Scan]
        
        Op4 --> Op3
    end
    class C1,Op4,Op3 context1;

    %% ==========================================
    %% EXECUTION CONTEXT 2
    %% ==========================================
    subgraph C2 [Execution Context 2]
        Op2[Filter]
        Op1[Table Scan]
        
        Op2 --> Op1
    end
    class C2,Op2,Op1 context2;

    %% ==========================================
    %% CONNECTIONS (Data Flow)
    %% ==========================================
    Op9 -->|Fetches final rows| Ex3
    Ex3 -->|Network Transfer| Op8
    
    Op7 -->|Left Input| Ex1
    Op7 -->|Right Input| Ex2
    
    Ex1 -->|Network Shuffle| Op4
    Ex2 -->|Network Shuffle| Op2
```

