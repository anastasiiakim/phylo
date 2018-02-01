#include <string>
#include <fstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <iomanip> // output more decimals
#define eps 0.0e-15
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



inline void distFromRoot (Node * p, int N)
{	
    if (p -> label.size() == 0)
    {	
        p -> right -> distFrRoot = p -> distFrRoot + p -> right -> length;
        p -> left -> distFrRoot = p -> distFrRoot + p -> left -> length;
        p -> right -> parent = p;
        p -> left -> parent = p;

        distFromRoot(p -> right, N);
        distFromRoot(p -> left, N);
    }
    else 
    {
        p -> outdegree = 0;
        p -> population = 1;
        p -> rank = N; 
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


inline void deleteTree (Node * p)
{
    if (p != NULL )
    {
        deleteTree(p -> right);
        deleteTree(p -> left);
        delete p;
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
            //cout << "Warning: some ranks are tied. Rand assignment was applied" << endl;		
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


inline void writeRankTree(Node * p, ofstream & file)
{
    if(p -> label.size() != 0) file << p -> label;
    else 
    {
        file << '(';
        writeRankTree(p -> left, file);
        file << ','; 
        writeRankTree(p -> right, file);
        file << ')'; 
    }
    if(p -> rank != 1)
        file << ':' << p -> rank - p -> parent -> rank;
}

inline void outputRankTree(Node * p)
{
    if(p -> label.size() != 0) cout << p->label;
    else 
    {
        cout << '(';
        outputRankTree(p -> left);
        cout << ',';
        outputRankTree(p -> right);
        cout << ')';
    }
    if(p -> rank != 1)
        cout << ':' << p -> rank - p-> parent -> rank;    
}

inline void anomalousCount(ifstream & fingt, string & strtopo, string & strgt, int & indicator)
{
    if(strtopo.compare(strgt) != 0)
    {   
        ++indicator;
    }
    else  indicator = 0;
}


int main(int argc, char **argv)
{ 
    int arg_counter = 1;
    string strtopo;
    string strgt; 
    string strtree;
    string arg;

    while(arg_counter < argc)
    {
        arg = argv[arg_counter];
        if(arg == "-topo") //STtopo.txt
        {
            ++arg_counter;
            ifstream sttopo_file(argv[arg_counter]);
            getline(st_topo, strtopo, '\n');
            sttopo_file.close();
            ++arg_counter;
            if(arg_counter < argc) arg = argv[arg_counter];
        }

        if(arg == "-prob")
        {
            ++arg_counter;
            ifstream probs_file(argv[arg_counter]); //temp_probForEachGT.txt

            int indicator = 0;
            int out_indicator;
            ofstream outfile;
            Node **arGT = new Node * [N];
            ofstream finanom;
            ofstream finbinom;
            finanom.open("num_anomal.txt", ios::app);
            finbinom.open("num_anomal_binom.txt", ios::app);
            while(getline(probs_file, strgt))
            {
                if(strgt.size() < 3) break;
                out_indicator = indicator;
                anomalousCount(probs_file, strtopo, strgt, indicator); 

                if(indicator == 0) break;
            }
            probs_file.close();
        }
        if(out_indicator != 0) 
        {
            finanom  << out_indicator << endl;
            finbinom  << 1 << endl;
        }
   //     cout  << out_indicator << endl;
        delete [] arGT;

        finanom.close();
        finbinom.close();
        ++arg_counter;
    }
    return 0;
} 

