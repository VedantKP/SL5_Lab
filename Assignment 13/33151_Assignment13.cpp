/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - TSP-LCBB
*/

#include<iostream>
#include<vector>
#include<limits.h>
#include<algorithm>
#include<bits/stdc++.h>
#include<iomanip>
using namespace std;

int upper = INT_MAX;
vector<int> finalPath; //vector to maintain node numbers in path

class node
{
	public :
	 	int level; //specifies level of node
	 	int* visited; //specifies visited status
		int cost;
		int** mat; //reduction matrix for every node
		int vertex;
		int n;
		vector<int> path;	
			
		//allocated memory to visited and matrix
		node(int n)
		{
			visited = new int[n];						
			mat = new int* [n];
			for(int i=0;i<n;i++)
			{
				mat[i] = new int[n];
				visited[i] = 0; 
			}
			this->n = n;
			cost = 0;
		}
		
		//copies contents from one node to other for constructing path
		node(node* oldObj)
		{
			this->n = oldObj->n;
			
			visited = new int[n];						
			mat = new int* [n];
			for(int i=0;i<n;i++)
			{
				mat[i] = new int[n]; 
			}
			
			for(int i = 0;i < n;i++)
			{
				for(int j=0;j < n;j++)
				{
					this->mat[i][j] = oldObj->mat[i][j];
				}
				this->visited[i] = oldObj->visited[i];
			}
			
			for(int i=0;i<oldObj->path.size();i++)
			{
				path.push_back(oldObj->path.at(i));
			}
		} 
};

//Comparator function for comparing reduction cost of every node 
bool myfunction(node i ,node j)
{
	return i.cost<j.cost;
}

//Identifies minimum from each row and subtracts from each element
int reduceRowMin(int** mat,node* Node,int index,int n)
{
	int min = INT_MAX;
	for(int j=0;j<n;j++)
	{
		if(mat[index][j] != INT_MAX)
		{
			if(min>mat[index][j])
			{
				min = mat[index][j];
			}
		}	
	}
	
	if(min != INT_MAX)
	{
		for(int j=0;j<n;j++)
		{
			if(mat[index][j] != INT_MAX)
			{
				Node->mat[index][j] = mat[index][j] - min;
			}
			else
				Node->mat[index][j] = INT_MAX;	 		
		}
		return min;
	}
	return 0;	
}

//Identifies minimum from each column and subtracts from each element
int reduceColumnMin(int** mat,node* Node,int index,int n)
{
	int min = INT_MAX;
	for(int j=0;j<n;j++)
	{
		if(mat[j][index] != INT_MAX)
		{
			if(min>mat[j][index])
			{
				min = mat[j][index];
			}
		}	
	}
	
	if(min != INT_MAX)
	{
		for(int j=0;j<n;j++)
		{
			if(mat[j][index] != INT_MAX)
			{
				Node->mat[j][index] = mat[j][index] - min;
			}	 		
		}
		return min;
	}
	return 0;		
}

//Reduces matrix by rows and then by columns
void reduceMatrix(int** mat,node* Node,int n)
{
	int reductionValue = 0,min;
	for(int i = 0;i<n;i++)
	{
		min = reduceRowMin(mat,Node,i,n);
		reductionValue += min;
	}
	
	for(int i = 0;i<n;i++)
	{
		min = reduceColumnMin(Node->mat,Node,i,n);
		reductionValue += min;
	}
	Node->cost += reductionValue;
}

//Calculates minimum distance from current node to the next node and stores in vector
int tsp(node* liveNode,int n)
{
	if(liveNode->cost > upper)
	{
		return INT_MAX;
	}
	else
		upper = INT_MAX;
	
	if(liveNode->level == n)
	{
		finalPath.clear();
		for(int i = 0;i<liveNode->path.size();i++)
		{
			finalPath.push_back(liveNode->path.at(i));
		}
		upper = liveNode->cost;
		return liveNode->cost;
	}
		
	vector<node> curNodes;
	int src = liveNode->vertex;
	node* newNode = NULL;
	int check;
	int min = INT_MAX;
	for(int i=0;i<n;i++)
	{
		if(liveNode->visited[i] == 0 && liveNode->mat[src][i] != INT_MAX)
		{
			newNode = new node(liveNode);
			newNode->level = liveNode->level+1;
			newNode->vertex = i;
			newNode->visited[i] = 1;
			newNode->cost = liveNode->cost;
			newNode->cost += liveNode->mat[src][i];
			newNode->path.push_back(i);
			for(int j=0;j<n;j++)
			{
				newNode->mat[src][j] = INT_MAX;
				newNode->mat[j][i] = INT_MAX;
			}
			newNode->mat[i][src] = INT_MAX;
			newNode->mat[i][0] = INT_MAX;
			reduceMatrix(newNode->mat,newNode,newNode->n);
			
			curNodes.push_back(*newNode);		
		}
	}
	sort(curNodes.begin(),curNodes.end(),myfunction);
	vector<node> :: iterator itr = curNodes.begin();
	for(;itr<curNodes.end();itr++)
	{
		check = tsp(&(*itr),n);
		if(check<min)
			min = check;		
	}
	return min;
}

int main()
{
	int n;
	int** adjMat;
	cout<<"\nEnter number of nodes => "; //Input number of nodes
	cin>>n;
	cout<<"\nEnter adjacency matrix:- \n"; 
	
	vector<int> path;
	
	//Dynamically allocate memory to matrix
	adjMat = new int* [n];
	for(int i=0;i<n;i++)
	{
		adjMat[i] = new int[n];
	}
	
	//Input adjacency matrix
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<"\nElement ["<<i<<"]["<<j<<"] => ";
			cin>>adjMat[i][j];
			if(adjMat[i][j] == 0) //Set distance to infinity for the same vertex
			{
				adjMat[i][j] = INT_MAX;
			}
		}
	}
	//Print adjacency matrix
	cout<<"\nAdjacency Matrix is :- \n";	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<setw(15)<<left<<adjMat[i][j]<<"  ";
		}
		cout<<"\n";
	}
	
	node newNode(n); //Create new object for node
	newNode.level = 1; //Set initial level
	newNode.visited[0] = 1; //First node visited
	newNode.vertex = 0; 
	reduceMatrix(adjMat,&newNode,n);
	newNode.path.push_back(0);
	int check = tsp(&newNode,n); //Get final cost for path
	cout<<"\nFinal Cost => "<<check<<"\n\n";
	cout<<"Final Path => ";
	finalPath.push_back(0);
	//Print final path for TSP
	for(int i=0;i<finalPath.size();i++)
	{
		cout<<finalPath.at(i)<<"";
		if(i<finalPath.size()-1)
			cout<<" -> ";
	}
	cout<<"\n";
	return 0;
}

/*

OUTPUT

(base) vedantkp@vedantkp-Lenovo-ideapad-330-15IKB$ g++ 33151_Assignment13.cpp
(base) vedantkp@vedantkp-Lenovo-ideapad-330-15IKB$ ./a.out

Enter number of nodes => 4

Enter adjacency matrix:- 

Element [0][0] => 0

Element [0][1] => 1

Element [0][2] => 3

Element [0][3] => 2

Element [1][0] => 1

Element [1][1] => 0

Element [1][2] => 5

Element [1][3] => 8

Element [2][0] => 3

Element [2][1] => 5

Element [2][2] => 0

Element [2][3] => 3

Element [3][0] => 2

Element [3][1] => 8

Element [3][2] => 3 

Element [3][3] => 0

Adjacency Matrix is :- 
2147483647       1                3                2                
1                2147483647       5                8                
3                5                2147483647       3                
2                8                3                2147483647       

Final Cost => 11

Final Path => 0 -> 3 -> 2 -> 1 -> 0

*/



