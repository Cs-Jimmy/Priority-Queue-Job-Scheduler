# 🖨️ Printer Job Scheduler

## 📋 Overview

This program simulates a printer job scheduling system. It reads print jobs from a file, organizes them in a priority queue based on arrival time and job duration, then processes each job in the correct order while calculating waiting times and execution metrics.

The program manages multiple print jobs from different workstations (PCs) and determines the optimal execution order to process all jobs efficiently.

## ⚡ Core Functionalities

### Job Management
- Reads print jobs from input file (`Printer.txt`)
- Stores job information including PC name, arrival time, and duration
- Converts time format from HH:MM to minutes for easier processing
- Tracks execution start time and waiting time for each job

### Priority Scheduling
- Implements a two-level priority system:
  - **Primary priority**: Earlier arrival time (jobs that arrive first get processed first)
  - **Secondary priority**: Shorter duration (if two jobs arrive at the same time, the shorter job goes first)
- Maintains jobs in a priority queue using a linked list structure

### Time Calculations
- Calculates when each job actually starts executing
- Computes waiting time for each job (difference between arrival and execution start)
- Tracks total printer occupation time
- Calculates average waiting time across all jobs

### Output Generation
- Displays each job's details and execution information
- Shows execution order and timing
- Provides system performance metrics

## 📁 Input Format

The program expects a file named `Printer.txt` with the following structure:

```
[Number of jobs]
[PC_name] [Arrival_time] [Duration_in_minutes]
[PC_name] [Arrival_time] [Duration_in_minutes]
...
```

## Example Input File (Printer.txt):
```
5
PC1 09:30 15
PC2 09:45 10
PC3 09:35 20
PC4 10:00 5
PC5 09:50 12
```

## Sample Output

Using the example input above, the program produces:

```
Jobs Executed: 5
PC1  09:30 15    executed at 09:30    waiting time: 0
PC3  09:35 20    executed at 09:45    waiting time: 10
PC2  09:45 10    executed at 10:05    waiting time: 20
PC5  09:50 12    executed at 10:15    waiting time: 25
PC4  10:00 5     executed at 10:27    waiting time: 27

The printer was occupied for 62 minutes

Average waiting time for all jobs is 16.40 minutes
```

## 🔄 Program Flow

1. **File Reading**: Program reads the number of jobs and job details from `Printer.txt`
2. **Queue Building**: Each job is added to the priority queue in the correct position
3. **Job Processing**: Jobs are removed from the queue one by one in priority order
4. **Execution Simulation**: For each job:
   - Determines actual start time (current time or arrival time, whichever is later)
   - Calculates waiting time
   - Updates current time by adding job duration
5. **Results Display**: Shows execution details and calculates final statistics

## 🧱 Key Classes

### JobInfo
- Stores individual job data (PC name, arrival time, duration)
- Handles time conversions
- Tracks execution metrics (start time, waiting time)

### Node  
- Represents a single element in the linked list
- Contains a JobInfo object and pointer to next node

### PriorityQueue
- Manages the collection of jobs in priority order
- Provides methods to add jobs (enqueue) and remove jobs (dequeue)
- Maintains proper ordering based on priority rules
