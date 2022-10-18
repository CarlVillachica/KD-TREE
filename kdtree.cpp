//EL KNN anda en el otro link
#include<iostream>
#include<vector>
#include<time.h>
#include<cmath>
#include<math.h>
#include<algorithm> 

using namespace std;

template<class T, int dim>
struct Node 
{			
	Node<T, dim> *right = 0;			
	Node<T, dim>*left = 0;

	int depth = 0;	

	vector<T> nodeVal;
		
	Node() {}
	~Node() {}
};

template<class T, int dim>		
struct KDTree 
{			
	Node<T, dim> *root = 0;
	KDTree() {}
	~KDTree() {}

	double euclideanDistance(Node<T,dim> *value1,vector<T> value2) 
	{
		double gap = 0, result = 0;
		int calc = 0 ,i = 0;

		for (; i < dim; i++)
		{
			calc= value1->nodeVal[i] - value2[i];
			result = result + pow(calc, 2);
		}
		gap = sqrt(result);
		return gap;
	}

	//dfs estandar 
	void DFS(Node<T,dim>*& cPointer) 
	{	
		int i = 0;	
		//vector<T> append1;
		//vector<T> append2;
		//vector<T> append3;

		//T vix= cPointer->depth;
		/*
		if(cPointer->depth == 1){
			append.pushback(append1);
		}
		else if(cPointer->depth == 2){
			append.pushback(append2);
		}
		(cPointer->depth == 3){
			append.pushback(append3); // imprimir por niveles
		}
		*/

		if (cPointer == nullptr) 
		{
			return;
		}
		for (; i < dim; i++)
		{
			cout << cPointer->nodeVal[i] << " ";
		}

		cout << " Depth => " << cPointer->depth<<endl;

		DFS(cPointer->right);
		DFS(cPointer->left);
	}

	// tomando de referencia el NN presentado en https://www.youtube.com/watch?v=Glp7THUpGow&ab_channel=StableSort

	void RecursiveNN(Node< T , dim > *start, vector< T > end ,Node< T , dim > *&temp, int depth, double& minDistance =-1) 
	{	

		if (!start) 
			return;

		if (minDistance==-1 || euclideanDistance(start, end) < minDistance) 
		{
			minDistance = euclideanDistance(start, end);
			temp = start;
		}


		int cut = depth % dim;
		bool right = true ;

		if (end[cut] > start->nodeVal[cut]) 
		{				
			RecursiveNN(start->right,end,temp,depth++,minDistance);
		}
		else 
		{
			right = false;
			RecursiveNN(start->left,end,temp,depth++,minDistance);
		}


		if (fabs(start->nodeVal[cut] - end[cut]) < temp->nodeVal[cut]) 
		{	
			if (right) {
				RecursiveNN(start->left,end,temp,depth++,minDistance);
			}
			else {
				RecursiveNN(start->right,end,temp,depth++,minDistance);
			}
		}
	}
 
	//referenciado del video https://www.youtube.com/watch?v=Glp7THUpGow&ab_channel=StableSort  
	void recursiveKNN(vector<T> append, vector< pair<double ,Node<T, dim> *>> & ans, Node<T, dim>* ptr) {
		if (!ptr) {
			return;
		}
		ans.push_back(make_pair(distance(ptr,append),ptr));
		recursiveKNN(append,ans,ptr->right);		
		recursiveKNN(append,ans,ptr->left);
	}


	void KNN(vector<T> append ,int neightbor ) {		
		
		vector<pair<double,Node<T,dim>*>> vix;

		recursiveKNN(append, vix, root);

		sort(vix.begin(),vix.end());		
		
		for (int i = 0; i < vix.size() && i < neightbor; i++) {
			for (int j = 0; j < dim; j++) {
				cout<<vix[i].second->val[j];
			}
		}

		return;
	}
	
	void NN( vector<T> append ) 
	{		

		double bestDistance = -1;
		Node<T, dim> *tempHigh=nullptr;
		
		RecursiveNN(root,append,tempHigh,0,bestDistance);
		cout << "NN => ";
		for (int i = 0; tempHigh && i < dim; i++)
		{
			cout << tempHigh->nodeVal[i]<<" ";
		}

		return;
	}
	
	Node<T, dim>* find(Node<T, dim>*& preptr, Node<T, dim> *currentRoot,  vector<T>& append, int initialDepth) 
	{
		
        int depth = initialDepth; 
		Node<T, dim>* cPointer = currentRoot; 
		Node<T, dim>* tmp ;   

		if (append.size() == 0)  // usando la idea de append del profesor
		{
			cout << "Buscar => ";
			T vix;
			for (int i = 0; i < dim; i++) 
			{
				cin >> vix;
				append.push_back(vix);
			}
		}

		Node<T, dim>* found = 0;

		while (cPointer) 
        {

            if (cPointer->nodeVal[depth % dim] == append[depth % dim]) 
			{
				found = cPointer;
				for (int i = 0; found && i < dim; i++) 
				{
					tmp = cPointer;
					if (cPointer->nodeVal[i] == append[i]) 
					{
						return found;
					}
                    else 
                    {
						found = find(tmp, cPointer->left, append, depth + 1);
						if (found) {
						    preptr = tmp;
						    return found;
						}
						found = find(tmp, cPointer->right, append, depth + 1);
						if (found) {
						    preptr = tmp;
						    return found;
						}
				
                    }
				}
			}

			else 
			{
				preptr = cPointer;
				if (cPointer->nodeVal[depth % dim] < append[depth % dim]) 
				{   
				cPointer = cPointer->right;
				}
				else 
				{   
				cPointer = cPointer->left;
				}
			}

			depth++;
		}
		return found;
	}
	

	void insert() 
    	{				
		vector<T> append;
		cout << "Insert => ";
		T vix;

        Node<T, dim>* toInsert = new Node<T, dim>;
		Node<T, dim>* preptr = root;
		Node<T, dim>* ptr = search(preptr, root,  append , 0);

		toInsert->nodeVal = append;

		for (int i = 0; i < dim; i++) 
        	{
			cin >> vix;
			append.push_back(vix);
		}
		/* esto se puede excluir */
		if (!ptr) 
        	{
			root = toInsert;
		}
		else 
        	{
			toInsert->depth = preptr->depth + 1;
			if (append[preptr->depth % dim] < preptr->nodeVal[preptr->depth % dim]) 
			{
				preptr->left = toInsert;
			}
			else 
			{
				preptr->right = toInsert;
			}
		}

		
	}


}
int main() {
	const int nodes = 10;	
	const int d = 3;	
	vector<Node<int, d>*> init;
	KDTree<int, d> KDTree;

	Node<int, d>* nodos = new Node<int, d>[nodes];
	int instancia[nodes][d] = {{2,7,1},{9,1,5},{5,4,4},{2,9,4},{9,1,8},{1,8,2},{7,6,6},{2,1,4},{6,3,4},{8,7,4}};
	
	for (int i = 0; i < nodes; i++) 
	{
		init.push_back(&nodos[i]);
		for (int j = 0; j < d; j++) 
		{
			init[i]->nodeVal.push_back(instancia[i][j]);
		}
	}
}
