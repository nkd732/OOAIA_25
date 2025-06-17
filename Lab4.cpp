#include <bits/stdc++.h>
using namespace std;

// Quantum Nanotech Suit
class QNS{
private:
    int power, durability, energy, heat;

public:
    QNS(int p, int d, int e, int h) : power(min(p, 5000)), durability(d), energy(e), heat(h) {}

    QNS(const QNS& other): power(other.power), durability(other.durability), energy(other.energy), heat(other.heat) {}  //copy constructor
    

    QNS operator+(QNS other){        //suit upgradation
        return QNS(min(power + other.energy, 5000), durability + other.durability, energy + other.power, heat);
    }

    QNS operator-(int damage){      //damage from an attack
        int newDurability = durability - damage;
        int newHeat = heat + damage;
        return QNS(power, newDurability, energy + damage, newHeat);
    }

    QNS operator*(int x){          //Power Boost mode
        int newPower = min(power + (power * x) / 100, 5000);
        int newHeat = heat + x;
        return QNS(newPower, durability, energy + (5 * x), newHeat);
    }

    QNS operator/(int x){          //cooling
        int newHeat = max(heat - x, 0);
        return QNS(power, durability + x, energy, newHeat);
    }

    void boostPower(int factor){     //Boosts(add) power
        *this = (*this) * factor;
    }

    void boostPower(QNS& extraSuit){  //Transfers energy from another suit
        *this = (*this) + extraSuit;
    }

    bool isDestroyed(){ return durability <= 0; }   //checks durability
    bool isOverheated(){ return heat > 500; }       //checks heat level


    bool operator==(QNS& other){       //Compares suits 
        return (power == other.power && durability == other.durability);
    }

    bool operator<(QNS& other){       // Compares suit effectiveness in battles
        return (power + durability) < (other.power + other.durability);
    }

    void print(){
        cout << power << " " << durability << " " << energy << " " << heat << endl;
    }

    //getters
    int getpower(){ return power; }
    int getDurability(){ return durability; }
    int getheat(){ return heat; }

    //setters
    void setpower(int p){ power = p; }
    void setDurability(int d){ durability = d; }
    void setheat(int h){ heat = h; }
};

// Avenger Class
class Avenger {
private:
    string name;
    QNS suit;
    int attackStrength;

public:
    Avenger(string avName, QNS avSuit, int strength) : name(avName), suit(avSuit), attackStrength(strength) {}  //constructor

    void attack(Avenger& enemy){  //Damages the enemy durability
        if(!suit.isDestroyed() && !suit.isOverheated() && !enemy.suit.isDestroyed()){
            // cout << "Before Attack: " << enemy.getName() << " -> ";
            // enemy.getSuit().print();  // Print before attack
    
            QNS newSuit = enemy.getSuit() - attackStrength; 
            enemy.setSuit(newSuit);  // Update suit
    
            // cout << "After Attack: " << enemy.getName() << " -> ";
            // enemy.getSuit().print();  // Print after attack
        }
    }

    void repair(int x){  //Restores some durability to suit
        QNS newSuit = suit / x;
        setSuit(newSuit);
    }

    void boostSuit(int factor){  //to boost suit power of avenger
        suit.boostPower(factor);
    }

    void upgrade(QNS& extraSuit){  //to upgrade suit based on extra avalible suit
        suit.boostPower(extraSuit);
    }
    
    void printStatus(){
        cout << name << " ";
        suit.print();
    }

    bool isAlive() { return !suit.isDestroyed(); } //checks if avenger is in battle (is suit not dstroyed)
    string getName(){ return name; }
    QNS getSuit() { return suit; }
    void setSuit(QNS& newSuit) {suit = newSuit;}
    
};


// Battle Class
class Battle{
    private:
        queue<QNS> available;  //avalible suits in queue (first come first serve)
        vector<Avenger*> heroes, enemies;
        unordered_map<string, Avenger*> avengerMap;  //all the avengers (heroes + enemies)
        vector<string> battleLog;
    
    public:
    
        void startBattle(){
            int k, n, m;
            cin >> k >> n >> m;
    
            for(int i = 0; i < k; i++){
                int p, d, e, h;
                cin >> p >> d >> e >> h;
                available.push(QNS(p, d, e, h));  //push suits into queue
            }
    
            for(int i = 0; i < n + m; i++){
                string name;
                int attack;
                cin >> name >> attack;
    
                if(!available.empty()){
                    Avenger* avenger = new Avenger(name, available.front(), attack);  //assign suits to avengers
                    available.pop(); 
                    if(i < n){
                        heroes.emplace_back(avenger);  //place them into heroes and enemies based on i
                    }
                    else{
                        enemies.emplace_back(avenger);
                    } 
                    avengerMap[name] = avenger;  //place them into unordered_map
                } 
                else{
                    cout << name << " is out of fight" << endl;  //if no suit is avalible, avenger doesnt go into battle
                }
            }
        }
    
        void attack(string aName, string tName){
            Avenger* attacker = nullptr;
            Avenger* target = nullptr;
    
            for(auto av : avengerMap){  //go thru map, and assign the pointers to the correct avengers
                if (av.first == aName) attacker = av.second;
                if (av.first == tName) target = av.second;    //attacker and target 
            }
                
            //check if attacker and target are there in map, both are alive and if attacker is not over heated
            if(attacker && target && attacker->isAlive() && !attacker->getSuit().isOverheated() && target->isAlive()){

                attacker->attack(*target);
                battleLog.push_back(aName + " attacks " + tName);
    
                if(!target->isAlive()) battleLog.push_back(tName + " suit destroyed");
                else if(target->getSuit().isOverheated()) battleLog.push_back(tName + " suit overheated");
            }
            else{
                cout << "incorrect avenger" << endl;
            }
        }

        void boostSuit(string avengerName, int factor){  //boost suit
            auto it = avengerMap.find(avengerName);
            if(it != avengerMap.end() && it->second->isAlive()){
                Avenger* hero = it->second;
                hero->boostSuit(factor);
                battleLog.push_back(avengerName + " boosted");
            
                if(hero->getSuit().isOverheated()){
                    battleLog.push_back(avengerName + " suit overheated");
                }
            }
        }
    
        void repair(string avengerName, int x){ //repair suit
            auto it = avengerMap.find(avengerName);
            if(it != avengerMap.end() && it->second->isAlive()){
                Avenger* hero = it->second;
                hero->repair(x);
                battleLog.push_back(avengerName + " repaired");
            }
        }
    
        void upgradeSuit(string avengerName){   //upgrade Suit
            auto it = avengerMap.find(avengerName);
            if(it != avengerMap.end() && it->second->isAlive()) {
                Avenger* hero = it->second;

                // Check if extra suits are available for upgrade means queue is still not empty
                if(available.empty()){
                    battleLog.push_back(avengerName + " upgrade Fail");
                    return;
                }

                hero->upgrade(available.front());
                available.pop(); //remove the suit from queue
                battleLog.push_back(avengerName + " upgraded");
            }
        }
    
    
        void BattleLog(){  
            for(auto& s : battleLog) cout << s << endl;  //print battle logs
        }
    
        int Result(){
            int heroScore = 0, enemyScore = 0;
        
            // Calculate score of heros
            for(auto& hero : heroes){
                if(hero->isAlive()){  //check if suit is not destroyed
                    heroScore += hero->getSuit().getpower() + hero->getSuit().getDurability();
                }
            }
        
            // Calculate score of enemies
            for(auto& enemy : enemies){
                if(enemy->isAlive()){  
                    enemyScore += enemy->getSuit().getpower() + enemy->getSuit().getDurability();
                }
            }
        
            // The result:
            if(heroScore > enemyScore) return 1;
            if(enemyScore > heroScore) return -1;
            return 0;
        }
        
        //getters for vector and unordered_map
        vector<Avenger*>& getHeroes(){ return heroes; }
        vector<Avenger*>& getEnemies(){ return enemies; }
        unordered_map<string, Avenger*>& getMap(){ return avengerMap; }
    
};
    

int main(){
    Battle battle;
    battle.startBattle(); // Initialize the input

    string command;
    while(cin >> command && command != "End"){
        if(command == "Attack"){   //attack
            string attacker, target;
            cin >> attacker >> target;
            battle.attack(attacker, target);
        }

        else if(command == "Repair"){  //repair suit
            string avName;
            int x;
            cin >> avName >> x;
            battle.repair(avName, x);
        }

        else if(command == "BoostPowerByFactor"){  //boost Power
            string avName;
            int x;
            cin >> avName >> x;
            battle.boostSuit(avName, x);
        }

        else if(command == "BoostPower"){
            string avName;
            int p, d, e, h;
            cin >> avName >> p >> d >> e >> h;
            QNS newSuit(p, d, e, h);
            if(battle.getMap().find(avName) != battle.getMap().end()){
                Avenger* av = battle.getMap().at(avName);
                av->upgrade(newSuit);
            }
        }

        else if(command == "AvengerStatus"){
            //cout << "Processing status.." << endl;
            string avName;
            cin >> avName;
            if(battle.getMap().find(avName)!= battle.getMap().end()){  //find avenger in map
                Avenger* it = battle.getMap().at(avName);
                it->printStatus();
            } 
            else{
                cout << avName << " not found" << endl;
            }   
        }

        else if(command == "Upgrade"){
            string avName;
            cin >> avName;
            battle.upgradeSuit(avName);
        }

        else if(command == "PrintBattleLog"){
            battle.BattleLog();
        }

        else if(command == "BattleStatus"){
            int result = battle.Result();
            if (result == 1) cout << "heroes are winning" << endl;
            else if (result == -1) cout << "enemies are winning" << endl;
            else cout << "tie" << endl;
        }
        else if(command == "End") break;
    }

    return 0;
}
