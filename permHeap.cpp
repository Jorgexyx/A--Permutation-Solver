/*
Authors : Jorge Bautista & Ryan Boyle
*/
#include <time.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <unistd.h>

class Node 
{
	public: 
	  Node(int *perm,int i, int j) { 
		  permutation = NULL;
		  permutation = perm;
		  parent = NULL;
		  x = i; 
		  y = j;
	  }
	  Node() {}
	  void storeGVal(int val)   { gVal = val; }
	  void storeFVal(int val)   { fVal = val; }
          void storeParent(Node *par) {parent = par;}

	  int *getPermutation() { return permutation; }
	  int getX() { return x;}
	  int getY() { return y; }
	  int getGVal()   { return gVal; }
	  Node *getParent() {return parent; }
	  double getFVal() { return fVal; }
	private:
	  Node *parent;
	  int *permutation;
	  int x;
	  int y;
	  int fVal;
	  int gVal;
}; 

void swap(Node *, Node *);

class Heap
{
  std::vector<Node *> pointers;
  int size;

  public:
    Heap();
    int parent(int i) { return (i-1) / 2;}
    int left(int i) { return ( 2* i + 1); }
    int right(int i) { return ( 2* i + 2); }
    Node *popMin();
    void Print();

    void insertKey(Node *);
    void heapify(int );
    int getSize() { return size; }
};

Heap::Heap()
{
  size = 0;
}

void Heap::Print() {
  std::cout << "printing tree: \n";
  for(int i = 0 ; i < pointers.size(); i++)
  {
    Node *nOne = pointers.at(i);
    std::cout << nOne->getFVal() << "-> ";
  }
  std::cout << "\n";
}

void Heap::insertKey(Node *node)
{
  pointers.push_back(node);
  int i = size;
  size ++;
   
  Node *nOne = pointers.at(parent(i));
  Node *nTwo= pointers.at(i);
  double fOne = nOne->getFVal();
  double fTwo = nTwo->getFVal();
  
  while(i != 0 && fOne > fTwo) 
  {
    swap(pointers.at(parent(i)), pointers.at(i)); 
    i = parent(i);
    nOne = pointers.at(parent(i));
    nTwo = pointers.at(i);

    fOne = nOne->getFVal();
    fTwo = nTwo->getFVal();
  }
}

Node *Heap::popMin()
{
  Node *node = pointers.at(0);
  size --;
  int loc = pointers.size() - 1;
  if(size + 1 == 1)
  {
    pointers.erase(pointers.end() - 1);
    return node;
  }
  pointers.at(0) = pointers.at(pointers.size() - 1);
  pointers.erase(pointers.end() - 1);
  heapify(0);
  return node;
}

void Heap::heapify(int i)
{
  int l = left(i);
  int r = right(i);

  int smallest = i;

  Node *currNode = pointers.at(smallest);
  double parF = currNode->getFVal();

  if ( l < pointers.size()){
    Node *nOne = pointers.at(l);
    double fOne = nOne->getFVal();
      if( fOne < parF) {
      smallest = l;
      }
  }

  currNode = pointers.at(smallest);
  parF = currNode->getFVal();

  if ( r < pointers.size() ) {
    Node *nTwo= pointers.at(r);
    double fTwo = nTwo->getFVal();

      if( fTwo < parF) {
  	smallest = r;
      }
  }

  if(smallest != i)
  { 
    Node *n1 = pointers.at(i);
    Node *n2 = pointers.at(smallest);

    swap(pointers.at(i),pointers.at(smallest));
    heapify(smallest);
  }

}

void swap(Node *n1, Node *n2)
{
  Node temp = *n1;
  *n1 = *n2;
  *n2 = temp;
}

std::vector<int> getInput();
void aStar(int *, int const );
void printPath(Node *par, int const size);
void printArray(int *arr, int const size);
bool generateChilds(Heap &, int const size);
bool inOrder(int *, int const size);
int *flip(int *,int, int, int const size);
int convert(std::string x);
double hVal(int *perm,int const size);
int numPopped = 0;

int main()
{ 
  std::vector<int> vals = getInput();
  const int size = vals.size();
  int vals2[size];
  for (int i = 0; i <size; i++)
      vals2[i] = vals[i];
  if(inOrder(vals2, size)) { 
    std::cout << "given permutation is already in order. \n"; 
    return 0; }

  std::cout << "\n Runnning A* .. \n";
  aStar(vals2,size);
  return 0;
}
void aStar(int *perm, int const size){

  Heap heap;

  Node *node = new Node(perm,-1,-1);
  node->storeGVal(0);
  node->storeFVal(0);

  heap.insertKey(node);
  
  double cpu0 = clock();
  int currCost = 0;

  
  while(heap.getSize() != 0) {
  if(generateChilds(heap, size))
  {
    double cpu1 = clock();
    std::cout << "Total cpu time for A*: " << (cpu1 - cpu0) /CLOCKS_PER_SEC << "seconds \n";
    std::cout << "Total number of states popped: " << numPopped << std::endl;
    return ;
  }
  } 
  std::cout << "Failed \n";
}


bool generateChilds(Heap &heap, int const size)
{
  Node *node = new Node();
  node = heap.popMin();
  numPopped ++;
  int x = node->getX();
  int y = node->getY();
  int *perm = node->getPermutation();
  int currCost = node->getGVal();

  for(int i = 1 ; i < size; i++) {
    for(int j = 0 ; j < size ; j ++) {	
      if( (j + i ) > size - 1) 
	break;

        if(x != j && y!=(i+j)) { 
      	  int *newPerm = flip(perm, i + j, j, size);
	  int newCost = currCost + 1;
	  double h = hVal(newPerm,size);
	  int fVal = h + newCost;

	  Node *leaf = new Node(newPerm,j,i+j); 
	  leaf->storeFVal(fVal);
	  leaf->storeGVal(newCost);
	  leaf->storeParent(node);

	  if(inOrder(newPerm, size)) { 

	    printPath(leaf->getParent(),size);
	    printArray(newPerm,size);
	    std::cout << "\n";
            return true;
	  }
	heap.insertKey(leaf);
      }
    }
  }
  return false;
}

double hVal(int *perm,int const size)
{
  double val = 0;
  for(int i = 0; i < size - 1; i ++){
    if(std::abs(perm[i] - perm[i + 1]) > 1)
    {
      val += 1;
    }
  }
  return val * .5;
}

int *flip(int *permutation, int endIdx,int startingIdx, int const size)
{
  int *tempArr = (int *)malloc(sizeof(int)* size); 
  for(int i = 0; i < size; i ++) 
    tempArr[i] = permutation[i]; 
	
  int tempVal = 0;
  while(startingIdx < endIdx) {	
    tempVal = tempArr[startingIdx];
    tempArr[startingIdx] = tempArr[endIdx]; 
    tempArr[endIdx] = tempVal;
    endIdx --;
    startingIdx ++;
   }	
   return tempArr;
}

bool inOrder(int *arr, int const size)
{
  for(int i = 0; i < size - 1; i++)
    if(arr[i] > arr[i + 1])
      return false;
  return true;
}


void printArray(int *arr, int const size)
{
  for(int i = 0; i < size; i ++)
    std::cout << arr[i] << ",";
}

void printPath(Node *parr, int const size)
{
 std::vector <int *> reverse;
 while(parr != NULL)
 {
    reverse.push_back(parr->getPermutation());
    parr = parr->getParent();
 }
 for(int i = reverse.size() - 1; i > -1; i--)
 {
   printArray(reverse.at(i),size);
   std::cout << " -> ";
 }

 return ;
}

int convert(std::string x) { return atoi(x.c_str()); }

std::vector<int> getInput() { 
  std::string xin;
  std::cout << "please enter values to sort: "; 
  std::getline(std::cin,xin);
  std::stringstream stream(xin);
  std::vector<int> vals;
  for (int i = 0; i < xin.length(); i++){
        std::string token;
        while(isdigit(xin[i])){
            char x = xin[i];
            token += x;
            i++;
        }
      int new_int = convert(token);
      vals.push_back(new_int);
  }
  return vals;
}
