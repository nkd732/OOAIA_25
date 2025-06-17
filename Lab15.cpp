#include <bits/stdc++.h>
using namespace std;


class Event{
    protected:
        int startTime;
        int endTime;

    public:
        Event() {} 
        virtual double calculateProfit() = 0;

        int getStart(){ return startTime;}
        int getEnd(){ return endTime; }
};


class Concert : public Event{
    private:
        double price; //ticket price
        double sold; //tickets sold
        double fee; //artist fee
        double logCost; //Logistic cost

    public:
        Concert(int start, int end, double p, double s, double f, double c)
            : price(p), sold(s), fee(f), logCost(c) {
            startTime = start;  
            endTime = end;      
        }


        double calculateProfit(){
            double rev = 0.82 * (price * sold);  //ticket revenue
            double cost = fee + logCost;
            double profit = rev - cost;

            if(profit > 2*cost){
                return 0.7 * profit;
            }
            return profit;
        }

};

class TheaterShow : public Event{
    private:
        double basePrice;
        double seats;
        double venueCost;
    
        public:
            TheaterShow(int start, int end, double bp, double s, double vc) 
                : basePrice(bp), seats(s), venueCost(vc) {
                startTime = start;  
                endTime = end;
            }

            double calculateProfit(){
                double revenue = ( (1.25) * (seats) * (basePrice) ) * (0.82);  //totla revenue
                double popRev = 0.25 * seats * 150;  //popcorn Revenue

                double netProfit = revenue + popRev - venueCost;
                return netProfit;
            }
};

class Wedding : public Event{
    private:
        int amt;  //amount
        int decCost;  //decoration cost
        int count;    //guest count
        int venCost;  //wedding venue cost

    public:
        Wedding(int start, int end, double a, double dc, double c, double vc)
            : amt(a), decCost(dc), count(c), venCost(vc) {
            startTime = start;  
            endTime = end;
        }

        double calculateProfit(){
            int totVenCost = (count > 200) ? venCost*3 : venCost;  //total venue cost
            int cateCost = (count > 100) ? count*70 : count*100;  //catering cost

            double netProfit = amt - totVenCost - decCost - cateCost;
            return netProfit;
        }
};

bool Comparator(Event* a, Event* b){
    if(a->getEnd() == b->getEnd()){
        return a->getStart() < b->getStart();
    }
    return a->getEnd() < b->getEnd();
}


class EventScheduler{
    private:
        vector<Event*> events;
        int n;
        vector<int> pj; //highest index j such that event j compatible with event i
        vector<double> dp;

    public:
        EventScheduler() {}

        void addEvent(Event* event){
            events.push_back(event);
        }

        void helper(){
            n = events.size();

            //sort based on start time:
            sort(events.begin(), events.end(), Comparator);

            //calculate p(j) vector:
            pj.resize(n, -1);
            for(int i = 0; i < n; i++){
                for(int j = 0; j < i; j++){
                    if(events[j]->getEnd() <= events[i]->getStart()){
                        pj[i] = j;
                    }
                }
            }

            //intialize dp
            dp.resize(n);
            dp[0] = events[0]->calculateProfit();
        }

        double maxProfit(){
            helper();
            
            for(int i = 1; i < n; i++){
                if(pj[i] != -1) dp[i] = max(dp[i-1], dp[pj[i]] + events[i]->calculateProfit());
                else{
                    dp[i] = max(dp[i-1], events[i]->calculateProfit());  //in case of no event before it is compatible
                }
            }

            return dp[n-1];
        }
};


int main(){
    int n;
    cin >> n;
    EventScheduler scheduler;

    while(n--){
        int type;
        cin >> type;
        int start, end;
        cin >> start >> end;

        Event* event  = nullptr;

        if(type == 1){
            double p, s, f, c;
            cin >> p >> s >> f >> c;
            event = new Concert(start, end, p, s, f, c);
        }
        
        else if (type == 2){
            double bp, s, vc;
            cin >> bp >> s >> vc;
            event = new TheaterShow(start, end, bp, s, vc);
        }

        else if(type == 3){
            double a, dc, c, vc;
            cin >> a >> dc >> c >> vc;
            event = new Wedding(start, end, a, dc, c, vc);
        }

        scheduler.addEvent(event);
    }


    cout << fixed << setprecision(2) << scheduler.maxProfit() << "\n";

    return 0;
}


