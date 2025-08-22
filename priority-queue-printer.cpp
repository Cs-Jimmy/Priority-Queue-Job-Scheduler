#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

class JobInfo
{
public:
    string PC_name, Arrival_time; // eg. PC2, 09:30
    // Arrival_time will be converted to Minutes for easier comparison
    // eg. 09:30 -> 570 min
    int Arrival_minutes, Duration; //
    // Minutes is considered as 1st priority and Duration is the 2nd priority
    int ExecutionTime = 0; // When the job started executing
    int WaitingTime = 0;   // How long the job waited before execution: ExecutionTime-Arrival_minutes
    JobInfo(string pc, string time, int dur)
    {
        PC_name = pc;
        Arrival_time = time;
        Duration = dur;
        Arrival_minutes = ConvertToMinutes(time);
    }
    // Static function that can work in JobInfo constructor for easier time handling
    // Converts hour format (hh:mm) to minutes
    static int ConvertToMinutes(const string &timeStr)
    {
        int hrs, mins;
        char colon;
        stringstream TimeExctract(timeStr);
        // Exctract hours and minutes ignoring the colon
        TimeExctract >> hrs >> colon >> mins;
        return hrs * 60 + mins;
    }
    // Used for execution time displaying from minutes to hour format (hh:mm)
    static string MinutesToHourFormat(int minutes)
    {
        int hrs = minutes / 60;
        int mins = minutes % 60;
        stringstream ss;
        ss << setw(2) << setfill('0') << hrs << ":" << setw(2) << setfill('0') << mins;
        // setw and setfill are used to adjust time to the correct format: hh:mm
        return ss.str();
    }
    void display()
    {
        cout << PC_name << "  " << Arrival_time << " " << Duration << "    ";
    }
};

class Node
{
public:
    JobInfo job; // Each node contains an object of JobInfo
    // Which contains the PC name, arrival time, and duration
    Node *next;
    Node() : job("", "", 0), next(nullptr) {} // default constructor to initialize the next pointer to null
    Node(JobInfo j, Node *n = nullptr) : job(j), next(n) {};
};

class PriorityQueue
{
private:
    Node *front; // Head of the list and first job to be printed
public:
    PriorityQueue()
    {
        front = nullptr; // intialization with empty list
    }
    bool isEmpty()
    {
        return front == nullptr;
    }
    // Returns true if 'a' has higher priority, and false otherwise
    bool hasHigherPriority(JobInfo a, JobInfo b)
    {
        // 1st priority: comparing based on time of arrival
        if (a.Arrival_minutes != b.Arrival_minutes)
        {
            return a.Arrival_minutes < b.Arrival_minutes;
        }
        // 2nd priority: if the two arrival times are equal compare based on waiting time
        return a.Duration < b.Duration;
    }
    void enqueue(JobInfo job)
    {
        Node *N = new Node(job); // adding a new node containing JobInfo object
        // Queue is empty or job has higher priority
        if (isEmpty() || hasHigherPriority(job, front->job))
        {
            N->next = front;
            front = N; // Same as add to head function
        }
        // Inserting job to the correct position (not the front)
        else
        {
            Node *ptr = front;
            while (ptr->next != nullptr && !hasHigherPriority(job, ptr->next->job))
            {
                ptr = ptr->next; // moves ptr to the next node
            }
            N->next = ptr->next;
            ptr->next = N;
        }
    }
    JobInfo dequeue()
    {
        if (isEmpty())
        {
            cerr << "The queue is empty!" << endl;
            return JobInfo("", "", 0); // returns an empty job
        }
        else // Not empty case
        {
            Node *temp = front;       // Start from the beginning of the queue
            JobInfo job = front->job; // Store the front job before dequeuing it
            front = front->next;      // Move to the next node
            delete temp;
            return job;
        }
    }
    void display_jobs()
    {
        Node *current = front;
        while (current != nullptr)
        {
            current->job.display();
            current = current->next;
        }
    }
};

int main()
{
    ifstream file("Printer.txt");
    if (!file)
    {
        cerr << "Can't open file! ";
        return 1;
    }
    int NumJobs;
    file >> NumJobs;
    PriorityQueue pq;
    for (int i = 0; i < NumJobs; ++i)
    {
        string pc, time;
        int duration;
        file >> pc >> time >> duration;
        pq.enqueue(JobInfo(pc, time, duration));
    } // Enqueue each job in the file
    cout << "Jobs Executed: " << NumJobs << endl;
    int CurrentTime = 0, TotalWaitingTime = 0, TotalOccupationTime = 0;
    while (!pq.isEmpty())
    {
        JobInfo job = pq.dequeue();
        if (CurrentTime < job.Arrival_minutes)
        {
            CurrentTime = job.Arrival_minutes;
        } // when CurrentTime is still earlier than Arrival, then the CurrentTime is updated
        job.ExecutionTime = CurrentTime; // Time when the job actually starts
        job.WaitingTime = CurrentTime - job.Arrival_minutes;
        TotalWaitingTime += job.WaitingTime;
        TotalOccupationTime += job.Duration;
        job.display();
        cout << "executed at " << JobInfo::MinutesToHourFormat(job.ExecutionTime) << "    ";
        cout << "waiting time: " << job.WaitingTime << endl;
        CurrentTime += job.Duration; // Update clock
    }
    cout<<"\nThe printer was occupied for "<<TotalOccupationTime<<" minutes"<<endl;
    cout<<"\nAverage waiting time for all jobs is "<<fixed << setprecision(2)
    << static_cast<float>(TotalWaitingTime) / NumJobs << " minutes" << endl<<"\n"; //report result
    return 0;
}