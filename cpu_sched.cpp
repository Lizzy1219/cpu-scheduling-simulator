#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

//資料結構
struct process {
    int id; //流程編號
    int arrival; // 到達時間
    int burst; // 執行時間
    int start; // 開始時間
    int completion; // 完成時間
    int turnaround; // 周轉時間（進入系統到完成執行的時間）
    int waiting; // 等待時間
    int priority; //優先序（Priority才會使用到）
    int remaining_time; //剩餘時間
};

//資料輸入
vector<process> read_input(int& n, int choice){
    cout << "請輸入process數量：";
    cin >> n;
    vector<process> p(n);
    for(int i = 0 ; i < n ; i++){
        p[i].id = i + 1;
        cout << "請輸入the arrival time of P" << p[i].id << " : ";
        cin >> p[i].arrival;
        cout << "請輸入the burst time of P" << p[i].id << " : ";
        cin >> p[i].burst;
        p[i].remaining_time = p[i].burst; //初始化剩餘時間

        if(choice == 3){
            cout << "請輸入the priority of P" << p[i].id << " : ";
            cin >> p[i].priority;
        }
    }
    return p;
} 

//工具函式
void print_table(vector<process>& p, double& avewait, double& avetra, int choice){
    cout << endl << " Process | Arrival | Burst | Start | Completion | Turnaround | Waiting" ;
    if(choice == 3) cout << " | Priority" ;
    cout << endl;
    cout << "---------------------------------------------------------------------";
    if(choice == 3) cout << "-----------";
    cout << endl;


    sort(p.begin(), p.end(), [](const process& a, const process& b){
        return a.id < b.id;
    });
    for(auto i : p){
        cout << setw(4)  << "P" << i.id 
        << setw(10) << i.arrival 
        << setw(9) << i.burst 
        << setw(8) << i.start 
        << setw(11) << i.completion 
        << setw(12) << i.turnaround 
        << setw(12) << i.waiting;
        if(choice == 3) cout << setw(10) << i.priority;
        cout << endl;

        avewait += i.waiting;
        avetra += i.turnaround;
    }
    cout << endl;
    return;
}
void print_gantt_chart(vector<process>& p, vector<pair<int, int>>& g, int choice, int t_comp){
    cout << "Gantt Chart:" << endl;
    
    if(choice == 4 || choice == 5){
        for(auto i : g) cout << "|  P" << setw(2) << i.first << "  ";
        cout << "|" << endl;

        cout << setw(2);
        for(auto i : g) cout << i.second << setw(8);
        cout << t_comp;
    }
    else{
        sort(p.begin(), p.end(), [](const process& a, const process& b){
            return a.start < b.start;
        });
        for(auto i : p) cout << "|  P" << setw(2) << i.id << "  ";
        cout << "|" << endl;

        cout << setw(2) << p[0].start;
        for(auto i : p) cout << setw(8) << i.completion;
    }
    cout << endl; 
    return;
}
void print_ave(double& avewait, double& avetra, int n){
    avewait /= n;
    avetra /= n;
    cout << endl;

    cout << "Average Turnaround Time : " << fixed << setprecision(2) << avetra << endl;
    cout << "Average Waiting Time : " << fixed << setprecision(2) << avewait << endl;
    cout << endl;
}

//排程
void calculate_fcfs(vector<process>& p, int n){
    sort(p.begin(), p.end(), [](const process& a, const process& b){
        return a.arrival < b.arrival; //FCFS 先進先出
    });
    
    //建輸出表格
    int current = 0;
    for(int i = 0 ; i < n ; i++){
        if(p[i].arrival <= current) p[i].start = current;
        else {
            p[i].start = p[i].arrival;
            current = p[i].arrival;
        }
        p[i].completion = p[i].start + p[i].burst;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].start - p[i].arrival;
        current = p[i].completion;
    }
    return;
}
void calculate_sjf(vector<process>& p, int n){
    int current = 0, completed = 0;
    vector<bool> done(n, false);
    //找出目前可以執行的process
    while(completed < n){
        int index = -1, minTime = INT_MAX;
        for(int i = 0 ; i < n ; i++){
            if(done[i] || current < p[i].arrival) continue; //若已執行過或時間尚未到則找下一個
            if(p[i].burst < minTime){
                minTime = p[i].burst;
                index = i;
            }
        }
        if(index == -1){ //若找不到則時間++
            current++;
            continue;
        }
        p[index].start = current;
        p[index].completion = current + minTime;
        p[index].turnaround = p[index].completion - p[index].arrival;
        p[index].waiting = p[index].start - p[index].arrival;

        done[index] = true;
        current = p[index].completion;
        completed++;
    }
    return;
}
void calculate_priority(vector<process>& p, int n){
    int current = 0, completed = 0;
    vector<bool> done(n, false);

    while(completed < n){
        int min_pri = INT_MAX, idx = -1;
        for(int i = 0 ; i < n ; i++){
            if(!done[i] && p[i].arrival <= current && p[i].priority < min_pri){
                min_pri = p[i].priority;
                idx = i;
            }
        }

        if(idx == -1){
            current++;
            continue;
        }

        p[idx].start = current;
        p[idx].completion = p[idx].start + p[idx].burst;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].start - p[idx].arrival;

        current = p[idx].completion;
        done[idx] = true;
        completed++;
    }
    return;
}
int calculate_rr(vector<process>& p, int n, int qt, vector<pair<int, int>>& g){
    sort(p.begin(), p.end(), [](process a, process b){
        if(a.arrival == b.arrival) return a.id < b.id;
        return a.arrival < b.arrival; //按照到達順序排序，若相同則依照執行時間長度
    });

    queue<int> ready_queue;
    ready_queue.push(0);
    int current = p[0].arrival;
    int i = 1;
    int completed = 0;
    int t_comp;

    while(completed < n){
        while(i < n && p[i].arrival <= current){
            ready_queue.push(i);
            i++;
        }

        if (ready_queue.empty()) {
            if(i < n){
                current = p[i].arrival;
                ready_queue.push(i);
                i++;
            }
            else break;
        }

        int f = ready_queue.front();
        ready_queue.pop();
        int exec_start = max(current, p[f].arrival);

        //第一次執行要記錄start
        if (p[f].remaining_time == p[f].burst) {
            p[f].start = exec_start;
        }

        //執行
        int run_time = min(qt, p[f].remaining_time);
        current += run_time;
        p[f].remaining_time -= run_time;

        while(i < n && p[i].arrival <= current){
            ready_queue.push(i);
            i++;
        }

        //若執行結束
        if (p[f].remaining_time == 0) {
            p[f].completion = current;
            p[f].turnaround = p[f].completion - p[f].arrival;
            p[f].waiting = p[f].turnaround - p[f].burst;
            completed++;
        } else {
            ready_queue.push(f);
        }

        g.push_back({p[f].id, exec_start});
        t_comp = p[f].completion;
    }

    return t_comp;
}
int calculate_srtf(vector<process>& p, int n, vector<pair<int, int>>& g){
   sort(p.begin(), p.end(), [](process a, process b){
        if(a.arrival == b.arrival) return a.burst < b.burst;
        return a.arrival < b.arrival;
   });

   int current = p[0].arrival, completed = 0;
   int j = 1, prev = -1;
   vector<int> ready;
   ready.push_back(0);
   while(completed < n){
        int f = ready[0];
        for(int i : ready){
            if(p[i].remaining_time < p[f].remaining_time) f = i;
        }
        if(p[f].remaining_time == p[f].burst) p[f].start = current;
        p[f].remaining_time--;
        if(prev != f){
            g.push_back({p[f].id, current});
            prev = f;
        }
        current++;

        if(p[f].remaining_time == 0){
            p[f].completion = current;
            p[f].turnaround = p[f].completion - p[f].arrival;
            p[f].waiting = p[f].turnaround - p[f].burst;
            completed++;
            ready.erase(remove(ready.begin(), ready.end(), f), ready.end());
        }

        while(j < n && p[j].arrival <= current){
            ready.push_back(j);
            j++;
        }
   }
   return current;
}

//主程式
int main(){
    while(true){
        //選單
        cout << "請選擇排程模式（數字）：" << endl;
        cout << "1. FCFS（非搶佔）" << endl;
        cout << "2. SJF（非搶佔）" << endl;
        cout << "3. Priority（非搶佔）"<< endl;
        cout << "4. Round Robin" << endl;
        cout << "5. SRTF" << endl;
        int choice;
        cin >> choice;

        int n, t_comp = 0;
        vector<process> p = read_input(n, choice);
        vector<pair<int, int>> gantt;

        //進入排程
        if(choice == 1) calculate_fcfs(p, n); //FCFS
        else if(choice == 2) calculate_sjf(p, n); //SJF
        else if(choice == 3) calculate_priority(p, n); //priority
        else if(choice == 4){ //RR
            int quanTum;
            cout << "請輸入Time Quantum：";
            cin >> quanTum;
            t_comp = calculate_rr(p, n, quanTum, gantt); 
        }
        else if(choice == 5) t_comp = calculate_srtf(p, n, gantt); 
        
        double avewait = 0, avetra = 0;
        print_table(p, avewait, avetra, choice);
        print_gantt_chart(p, gantt, choice, t_comp);
        print_ave(avewait, avetra, n);
    }
    return 0;
}