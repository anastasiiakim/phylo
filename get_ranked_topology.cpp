#include <string>
#include <fstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <algorithm>


using namespace std;

struct Node
{
    string label;
    string desctaxa;
    Node * left;
    Node * right;
    Node * parent;
    Node ()
    {
        debug_name = st_count;
        st_count++;
        right = NULL;
        left = NULL;
        parent = NULL;
        label = "";
        desctaxa = "";
        rank = 0;
        length = 0.;
        time = 0.;
        distFrRoot = 0.;
        //  printf("nodeWithName %d created \n", debug_name);
    };
    static int st_count;
    int debug_name;
    double length;
    double time;
    int rank;
    double distFrRoot;
    bool isRight;
    int population; // from L to R
    int outdegree;
};

int Node::st_count = 0;

inline void removeSpaces(string & str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            str.erase(i, 1);
            i--;
        }
    }
}

inline double getBranchLengths (int  i, string & str)
{
    double temp = 0.;
    double tmp = 0.1;
    double tempsc = 0.;
    double decim = 0.;
    int scientific = 0;
    while((double) (str[i] - '0') >= 0 && (double) (str[i] - '0') <= 9)
    {
        temp = temp * 10 + (double) (str[i] - '0');
        if(str[i+1] == '.')
        {
            ++i;
            while(str[i+1] != ',' && str[i+1] != ')' && str[i+1] != ' '  && str[i+1] != ';')
            {
                if(str[i+1] == 'e' && str[i+2] == '-')
                {
                    i = i+2;
                    while(str[i+1] != ',' && str[i+1] != ')' && str[i+1] != ' '  && str[i+1] != ';')
                    {
                        tempsc = tempsc * 10 + double (str[i+1] - '0');
                        ++i;
                    }
                    return (temp + decim) * pow(10, -tempsc);
                }

                if(str[i+1] == 'e' && str[i+2] == '+')
                {
                    i = i+2;
                    while(str[i+1] != ',' && str[i+1] != ')' && str[i+1] != ' '  && str[i+1] != ';')
                    {
                        tempsc = tempsc * 10 + double (str[i+1] - '0');
                        ++i;
                    }
                    return (temp + decim) * pow(10, tempsc);
                }

                decim +=  (double) (str[i+1] - '0') * tmp;
                tmp *= 0.1;
                ++i;
            }
        }
        ++i;
    }

    return  temp + decim;   

}


inline void pushToArray (Node * p, int & tail, Node ** &ar)
{
    int i = tail;
    ar[i] = p;  
    while((i > 0) && (ar[i] -> time < ar[i - 1] -> time))
    {
        Node * tempe;
        tempe = ar[i];
        ar[i] = ar[i-1];
        ar[i-1] = tempe;
        i--;
    }

    tail ++;
}

inline void pushToArrayNoSort (Node * p, int & tail, Node ** &ar)
{
    int i = tail;
    ar[i] = p;   
    tail ++;
}



inline Node * popFromArray (int & tail, Node ** &ar)
{
    tail --;
    return ar[tail];
}



inline void distFromRoot (Node * p)
{	
    if (p -> label.size() == 0)
    {	
        p -> right -> distFrRoot = p -> distFrRoot + p -> right -> length;
        p -> left -> distFrRoot = p -> distFrRoot + p -> left -> length;
        p -> right -> parent = p;
        p -> left -> parent = p;

        distFromRoot(p -> right);
        distFromRoot(p -> left);
    }
    else 
    {
        p -> outdegree = 0;
        p -> population = 1;
    }

}

inline void countParentheses (std::string & str)
{
    int lpcount = 0;
    int rpcount = 0;
    int i = 0;

    while(i < str.size())
    {
        if(str[i] == '(') lpcount++;
        else if(str[i] == ')') rpcount++;
        ++i; 
    }	

    if(lpcount != rpcount)
    {
        cout << "Error: The numbers of opening and closing parentheses do not match" << endl;
    }
}


inline void deleteStack (std::stack <Node *> stk)
{
    while(!stk.empty())
        stk.pop();
}

inline void pushNodes (int & lbl, std::stack <Node *> & stk, std::string str)
{
    int i = 0;
    while(i < str.size())
    {
        if(((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) && (str[i+1] != '-' && str[i+1] != '+'))
        {
            std::string strtemp;
            strtemp += str[i]; 
            while(str[i+1] != ':')
            {
                strtemp += str[i+1];		   
                ++i;
            }

            Node * newnode = new Node();
            newnode -> label = strtemp;
            newnode -> time = 0.;
            ++lbl;	
            ++i;
            if((double) (str[i+1] - '0') >= 0 && (double) (str[i+1] - '0') <= 9)
            {
                newnode -> length = getBranchLengths(i+1, str);
            }
            stk.push(newnode);
        }


        else if ( str[i] == ')' )
        {	
            Node * newnode = new Node();
            newnode -> right = stk.top();
            newnode -> right -> isRight = 1;
            stk.pop();
            newnode -> left = stk.top();
            newnode -> left -> isRight = 0;
            stk.pop();
            ++i;

            if((double) (str[i+1] - '0') >= 0 && (double) (str[i+1] - '0') <= 9)
            {
                newnode -> length = getBranchLengths(i+1, str);
            }

            newnode -> time = newnode -> right -> time + newnode -> right -> length;
            stk.push(newnode);

        } 
        ++i;
    }

}	

inline void getRanks(Node* newnode, int & tail, Node ** &ar)
{
    int k = 1, k1 = 1; 
    while(newnode -> time != 0)
    {
        Node * tempnode;
        tempnode = popFromArray(tail, ar);
        if(tempnode -> rank == 0) tempnode -> rank = k; 
        k++;
        k1++;
        if((tail > 0) && (tempnode -> time == ar[tail - 1] -> time) && (tempnode -> time > 0))
        {
            int x;
            k--;	
            x = rand()%(k1-k+1)+k;
            tempnode -> rank = x;
            if(x == k) ar[tail-1] -> rank = k1; 
            else ar[tail-1] -> rank = k;

        }

        else if((tail > 0) && (tempnode -> time != ar[tail - 1] -> time))  
        {
            k = k1;
        }

        pushToArray(tempnode -> right, tail, ar);
        pushToArray(tempnode -> left, tail, ar);

        newnode = ar[tail - 1];
    }

}	


Node * nodeFrRank (Node * p, int rankValueGT)
{
    if(p == NULL) cout << "Tree is empty" << endl;
    Node * k;
    Node * t;
    if(p -> label.size() == 0)
    {   

        if(p -> rank != rankValueGT) 
        {
            t =  nodeFrRank(p -> right, rankValueGT);
            k =  nodeFrRank(p -> left, rankValueGT);
            if(t != 0) return t;
            return k;
        }
        else 
        {
            return p;
        }   
    }
    else return 0;
}   

inline void getTaxa (Node * p, std::stack <Node *> & allTaxa)
{
    if(p -> label.size() == 0)
    { 
        getTaxa(p -> right, allTaxa);
        getTaxa(p -> left, allTaxa);
    }
    else 
    {
        allTaxa.push(p);
    }	
}

inline void sortString(int n, string* str)
{
    string temp;
    for(int i = 0; i < n-1; ++i)
    {
        for(int j = i+1; j < n; ++j)
        {
            if(str[i] > str[j])
            {
                temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }   
}


inline void getS(Node * p, double * s)
{
    if(p -> label.size() == 0)
    {
        s[p -> rank - 1] = p -> time;
        getS(p -> right, s);
        getS(p -> left, s);
    }
}

int main(int argc, char **argv)
{ 
    //ST.txt input file
    int arg_counter = 1;
    string str;
    string arg;
    while(arg_counter < argc)
    {
        arg = argv[arg_counter];
        if(arg == "-st")
        {
            ++arg_counter;
            ifstream st_file(argv[arg_counter]);
            getline(st_file, str, '\n');	
            st_file.close();
            ++arg_counter;
            if(arg_counter < argc) arg = argv[arg_counter];
        }


        removeSpaces(str);
        std::stack <Node *> stkST;

        int lbl = 0;
        double tempdist = 0.;	
        int labelcount = 0;
        countParentheses(str); 

        pushNodes(lbl, stkST , str);
        Node * newnode;
        newnode = stkST.top();
        newnode -> distFrRoot = 0.;
        int tail = 0;

        distFromRoot(newnode);

        int N = lbl;
        Node ** ar = new Node * [N];
        int ** ar_y = new int * [N];
        for (int i = 0; i < N; i++) ar_y[i] = new int [N];

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
            {
                ar_y[i][j] = 0;
            }

        pushToArray(newnode, tail, ar);	
        getRanks(newnode, tail, ar);

        int lblscounter = 0;
        string strLabels[N];

        double s[N-1];
        getS(newnode, s);

        if(arg == "-out") 
        {
            // ofstream topo_file("STtopo.txt");
            ++arg_counter;
            ofstream topo_file(argv[arg_counter]);

            for(int i = 2; i < N; ++i)
            {   

                stack <Node *> allTaxa;
                string tempstr[N];
                int tmpcount = 0;
                getTaxa(nodeFrRank(newnode, i), allTaxa);
                while(!allTaxa.empty())
                {
                    tempstr[tmpcount] = allTaxa.top() -> label;
                    allTaxa.pop();
                    tmpcount++;   
                } 
                sortString(N, tempstr);
                for(int j = 0; j < N; ++j)
                    topo_file << tempstr[j];
                topo_file << "-" << i << "-";
            }
            topo_file << "\n";
            topo_file.close();
        }
        ++arg_counter;    
        for(int i = 0; i < N; ++i)
        {
            delete[] ar_y[i];	
        }
        delete[] ar_y;	

        delete[] ar;

        deleteStack(stkST);
    }


    return 0;
} 

