#include <bits/stdc++.h>
using namespace std;

class Employee{
    private:
        long long age;
        long long exp;
        long long empid;
        long long bossID;
        long long depth;
    
    public:
        vector<Employee*> subord = {};
        bool vis;
        long long avg;

        Employee(long long eID, long long a, long long e, long long bID) : 
            age(a), exp(e), empid(eID), bossID(bID), depth(0) {}

        void addSubord(Employee* emp){
            subord.emplace_back(emp);
        }

        long long sum(){
            long long sum = 0;
            for(auto emp : subord){
                sum += emp->exp;
            }
            sum += this->exp;
            return sum;
        }

        // long long youngestSubord(long long k){
        //     if(subord.size() < k) return -1;
            
            
        //     return subord[0]->getID();
        // }

        long long getID(){ return empid; }
        long long getExp(){ return exp; }
        long long getAge(){ return age; }
        long long getBoss(){ return bossID; }

        long long getDepth(){return depth; }
        void setDepth(long long d){ depth = d; }
};

class Tree{
    private:
        Employee* root;
        int count;

    public:
        Tree(){
            root = nullptr;
            count = 0;
        }

        Tree(Employee* founder){
            root = founder;
            root->setDepth(0);   
            count = 1;        
            //cout << root->getID() << endl;
        }

        void insert(unordered_map<long long, vector<Employee*>> &IDmap, unordered_map<long long, Employee*> &eIDmap){
            for(auto pair: IDmap){
                //cout << pair.first << endl;
                // for(auto p : pair.second){ 
                //     cout << "W" << p->getID() << endl;
                //     eIDmap[pair.first]->addSubord(p);
                // }
                sort(pair.second.begin(), pair.second.end(), [](Employee* a, Employee* b){
                    if(a->getAge() == b->getAge()){
                        return a->getID() < b->getID();
                    }
                    return a->getAge() < b->getAge();
                });
                eIDmap[pair.first]->subord = pair.second;
                count += pair.second.size();
                //cout << "working?" << endl;
            }
        }

        void findDepth(){
            queue<Employee*> q;
            long long d = 0;

            q.push(root);
            root->setDepth(d);

            while(!q.empty()){
                long long sz= q.size();
                d++;
                for(long long i = 0; i<sz; i++){
                    Employee* node = q.front();
                    q.pop();
                    for(auto child : node->subord){
                        child->setDepth(d);
                        q.push(child);

                    }
                }
            }
        }

        long long youngestSubord(long long k, Employee* emp){
            long long ans = 0;
            dfsYoung(emp, k, ans);
            return ans;
        }

        void dfsYoung(Employee* curr, long long k, long long &count){
            if(curr == nullptr) return;
            if(count == k) return;

            curr->vis = true;
            for(auto child : curr->subord){
                child->vis = true;
                if(count == k) return;
                if(count < k && child->getAge() != curr->getAge()){
                    count++;
                }
                dfsYoung(child, k, count);
            }
            curr->vis = false;
        }

        long long findAvg(Employee* emp){
            long long count = 0;
            long long sum = 0;
            dfsAvg(emp, count, sum);
            cout << sum << " " << count << endl;
            long long avg = (long long) (sum) / count;
            return avg;
        }

        long long dfsAvg(Employee* curr, long long &count, long long &sum){
            if(curr->subord.empty()){
                cout << curr->sum() << endl;
                return curr->sum();
            }

            curr->vis = true;
            for(auto child : curr->subord){
                child->vis = true;
                sum += (dfsAvg(child, count, sum) * (child->subord.size() + 1));
                cout << sum << endl;
                count += curr->subord.size()+1;
                cout << "H " << count<<endl;
            }
            sum += curr->sum();
            return sum;
        }
};

int main(){
    long long n;
    cin >> n;

    vector<Employee*> emps;
    unordered_map<long long, vector<Employee*>> IDmap;  //bossID to employees map
    unordered_map<long long, Employee*> eIDmap;   //id to employee map

    Tree t;   //company's hirearchy

    //collect employee data:
    for(long long i = 0; i<n; i++){
        long long id, a, e, bid;
        cin >> id >> a >> e >> bid;
        Employee* emp = new Employee(id, a, e, bid);
        emps.push_back(emp);
        eIDmap[id] = emp;
        if(bid == id) {
            t = Tree(emp);
            //cout << "Made root " << endl;
        }
        else IDmap[bid].push_back(emp);

    }


    //create hirerchy:
    //cout << "creating..." << endl;
    t.insert(IDmap, eIDmap);
    //cout << "finding" << endl;
    t.findDepth();
    //cout << "created" << endl;

    //queuries:
    long long q;
    cin >> q;
    while(q--){
        long long type;
        cin >> type;

        if(type == 1){
            long long dtype;
            cin >> dtype;

            if(dtype == 1){
                sort(emps.begin(), emps.end(), [](Employee* a, Employee* b){
                    if(a->getDepth() == b->getDepth()){
                        return a->getExp() > b->getExp();
                    }
                    return a->getDepth() < b->getDepth();
                });

                long long count = 0;
                
                for(auto x : emps){
                    if(x->getDepth() == emps[0]->getDepth()){
                        if(x->getExp() == emps[0]->getExp()){
                            count++;
                        }
                        break;
                    }
                    break;
                }

                cout << count << endl;
            }


            if(dtype == 2){
                sort(emps.begin(), emps.end(), [](Employee* a, Employee* b){
                    if(a->getDepth() == b->getDepth()){
                        return a->getAge() > b->getAge();
                    }
                    return a->getDepth() < b->getDepth();
                });

                long long count = 0;
                
                for(auto x : emps){
                    if(x->getDepth() == emps[0]->getDepth()){
                        if(x->getAge() == emps[0]->getAge()){
                            count++;
                        }
                        break;
                    }
                    break;
                }

                cout << count << endl;
            }

            if(dtype == 3){
                sort(emps.begin(), emps.end(), [](Employee* a, Employee* b){
                    if(a->getExp() == b->getExp()){
                        return a->getAge() < b->getAge();
                    }
                    return a->getExp() > b->getExp();
                });

                long long count = 0;
                
                for(auto x : emps){
                    if(x->getExp() == emps[0]->getExp()){
                        if(x->getAge() == emps[0]->getAge()){
                            count++;
                        }
                        break;
                    }
                    break;
                }

                cout << count << endl;
            }

        }

        else if(type == 2){
            long long eid;
            cin >> eid;
            cout << t.findAvg(eIDmap[eid]) << endl;
        }

        else if(type == 3){
            long long eid, k;
            cin >> eid >> k;
            cout << t.youngestSubord(k, eIDmap[eid]) << endl;
        }
    }

    return 0;
}


