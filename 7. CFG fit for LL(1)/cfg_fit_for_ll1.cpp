#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
int num_of_productions;
vector<string> productions;
vector<char> terminals;
vector<char> nonTerminals;
vector<string> newProductions;
vector<string> deterministicProductions;
int isInTerminal(char c){//Check if a value is a valid Terminal
    if(find(terminals.begin(),terminals.end(),c)!=terminals.end()){
        return 1;
    }
    return 0;
}
int isInNonTerminal(char c){//Check if a value is a valid NonTerminal
    if(find(nonTerminals.begin(),nonTerminals.end(),c)!=nonTerminals.end()){
        return 1;
    }
    return 0;
}
int isTerminal(char tem){//Assume Capital Letters As non terminals and small letters as terminals
    if(tem>='A' && tem<='Z'){
        return 0;
    }else{
        return 1;
    }
}
int prepareListOfCharacter(){//Prepare List Of Valid Characters
    for(int i=0;i<productions.size()-1;i++){
        string tempString=productions[i];
        int tempStringLen = tempString.length();
        for(int j=0;j<tempStringLen;j++){
            char t = tempString[j];
            if(!(t==' ')){
                if(isInNonTerminal(t)||isInTerminal(t)){
                    continue;
                }else{
                    //Check if Terminal Or Non Terminal.
                    // Add in the respective Vector
                    if(isTerminal(t)){
                        terminals.push_back(t);
                    }else{
                        nonTerminals.push_back(t);
                    }
                }
            }
        }
    }
    return 1;
}
int readProductions(){
    //Function to read the productions from the input file
	//char ipf[12];
	//strcpy(ipf,"input1.txt");
    //READING BEGINS
    string fileName ="";
    //strcpy(fileName,"inpu1.txt");
    cout<<"Input File Name : ";
    cin>>fileName;
    cout<<endl;
    ifstream input(fileName.c_str());
    string tempString;
    input>>num_of_productions;
    getline(input, tempString, '\n');
    while (getline(input, tempString, '\n')) {
        productions.push_back(tempString);
    }
    cout<<"****PRODUCTIONS****: "<<endl;
    cout<<"Num Of Productions : "<<num_of_productions<<endl;
    for(int i=0;i<productions.size()-1;i++){
        cout<<productions[i]<<endl;
    }
    //cout<<"DATA FROM INPUT FILE ENDS: "<<endl<<endl;
    //READING ENDS
    prepareListOfCharacter();
    cout<<"\n****Terminals****\t";
    for(int i=0;i<terminals.size();i++){
        cout<<terminals[i]<<" ";
    }
    cout<<"\n****Non Terminals****\t";
    for(int i=0;i<nonTerminals.size();i++){
        cout<<nonTerminals[i]<<" ";
    }
    cout<<"\n";

    return 1;
}
char nonTermGen(){
    //Generate A Non Terminal To Replace
    char nonTerm = 'A';
    while(true){
        if(isInNonTerminal(nonTerm)){
            nonTerm += 1;
        }else{
            return nonTerm;
        }
        if(nonTerm==('Z'+1)){
            return '$';
        }
    }
}
int isLeftRecursive(char c){
    for(int j=0;j<productions.size();j++){
        string tempString = productions[j];
        if(tempString[0]==c){
            if(!isInNonTerminal(tempString[2])){
            //First Character Is A Terminal
            newProductions.push_back(tempString);
            }else{
                //First Character Is A Non Terminal
                if(tempString[2]==c){
                    //Direct Recursion
                    return 1;
                }else{
                    //Indirect Recursion IGNORED FOR NOW
                    newProductions.push_back(tempString);
                }
            }
        }else{
            //cout<<"\n First Character not "<<c;
        }      
    }
    return 0;
}

int rectifyLeftRecursion(char c){
    //    A->Ap|B
    //      ||
    //     A->BA'  
    //    A'->pA'|e

    //Adding Beta Productions
    char newNonTerm = nonTermGen();
    if(newNonTerm=='$'){
        cout<<"Ran Out Of Non Terminals To Assign. Too Many Left Recursive Entries.";
        exit;
    }

    vector<string> beta;
    for(int k=0;k<productions.size();k++){
        if(productions[k][0]==c){
            if(productions[k][2]!=c){
                beta.push_back(productions[k]);
            }
        }
    }
    for(int k=0;k<beta.size();k++){
        //Add Beta Productions
        string betaProduction = beta[k]+newNonTerm;
        if(!isInNonTerminal(newNonTerm)){
            nonTerminals.push_back(newNonTerm);
        }
        newProductions.push_back(betaProduction);
    }


    //Dealing With A' Productions
    for(int j=0;j<productions.size();j++){
        if(productions[j][0]==c){
            string tempString = productions[j];
            if(tempString[2]==c){
                //Direct Left Recursion Present
                string alpha = tempString.substr(3);
                //Add A' Productions
                string alphaProduction = "";
                alphaProduction += newNonTerm;
                alphaProduction += (" "+alpha);
                alphaProduction += newNonTerm;
                if(!isInNonTerminal(newNonTerm)){
                    nonTerminals.push_back(newNonTerm);
                }
                newProductions.push_back(alphaProduction);
            }
        }
    }
    string epsilonProduction = "";
    epsilonProduction += newNonTerm;
    epsilonProduction += " #";
    newProductions.push_back(epsilonProduction);
    terminals.push_back(newNonTerm);
    return 1;
}

int printFinalGrammar(){
    cout<<"\n>>>FINAL LEFT RECURSION FREE GRAMMAR<<<"<<endl;
    for(int i=0;i<newProductions.size();i++){
        cout<<newProductions[i]<<endl;
    }
    return 1;
}


int clearProductions(char c){
    vector<string> tempProds=productions;
    int j=0;
    while(true){
        if(tempProds[j][0]==c){
            tempProds.erase(tempProds.begin()+j);
        }else{
            j++;
        }
        if(j==tempProds.size()){
            break;
        }
    }
    productions = tempProds;
    return 1;
}

int isNonDeterministic(char c){
    vector<string> tempProductions; 
    for(int j=0;j<productions.size();j++){
        string tempString = productions[j];
        if(productions[j][0]==c){
            tempProductions.push_back(productions[j]);
        }
    }
    for(int j=0;j<tempProductions.size();j++){
        string tempString = tempProductions[j];
        for(int k=0;k<tempProductions.size();k++){
            if(tempString==tempProductions[k]){
                continue;
            }else{
                if(tempString.length()>tempProductions[k].length()){
                    for(int l=tempProductions[k].length();l>1;l--){
                        size_t found = tempString.find(tempProductions[k].substr(1,l));
                        if(found!=string::npos){
                            if(found==1){
                                return 1;
                            }
                        }
                    }
                }else{
                    for(int l=tempString.length();l>1;l--){
                        size_t found = tempProductions[k].find(tempString.substr(1,l));
                        if(found!=string::npos){
                            if(found==1){
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int exists(string s){
    for(int m=0;m<deterministicProductions.size();m++){
        if(s == deterministicProductions[                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   m]){
            return 1;
        }
    }
    return 0;
}

int rectifyNonDeterminism(char c){

    char newNonTerm = nonTermGen();
    if(newNonTerm=='$'){
        cout<<"!!! Ran out of Non Terminals To Assign. Too Many Non Deterministic Entries !!!";
        exit;
    }

    vector<string> tempProductions; 
    for(int j=0;j<productions.size();j++){
        string tempString = productions[j];
        if(productions[j][0]==c){
            tempProductions.push_back(productions[j]);
        }
    }
    for(int j=0;j<tempProductions.size();j++){
        string tempString = tempProductions[j];
        int flag=0;
        for(int k=0;k<tempProductions.size();k++){
            if(tempString==tempProductions[k]){
                continue;
            }else{
                if(tempString.length()>tempProductions[k].length()){
                    for(int l=tempProductions[k].length();l>1;l--){
                    	size_t found = tempString.find(tempProductions[k].substr(1,l));
                        if(found!=string::npos){
                        	if(found==1){
                                flag=1;
                                if(!isInNonTerminal(newNonTerm)){
                                    nonTerminals.push_back(newNonTerm);
                                }
                                string newProd1 = "";
                                newProd1+=c;
                                newProd1 += (" "+tempString.substr(1,l));
                                newProd1 += newNonTerm;
                                

                                string newProd2 = "";
                                newProd2+=newNonTerm;
                                newProd2 += (" "+tempString.substr(l+1));
                                if(!exists(newProd1)){
                                    deterministicProductions.push_back(newProd1);
                                }
                                if(!exists(newProd2)){
                                    deterministicProductions.push_back(newProd2);
                                }
                            }
                        }
                    }
                }else{
                    for(int l=tempString.length();l>1;l--){
                        size_t found = tempProductions[k].find(tempString.substr(1,l));
                        if(found!=string::npos){
                            if(found==1){
                                flag=1;
                                if(!isInNonTerminal(newNonTerm)){
                                    nonTerminals.push_back(newNonTerm);
                                }
                                string newProd1 = "";
                                newProd1+=c;
                                newProd1 += tempString.substr(1,l);
                                newProd1 += newNonTerm;
                                string newProd2 = "";
                                newProd2+=newNonTerm;
                                newProd2 += (" "+tempString.substr(l+1));
                                if(!exists(newProd1)){
                                    deterministicProductions.push_back(newProd1);
                                }
                                if(!exists(newProd2)){
                                    deterministicProductions.push_back(newProd2);
                                }
                            }
                        }
                    }
                }
            }
        }
        if(flag==0){
            if(!exists(tempString)){
                deterministicProductions.push_back(tempString);
            }
        }
    }
    return 1;
}

int printNDGrammar(){
    cout<<"\n>>>FINAL NON DETERMINISTIC FREE GRAMMAR<<<"<<endl;
    for(int i=0;i<productions.size();i++){
        cout<<productions[i]<<endl;
    }
    return 1;
}

int main(){
    //Check For:
    //Unambiguous   -- undecidable
    //Left Recursion
    //Non Determinism

	cout<<"# => epsilon"<<endl;

    //Read All The Productions
    readProductions();
    int flag=0;

    //NON DETERMINISM CHECK BEGINS
    for(int i=0;i<nonTerminals.size();i++){
        if(isNonDeterministic(nonTerminals[i])){
            //cout<<"\nInside ND\n";
            rectifyNonDeterminism(nonTerminals[i]);
            clearProductions(nonTerminals[i]);
            flag=1;
        }else{
            //cout<<"\nInside Dete\n ";
            for(int j=0;j< productions.size();j++){
                if(productions[j][0]==nonTerminals[i]){
                    deterministicProductions.push_back(productions[j]);
                }
            }
        }
    }
    if(flag==0){
        cout<<"\n***Grammar Is Free Of Non Determinism***";
    }else{
        productions = deterministicProductions;
        printNDGrammar();
    }
    //NON DETERMINISM CHECK ENDS
    
    flag=0;

    //LEFT RECURSION CHECK BEGINS
    for(int i=0;i<nonTerminals.size();i++){
        if(isLeftRecursive(nonTerminals[i])){
            //Left Recursion Found
            //cout<<"Grammar Has Left Recursion In Production Of : "<<nonTerminals[i]<<endl;
            rectifyLeftRecursion(nonTerminals[i]);
            clearProductions(nonTerminals[i]);
            flag=1;
        }
    }
    if(flag==0){
        cout<<"\n***Grammar Is Free Of Left Recursion***"<<endl;
    }else{
       printFinalGrammar();
       productions = newProductions;
    }
    //LEFT RECURSION CHECK FINISH
    flag=0;
    return 1;
}
