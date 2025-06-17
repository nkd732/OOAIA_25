
#include <bits/stdc++.h>
using namespace std;
class Faculty{
private:
string name;
bool isPerm;
int btpguidees;
public:
Faculty(){
btpguidees = 0;
}
Faculty(string name, bool perm){
this->name = name;
isPerm = perm;
btpguidees = 0;
}
string getName(){
return this->name;
}
bool getPerm(){
return this->isPerm;
}
void setPerm(bool perm){
isPerm = perm;
}
};
class Stu{
protected:
string rollno, hostel;
float cgpa;
Faculty facad;
Stu(string rollno, string hostel, float cgpa){
this->rollno = rollno;
this->hostel = hostel;
this->cgpa = cgpa;
}
void setFaculty(Faculty facad){
this->facad = facad;
}
string getHostel(){
return this->hostel;
}
Faculty getfacad(){
return this->facad;
}
void changeHostel(string newHostel){
hostel = newHostel;
}
friend class Faculty;
};
class BTech : public Stu{
Faculty* btpGuide;
BTech(string rollno, string hostel, float cgpa){
btpGuide = nullptr;
}
Faculty getBtpGuide(){
return *(this->btpGuide);
}
void setbtpGuide(Faculty facad){
this->btpGuide = &facad;
}
};
class DualDegree : public Stu{
Faculty* ddpGuide;
Faculty* taSupervisor;
DualDegree(string rollno, string hostel, float cgpa){
ddpGuide = nullptr;
taSuperVisor = nullptr;
}
Faculty getddpGuide(){
return *(this->ddpGuide);
}
void setddpGuide(Faculty facad){
this->ddpGuide = &facad;
}
Faculty getTASupervisor(){
return *(this->taSuperVisor);
}
void setTASupervisor(Faculty facad){
this->taSuperVisor = &facad;
}
};
class IITM{
vector<Student*> students;
vector<Faculty*> faculty;
friend class Stu;
friend class BTech;
friend class Faculty;
friend class DualDegree;
};
// int main(){
// Faculty* f1 = new Faculty(F1, true);
// Faculty* f2 = new Faculty(F2, false);
// Faculty* f3 = new Faculty(F3, true);
// Student* sb1 = new BTech(b001, h1, 8.4);
// Student* sb2 = new BTech(b002, h1, 8.3);
// Student* sd1 = new DualDegree(d001, h2, 8.7);
// return 0;
// }
