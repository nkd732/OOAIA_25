#include <bits/stdc++.h>
using namespace std;
class Book{
private:
string isbn;
int copies;
int totCop;
public:
string title;
string author;
Book(string title = "UnknownTitle", string author = "UnknownAuthor", string isbn = "ISBN",
int copies = 0, int totCop = 5){
this->title = title;
this->author = author;
this->isbn = isbn;
this->copies = copies;
this->totCop = totCop;
}
Book(const Book& other, string newIsbn){
this->title = other.title;
this->author = other.author;
this->isbn = newIsbn;
this->copies = other.copies;
this->totCop = other.totCop;
}
string getIsbn(){ return isbn; }
int getCopies(){ return copies; }
int getTotal(){ return totCop; }
void updateCopies(int count){
if(copies + count < 0 || totCop + count < 0){
cout << "Invalid request! Count becomes negative" << endl;
}
else{
copies += count;
totCop += count;
}
}
bool borrowBook(){
if(copies > 0){
copies--;
return true;
}
else{
cout << "Invalid request! Copy of book not available" << endl;
return false;
}
}
bool returnBook(){
if(copies < totCop){
copies++;
return true;
}
else{
cout << "Invalid request! Copy of book exceeds total copies" << endl;
return false;
}
}
void printDetails(){
cout << title << " " << author << endl;
}
};
class Member{
private:
string memberID;
int borrowLimit;
map<string, int> borrowedBooks;
public:
string name;
Member(string memberID, string name, int borrowLimit = 3){
this->memberID = memberID;
this->name = name;
this->borrowLimit = borrowLimit;
}
string getID(){ return memberID; }
int getLimit(){ return borrowLimit; }
bool borrowBook(string isbn){
if(borrowLimit > 0){
borrowedBooks[isbn]++;
borrowLimit--;
return true;
}
cout << "Invalid request! Borrow limit exceeded" << endl;
return false;
}
bool returnBook(string isbn){
if(borrowedBooks.find(isbn) != borrowedBooks.end() && borrowedBooks[isbn] > 0){
borrowedBooks[isbn]--;
if(borrowedBooks[isbn] == 0){
borrowedBooks.erase(isbn);
}
borrowLimit++;
return true;
}
cout << "Invalid request! Book not borrowed" << endl;
return false;
}
void printDetails(){
for(auto book : borrowedBooks){
cout<< memberID << " " << name << " " << book.first << " " << book.second << endl;
}
}
};
class Library{
private:
vector<Book*> books;
vector<Member*> members;
public:
unordered_map<string, Book*> bookMap;
unordered_map<string, Member*> membMap;
~Library() {
for (auto book : books) delete book;
for (auto member : members) delete member;
}
bool addBook(Book& book){
if(bookMap.find(book.getIsbn()) == bookMap.end()){
Book* bookptr = &book;
books.push_back(bookptr);
bookMap[bookptr->getIsbn()] = bookptr;
return true;
}
cout << "Invalid request! Book with same isbn already exists" << endl;
return false;
}
bool registerMember(Member& member){
if(membMap.find(member.getID()) == membMap.end()){
Member* membptr = &member;
members.push_back(membptr);
membMap[membptr->getID()] = membptr;
return true;
}
cout << "Invalid request! Member with same id already exists" << endl;
return false;
}
bool borrowBook(string id, string isbn){
if(bookMap.find(isbn) == bookMap.end()){
cout << "Invalid request! Book does not exist" << endl;
return false;
}
if(membMap.find(id) == membMap.end()){
cout << "Invalid request! Member does not exist" << endl;
return false;
}
if(bookMap[isbn]->getCopies() > 0){
if(membMap[id]->getLimit() > 0){
membMap[id]->borrowBook(isbn);
return bookMap[isbn]->borrowBook();
}
else return membMap[id]->borrowBook(isbn);
}
return bookMap[isbn]->borrowBook();
}
bool returnBook(string memberID, string isbn){
if(bookMap.find(isbn) == bookMap.end()){
cout << "Invalid request! Book does not exist" << endl;
return false;
}
if(membMap.find(memberID) == membMap.end()){
cout << "Invalid request! Member does not exist" << endl;
return false;
}
if(bookMap[isbn]->getCopies() < bookMap[isbn]->getTotal()){
if(membMap[memberID]->returnBook(isbn)){
return bookMap[isbn]->returnBook();
}
else return false;
}
return bookMap[isbn]->returnBook();
}
void printLibraryDetails(){
for(auto book : books){
cout << book->title << " " << book->author << " " << book->getCopies() << endl;
}
for(auto memb : members){
cout << memb->getID() << " " << memb->name << endl;
}
}
};
int main(){
string command;
Library Lib;
while(cin >> command && command != "Done"){
if(command == "Book"){
string type;
cin >> type;
if(type == "None"){
Book* book = new Book();
Lib.addBook(*book);
}
else if(type == "ExistingBook"){
string oldIsbn, newIsbn;
cin >> oldIsbn >> newIsbn;
if(Lib.bookMap.find(oldIsbn) != Lib.bookMap.end()){
Book* book = new Book(*Lib.bookMap[oldIsbn], newIsbn);
Lib.addBook(*book);
}
else{
cout << "Invalid request! Old ISBN not found" << endl;
}
}
else{
string author, isbn;
int cop, tot;
cin >> author >> isbn >> cop >> tot;
Book* book = new Book(type, author, isbn, cop, tot);
Lib.addBook(*book);
}
}
else if(command == "UpdateCopiesCount"){
string isbn;
int newCount;
cin >> isbn >> newCount;
if(Lib.bookMap.find(isbn) != Lib.bookMap.end()){
Lib.bookMap[isbn]->updateCopies(newCount);
}
else{
cout << "Invalid request! Book does not exist" << endl;
}
}
else if(command == "Member"){
string type;
cin >> type;
string id, name;
int limit;
if(type == "NoBorrowLimit"){
cin >> id >> name;
Member* member = new Member(id, name);
Lib.registerMember(*member);
}
else{
cin >> name >> limit;
Member* member = new Member(type, name, limit);
Lib.registerMember(*member);
}
}
else if(command == "Borrow"){
string id, isbn;
cin >> id >> isbn;
Lib.borrowBook(id, isbn);
}
else if(command == "Return"){
string id, isbn;
cin >> id >> isbn;
Lib.returnBook(id, isbn);
}
else if(command == "PrintBook"){
string isbn;
cin >> isbn;
if(Lib.bookMap.find(isbn) != Lib.bookMap.end()){
Lib.bookMap[isbn]->printDetails();
}
else{
cout << "Invalid request! Book does not exist" << endl;
}
}
else if(command == "PrintMember"){
string id;
cin >> id;
if(Lib.membMap.find(id) != Lib.membMap.end()){
Lib.membMap[id]->printDetails();
}
else{
cout << "Invalid request! Member does not exist" << endl;
}
}
else if(command == "PrintLibrary"){
Lib.printLibraryDetails();
}
}
return 0;
}
