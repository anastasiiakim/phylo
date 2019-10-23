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


void getClades (Node * p, std::stack <string> & allTaxa)
{
    if(p -> label.size() == 0)
    { 
        getClades(p -> right, allTaxa);
        getClades(p -> left, allTaxa);
        allTaxa.push(p -> desctaxa);
    }
}

void getTaxa(Node * p, stack <Node *> & allTaxa)
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


void sortString(int n, string* str)
{
    string temp="";
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



void finSort(int n, string* str)
{
    string temp="";
    string hash="|";
    for(int i = 0; i < n-1; ++i)
    {
        for(int j = i+1; j < n; ++j)
        {
            // cout << i << " " << j << " " << str[i] << " " << str[j] << endl;
            if(str[i].size() == str[j].size())
            {
                //cout << "hippo: " << str[i] << " " << str[j] << endl;
                int t = 0;
                string itemp = "" ;
                string jtemp = "" ;

                while(hash.compare(&str[i][t]) != 0)
                {
                    itemp += str[i][t];
                    jtemp += str[j][t];
                    ++t;
                }

                if(itemp > jtemp)
                {
                    temp = str[i];
                    str[i] = str[j];
                    str[j] = temp; 

                }
            }
        }
    }   
}




void getDescTaxa(Node * p, int N)
{
    string hash = "|";
    if(p -> label.size() == 0)
    {
        stack <Node *> allTaxa;
        getTaxa(p, allTaxa);

        int tmpcount = 0;
        int cur_size = allTaxa.size();
        string tempstr[cur_size];
        string temp;
        while(!allTaxa.empty())
        {
            tempstr[tmpcount] = allTaxa.top() -> label;
            temp += allTaxa.top() -> label;
            allTaxa.pop();
            tmpcount++;  
        } 
        sortString(cur_size, tempstr);
        temp = "";

        for(int j = 0; j < cur_size; ++j)
        {
            temp +=  tempstr[j];
            temp += hash;
        }
        p -> desctaxa = temp;
        getDescTaxa(p -> right, N);
        getDescTaxa(p -> left, N);
    }
    else
    {
        p -> desctaxa = p -> label;
    }
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
            //cout << newnode -> debug_name << " " << newnode -> label << endl;
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

            stk.push(newnode);

        } 
        ++i;
    }

}	



void sort(string *s, int N) 
{ 
    for (int i = 1; i < N; i++) 
    { 
        string temp = s[i]; 
        int j = i - 1; 
        while (j >= 0 && temp.size() < s[j].size()) 
        { 
            s[j+1] = s[j]; 
            j--; 
        } 
        s[j+1] = temp; 
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
        if(arg == "-gts")
        {
            ++arg_counter;
            ifstream gt_file(argv[arg_counter]);
            ofstream topo_file("utopos.txt");
            ++arg_counter;
            if(arg_counter < argc) arg = argv[arg_counter];


            while(getline(gt_file, str, ';'))
            {
                if(str.size() < 3) break;

                removeSpaces(str);
                std::stack <Node *> stkST;

                int lbl = 0;
                double tempdist = 0.;	
                int labelcount = 0;
                countParentheses(str); 

                pushNodes(lbl, stkST, str);

                Node * newnode;
                newnode = stkST.top();
                int N = lbl;

                stack <string> allTaxa;
                getDescTaxa(newnode, N);
                getClades(newnode, allTaxa);

                int tmpcount = 0;
                string tempstr[N];

                while(!allTaxa.empty())
                {
                    tempstr[tmpcount] = allTaxa.top();
                    allTaxa.pop();
                    tmpcount++;  
                }


                sort(tempstr, N-1);
               // for(int j = 0; j < N-1; ++j)
              //  {
              //      cout << tempstr[j] << " ";
              //      cout << tempstr[j].size() << " ";
             //   }
             //   cout << endl;

                //cout << "--------after---sort-------" << endl;
                finSort(N-1, tempstr);

                for(int j = 0; j < N-1; ++j)
                {
                    topo_file << tempstr[j] << "-";
                    //            cout << tempstr[j].size() << " ";
                }
                topo_file << endl;
                //        cout << tempstr[0][0] << endl;        
                deleteStack(stkST);
            }
            gt_file.close();
            topo_file.close();
        }
    }


    return 0;
} 

