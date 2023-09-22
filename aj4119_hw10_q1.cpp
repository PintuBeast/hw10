#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class Account
{
    float amount, deficit;  // deficit is the amount of money less than the average bill(+ve for those who have to pay, 
                                //-ve for those who are owed money).
    string name;
    Account *link_ptr;
    
    public: 
        Account();
        Account(float amount, float deficit, string name, Account* link_ptr) ;
          
        float getAmount();
        void setAmount(float amount);    
        float getDeficit();
        void setDeficit(float deficit);         
        string getName();
        void setName(string mame);
        Account* getLink();
        void setLink(Account* link);


};

typedef Account* AccountPtr;

Account::  Account(float amount, float deficit,  string name, AccountPtr link_ptr) 
           : amount(amount),deficit(deficit), name(name), link_ptr(link_ptr) {};

Account::  Account() 
           : amount(0), deficit(0), name(" "), link_ptr(NULL) {};


float Account:: getAmount()
{
    return this -> amount;
}

void Account:: setAmount(float amount)
{
    this -> amount = amount;
}


float Account:: getDeficit()
{
    return this -> deficit;
}

void Account:: setDeficit(float deficit)
{
    this -> deficit = deficit;
}


string Account:: getName()
{
    return this -> name ;

}
void Account::setName(string name)
{
    this -> name = name ;
}

AccountPtr Account:: getLink()
{
    return this -> link_ptr ;

}
void Account::setLink(AccountPtr link)
{
    this -> link_ptr = link ;
}

void insertHead(AccountPtr& head, float amount,  string name);

void insertHead(AccountPtr& head, float amount,  string name)
{
    AccountPtr temp_ptr;
    temp_ptr = new Account(amount, 0, name, head);
    head = temp_ptr; 
}


void createVectors(AccountPtr& head, vector<Account>& creditorVector, vector<Account>& debtorVector,  float averageAmount );

void createVectors(AccountPtr& head, vector<Account>& creditorVector, vector<Account>& debtorVector,  float averageAmount )
{
    AccountPtr tmp = head;
    while(tmp != NULL)
    {
       
        float amount = tmp -> getAmount();
        
        float deficit = averageAmount - amount;
        if(deficit ==  0)
        {
            cout<< tmp -> getName()<<", you don't need to do anything"<<endl;
        }
        else
        if(deficit > 0)
        {
            
            Account *ac1 = new Account(amount, deficit,tmp -> getName(), NULL);
            debtorVector.push_back(*ac1);
        }
        else
        if(deficit < 0)
        {
            
            Account *ac1 = new Account(amount, -1 * deficit, tmp -> getName(), NULL);
            creditorVector.push_back(*ac1);            
        }  

        tmp = tmp -> getLink();      
    }

}                    
void sortVectors(vector<Account>& creditorVector, vector<Account>& debtorVector); // sort creditors in Decending Order and Debtors in descending order




void sortVectors(vector<Account>& creditorVector, vector<Account>& debtorVector)
{
    int creditorSize = creditorVector.size(), debitorSize = debtorVector.size();


    //bubblesort for Creditorvector - Descending
    for(int i = creditorSize - 1 ; i>0 ; i--)
    {
        for (int j = 0 ; j < i ; j++)
        {
            if(creditorVector[j].getDeficit() < creditorVector[j + 1].getDeficit() )
            {
                Account temp = creditorVector[j + 1] ;
                creditorVector[j + 1] = creditorVector[j];
                creditorVector[j] = temp;               
            }
        }
    }
    //bubblesort for Debtorvector - Ascending

    for(int i = debitorSize - 1 ; i > 0 ; i--)
    {
        for (int j = 0 ; j < i ; j++)
        {
            if(debtorVector[j].getDeficit() > debtorVector[j + 1].getDeficit() )
            {
                Account temp = debtorVector[j + 1] ;
                debtorVector[j + 1] = debtorVector[j];
                debtorVector[j] = temp;               
            }
        }
    }


}

void splitExpenses(vector<Account>& creditorVector, vector<Account>& debtorVector);

void splitExpenses(vector<Account>& creditorVector, vector<Account>& debtorVector)
{
    int creditorSize = creditorVector.size(), debtorSize = debtorVector.size();
    int start = 0;


    for(int i = 0 ; i < creditorSize ; i++)
    {
        if(creditorVector[i].getDeficit() == 0)
        {
            continue;                
        }        
        for(int j = start ; j < debtorSize ; j++)
        {
            if(debtorVector[j].getDeficit() == 0)
            {
            continue;;                
            }              

            float difference = creditorVector[i].getDeficit() - debtorVector[j].getDeficit() ;
            if(creditorVector[i].getDeficit() >=  debtorVector[j].getDeficit())
            {    
                cout<<debtorVector[j].getName()<<", you give "<<creditorVector[i].getName()<<" $"<<debtorVector[j].getDeficit()<<endl;
                creditorVector[i].setDeficit(difference);
                debtorVector[j].setDeficit(0); 
                start++;
            }
            else
            {
                cout<<debtorVector[j].getName()<<", you give "<<creditorVector[i].getName()<<" $"<<creditorVector[i].getDeficit()<<endl;
                debtorVector[j].setDeficit(-1 * difference);
                creditorVector[i].setDeficit(0);                
            }
        }
        
    }
    

}


int main()
{
    ifstream inputFile;
    string fileName;
    cout<<"Please enter the filename: ";
    cin>>fileName;
    inputFile.open(fileName);

    AccountPtr head = NULL;

    float amount, sumAmounts=0,averageAmount;
    string name;

    int numPeople = 0;

    while(inputFile>>amount)
    {   sumAmounts += amount;
        inputFile.ignore(9999,'\t');
        getline(inputFile,name);
        insertHead(head, amount, name);
        numPeople++;
    }
    averageAmount = sumAmounts / numPeople;
vector<Account> creditorVector, debtorVector; 
int creditorSize, debtorSize;

createVectors(head, creditorVector, debtorVector, averageAmount);
sortVectors(creditorVector,debtorVector);
splitExpenses(creditorVector,debtorVector);
cout<<"In the end, you should all have spent around $"<<averageAmount;

return 0;
}