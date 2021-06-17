/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Bellman-Ford Shortest Path Algorithm
*/

#include<iostream>
#include<bits/stdc++.h>
#include<vector>
using namespace std;

class EdgeNode
{
	private:
		int wt;
		int vertexNo;
		EdgeNode* link;
	public:
		EdgeNode();
		void setLink(EdgeNode* link);
		EdgeNode* getLink();
		void setWt(int wt);		
		int getWt();
		void setVertexNo(int vertexNo);		
		int getVertexNo();
};

EdgeNode :: EdgeNode()
{
	vertexNo = -1;
	wt = 0;
	link = NULL;
}

void EdgeNode :: setLink(EdgeNode* link)
{
	this->link = link;
}

EdgeNode* EdgeNode :: getLink()
{
	return this->link;
}

void EdgeNode :: setWt(int wt)
{
	this->wt = wt;
}

int EdgeNode :: getWt()
{
	return this->wt;
}

void EdgeNode :: setVertexNo(int vertexNo)
{
	this->vertexNo = vertexNo;
}

int EdgeNode :: getVertexNo()
{
	return this->vertexNo;
}

class VertexNode
{
	private:	
		VertexNode* dlink;
		EdgeNode* link;
		int vertexNo;
	public:
		VertexNode();
		void setDlink(VertexNode* dlink);
		VertexNode* getDlink();
		void setLink(EdgeNode* link);
		EdgeNode* getLink();
		void setVertexNo(int vertexNo);		
		int getVertexNo();			 				
};

VertexNode :: VertexNode()
{
	dlink = NULL;
	link = NULL;
	vertexNo = -1;
}

void VertexNode :: setDlink(VertexNode* dlink)
{
	this->dlink = dlink;
}

VertexNode* VertexNode :: getDlink()
{
	return this->dlink;
}

void VertexNode :: setLink(EdgeNode* link)
{
	this->link = link;
}

EdgeNode* VertexNode :: getLink()
{
	return this->link;
}

void VertexNode :: setVertexNo(int vertexNo)
{
	this->vertexNo = vertexNo;
}

int VertexNode :: getVertexNo()
{
	return this->vertexNo;
}

VertexNode* inputAdjList(int noOfVertices)
{
	int i,j,noOfEdges,k;
	VertexNode* vertexHead = NULL;
	VertexNode* temp = NULL;
	EdgeNode* temp1 = NULL;
	bool* checkArr = new bool[noOfVertices];
	
	for(i=0;i<noOfVertices;i++)
	{
		checkArr[i] = false;
	}

	for(i=0;i<noOfVertices;i++)
	{
		for(k=0;k<noOfVertices;k++)
		{
			checkArr[k] = false;
		}
		VertexNode* newVNode = new VertexNode();
		newVNode->setVertexNo(i);
		cout<<"\nVertex "<<i+1<<" => ";
		cout<<"\nNO OF EDGES : ";
		cin>>noOfEdges;
		for(j=0;j<noOfEdges;j++)
		{
			cout<<"\n------------------------------------------\n";
			int vertexNo;
			int wt;
			EdgeNode* edgeNode = new EdgeNode();
			cout<<"\nEDGE "<<j+1<<" -> ";
			cout<<"\n\tNEIGHBOUR VERTEX NO : ";
			cin>>vertexNo;
			if(vertexNo > noOfVertices || vertexNo <= 0 || vertexNo == (i+1))
			{
				cout<<"\nERROR : INVALID NEIGHBOUR VERTEX NO\n";	
				return NULL;
			}
			if(checkArr[vertexNo-1] == true)
			{
				cout<<"\nERROR : DUPLICATE EDGES NOT ALLOWED\n";
				return NULL;
			}
			vertexNo--;
			checkArr[vertexNo] = true;
			cout<<"\n\tWEIGHT : ";
			cin>>wt;
			edgeNode->setVertexNo(vertexNo);
			edgeNode->setWt(wt);
			cout<<"------------------------------------------\n";
			if(newVNode->getLink() == NULL)
			{
				newVNode->setLink(edgeNode);
			}
			else
			{
				temp1->setLink(edgeNode);
			}
			temp1 = edgeNode;
		}
		if(vertexHead == NULL)
		{
			vertexHead = newVNode;
		}
		else
		{
			temp->setDlink(newVNode);
		}
		temp = newVNode;				
	}	
	return vertexHead;
} 

void displayAdjList(VertexNode* head)
{
	EdgeNode* temp = NULL;
	while(head != NULL)
	{
		cout<<"\nVERTEX "<<head->getVertexNo()<<" => ";
		temp = head->getLink();
		while(temp != NULL)
		{
			cout<<"[ "<<temp->getVertexNo() << " , "<<temp->getWt()<<" ] ";
			temp = temp->getLink();
		}
		head = head->getDlink();
	}
	cout<<"\n";
}

int bellmanFord(VertexNode* head,int* vertexData,int noOfVertices,int src)
{
	int i;
	vector<int> *v1 = new vector<int>[noOfVertices];
	VertexNode* head1 = head;
	EdgeNode* temp = NULL;
	vertexData[src] = 0;
	v1[src].push_back(src);
	for(i=0;i<noOfVertices;i++)
	{
		if(i!=src)
			vertexData[i] = INT_MAX;	
	}
	
	for(i=0;i<(noOfVertices-1);i++)
	{
		while(head != NULL)
		{	
			temp = head->getLink();			
			while(temp != NULL)
			{
				if (vertexData[head->getVertexNo()] != INT_MAX && vertexData[head->getVertexNo()] + temp->getWt() < 						vertexData[temp->getVertexNo()])
				{ 
                			vertexData[temp->getVertexNo()] = vertexData[head->getVertexNo()] + temp->getWt();
                			v1[temp->getVertexNo()].clear();
                			v1[temp->getVertexNo()] = v1[head->getVertexNo()];
                			v1[temp->getVertexNo()].push_back(temp->getVertexNo());
                		}	 	
				temp = temp->getLink();
			}
			head = head->getDlink();
		}
	}	
	
	head = head1;
	temp = NULL;	
	while(head != NULL)
	{	
		temp = head->getLink();			
		while(temp != NULL)
		{
			if (vertexData[head->getVertexNo()] != INT_MAX && vertexData[head->getVertexNo()] + temp->getWt() < 				   vertexData[temp->getVertexNo()])
			{ 
                		return -1;
                	}	 	
			temp = temp->getLink();
		}
		head = head->getDlink();
	}
	
	cout<<"\nDISTANCE ARRAY : [ ";
	for(i=0;i<noOfVertices;i++)
	{
		cout<<vertexData[i]<<" ";
	}
	cout<<"]";
	
	int dest;
	cout<<"\nENTER DESTINATION : ";
	cin>>dest;
	
	for (auto i = v1[dest].cbegin(); i != v1[dest].cend(); ++i) 
        cout << *i << " "; 
	
	return 1;
}

int main()
{
	int* vertexData;
	int noOfVertices;
	VertexNode* vertexHead = NULL;
	cout<<"\nNO OF VERTICES : ";
	cin>>noOfVertices;	
	vertexData = new int[noOfVertices];
	
	vertexHead = inputAdjList(noOfVertices);
	
	if(vertexHead != NULL)
	{
		displayAdjList(vertexHead);
	
		int check = bellmanFord(vertexHead,vertexData,noOfVertices,0);
		if(check == -1)
		{
			cout<<"\nERROR : GRAPH CONTAINS NEGATIVE CYCLE\n";
		}
		else if(check == 1)
		{
			cout<<"\nEXECUTION SUCCESSFUL !\n";
		}
	}		 
	
	return 0;
}

/*
NO OF VERTICES : 5       

Vertex 1 => 
NO OF EDGES : 2

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 2

	WEIGHT : 1
------------------------------------------

------------------------------------------

EDGE 2 -> 
	NEIGHBOUR VERTEX NO : 3

	WEIGHT : 4
------------------------------------------

Vertex 2 => 
NO OF EDGES : 2

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 4

	WEIGHT : 3
------------------------------------------

------------------------------------------

EDGE 2 -> 
	NEIGHBOUR VERTEX NO : 5

	WEIGHT : 2
------------------------------------------

Vertex 3 => 
NO OF EDGES : 2

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 4

	WEIGHT : 5
------------------------------------------

------------------------------------------

EDGE 2 -> 
	NEIGHBOUR VERTEX NO : 5

	WEIGHT : -4
------------------------------------------

Vertex 4 => 
NO OF EDGES : 0

Vertex 5 => 
NO OF EDGES : 0

VERTEX 0 => [ 1 , 1 ] [ 2 , 4 ] 
VERTEX 1 => [ 3 , 3 ] [ 4 , 2 ] 
VERTEX 2 => [ 3 , 5 ] [ 4 , -4 ] 
VERTEX 3 => 
VERTEX 4 => 

DISTANCE ARRAY : [ 0 1 4 4 0 ]
ENTER DESTINATION : 4
0 2 4 
EXECUTION SUCCESSFUL !

----------------------------------

NO OF VERTICES : 4 

Vertex 1 => 
NO OF EDGES : 2

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 2

	WEIGHT : 3
------------------------------------------

------------------------------------------

EDGE 2 -> 
	NEIGHBOUR VERTEX NO : 4

	WEIGHT : 3
------------------------------------------

Vertex 2 => 
NO OF EDGES : 1

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 4

	WEIGHT : -8
------------------------------------------

Vertex 3 => 
NO OF EDGES : 1

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 1

	WEIGHT : 1
------------------------------------------

Vertex 4 => 
NO OF EDGES : 1

------------------------------------------

EDGE 1 -> 
	NEIGHBOUR VERTEX NO : 3

	WEIGHT : 2
------------------------------------------

VERTEX 0 => [ 1 , 3 ] [ 3 , 3 ] 
VERTEX 1 => [ 3 , -8 ] 
VERTEX 2 => [ 0 , 1 ] 
VERTEX 3 => [ 2 , 2 ] 

ERROR : GRAPH CONTAINS NEGATIVE CYCLE

*/