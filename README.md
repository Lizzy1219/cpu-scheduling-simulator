# CPU Scheduling Simulator (C++)
This project demonstrates process scheduling algorithms including FCFS, SJF, Priority, Round Robin, and SRTF, complete with performance metrics and Gantt chart output.

## Features
- 5 scheduling modes: FCFS, SJF, Priority (non-preemptive), Round Robin, SRTF
- Custom process input: Arrival Time, Burst Time, Priority (if applicable)
- Time Quantum support for Round Robin
- Text-based Gantt Chart output
- Automatic calculation of:
  - Turnaround Time
  - Waiting Time
  - Average turnaround and waiting time

## Compile:
```bash
g++ -std=c++17 cpu_sched.cpp -o cpu_scheduler
```

## Sample Output (FCFS)
```plaintext
 Process | Arrival | Burst | Start | Completion | Turnaround | Waiting
---------------------------------------------------------------------
   P1         0        4       0          4           4           0
   P2         1        3       4          7           6           3
   P3         2        1       7          8           6           5

Gantt Chart:
|  P1  |  P2  |  P3  |
  0     4     7     8
```

## Author
Lizzy
