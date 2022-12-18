#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

void validate(string,vector<char>);
string put(string,int *,vector<char>);
string str(int);
bool check(string);
vector<int> find_outer_oprt(string);
int find_outer_oprt2(string);
string dlt_outer_parantes(string);
void display_wrong(int *,bool,int);
string remove_negation(string);
string negation(string);
void expand_branch(string,int);
string dlt_par_btw(string s);
bool isleaf(string s);
string give_other(string s,int a);
bool isopen(string s);
void display();

vector<vector<string> > v; //store all branch info.
int branch_num;

int main(){
    string s;
    cout<<"Enter proposition:";
    cin>>s;
    s=remove_negation(s);

    vector<char> vars;
    cout<<"Enter variables in proposition or - to finish:";
    char c;
    while(true){
        cin>>c;
        if(c=='-') break;
        vars.push_back(c);
    }

    cout<<"\nlet's first validate with truth table:\n";

    auto start = high_resolution_clock::now();
    validate(s,vars);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
    cout <<"truth table take "<< duration.count() <<" microseconds." << endl;

    cout<<"\nknow we use tableaux algorithm.\nnegation of our prop:";
    s=negation(s);
    cout<<s<<endl;
    branch_num=0;
    cout<<"\nthe tableaux:\n";

    auto start2 = high_resolution_clock::now();
    expand_branch(s,0);
    display();
    auto stop2 = high_resolution_clock::now();
	auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout <<"\ntableaux algorithm take "<< duration2.count() <<" microseconds.\n" << endl;

    return 0;
}

void validate(string s,vector<char> vars){
    int n=vars.size();
    int *counter=new int [n];
    for(int i=0;i<n;i++) counter[i]=0;

    bool flag=true;
    int point=n-1;

    for(int i=0;i<vars.size();i++){
        cout<<"  "<<vars[i]<<"  "<<'|';
    }
    cout<<"||value\n";
    for(int i=0;i<vars.size();i++){
        cout<<"------";
    }
    cout<<"-------\n";
    //binary counter
    for(int i=0;i<pow(2,n);i++){
        if(!check(put(s,counter,vars))){
            display_wrong(counter,false,vars.size());
            flag=false;
        }
        else display_wrong(counter,true,vars.size());
        if(counter[point]==0) counter[point]++;
        else{
            while(counter[point]==1){
                counter[point]=0;
                point--;
            }
            counter[point]=1;
            point=n-1;
        }
    }
    if(flag) cout<<"this proposition is tautology."<<endl;
    else cout<<"this proposition isn't tautology.\n";
}

string put(string s,int *validmap,vector<char> vars){
    int i=0;
    while(i<s.size()){
        for(int j=0;j<vars.size();j++){
            if(vars[j]==s[i]){
                if(i!=0){
                    if(s[i-1]=='~') s=s.substr(0,i-1) + str((validmap[j]+1)%2) + s.substr(i+1,s.size()-i-1);
                    else s=s.substr(0,i) + str(validmap[j]) + s.substr(i+1,s.size()-i-1);
                }
                else s=s.substr(0,i) + str(validmap[j]) + s.substr(i+1,s.size()-i-1);
                i=-1;
                break;
            }
        }
        i++;
    }
    return s;
}

string str(int a){
    if(a==0) return "0";
    string dict="0123456789",re="";
    while(a!=0){
        re=dict[a%10] + re;
        a=a/10;
    }
    return re;
}

bool check(string s){
    s=dlt_outer_parantes(s);
    if(s=="1") return true;
    if(s=="0") return false;
    int oprt=find_outer_oprt2(s);
    if(s[oprt]=='^') if(check(s.substr(0,oprt)) && check(s.substr(oprt+1,s.size()-oprt-1))) return true;
    if(s[oprt]=='v') if(check(s.substr(0,oprt)) || check(s.substr(oprt+1,s.size()-oprt-1))) return true;
    if(s[oprt]=='>')if(!check(s.substr(0,oprt)) || check(s.substr(oprt+1,s.size()-oprt-1))) return true;
    return false;
}

int find_outer_oprt2(string s){
    int c=0;
    for(int i=0;i<s.size();i++){
        if(s[i]=='(') c++;
        if(s[i]==')') c--;
        if(s[i]=='>' && c==0) return i;
        if(s[i]=='^' && c==0) return i;;
        if(s[i]=='v' && c==0) return i;;
    }
    return find_outer_oprt2(dlt_outer_parantes(s));
}

string dlt_outer_parantes(string s){
    if(s[0]!='(') return s;
    int c=0;
    for(int i=0;i<s.size();i++){
        if(s[i]=='(') c++;
        if(s[i]==')') c--;
        if(c==0 && i!=s.size()-1) return s;
    }
    return dlt_outer_parantes(s.substr(1,s.size()-2));
}

void display_wrong(int *validmap,bool flg,int n){
    for(int i=0;i<n;i++){
        if(validmap[i]==1) cout<<"True |";
        else cout<<"False|";
    }
    cout<<"||";
    if(flg) cout<<"True";
    else cout<<"False";
    cout<<"\n";
}

string remove_negation(string s){
    string cur=s;
    string re="";
    int i=0,j,c;
    int mysize=s.size();
    while(i<mysize-1){
        if(s[i]=='~' && s[i+1]=='('){
            j=i+2;c=1;
            while(c!=0 && j<mysize){
                if(s[j]=='(') c++;
                if(s[j]==')') c--;
                if(c!=0) re+=s[j];
                if(c==0){
                    s=s.substr(0,i) + '(' +negation(re)+ ')' +s.substr(j+1,s.size()-j-1);
                    re="";
                    i=j+2;
                    mysize=s.size();
                }
                j++;
            }
        }
        if(s[i]=='~' && s[i+1]=='~'){
            s=s.substr(0,i)+s.substr(i+2,s.size()-i-2);
            mysize=s.size();
        }
        i++;
    }
    if(cur!=s) return remove_negation(s);
    return s;
}

string negation(string s){
    s = dlt_outer_parantes(s);
    if(s[0]=='~' && s.size()>2){
        int c=0;
        bool flg=true;
        for(int i=1;i<s.size();i++){
            if(s[i]=='(') c++;
            if(s[i]==')') c--;
            if(c==0 && i!=s.size()-1) flg=false;
        }
        if(flg) return s.substr(1,s.size()-1);
    }

    vector<int> v;
    v = find_outer_oprt(s);
    if(v.empty()){
        if(s.size()==1) return "~" + s;
        else return s.substr(1,1);
    }
    if(v.size()==1 && s[v[0]]=='>'){
        return '('+ s.substr(0,v[0]) + ')' + '^' +'('+ negation(s.substr(v[0]+1,s.size()-v[0]-1))+')';
    }

    int start=0,len=v[0];
    string re="";
    re+='(';
    for(int i=0;i<v.size();i++){
        if(s[v[i]]=='^')
            re+=negation(s.substr(start,len)) + 'v';
        else if(s[v[i]]=='v')
            re+=negation(s.substr(start,len)) + '^';
        start=v[i]+1;
        if(i+1<v.size()) len=v[i+1]-v[i]-1;
    }
    re+=negation(s.substr(start,s.size()-start)) + ')';
    return re;
}

vector<int> find_outer_oprt(string s){
    int c=0;
    vector<int> v;
    for(int i=0;i<s.size();i++){
        if(s[i]=='(') c++;
        if(s[i]==')') c--;
        if(s[i]=='>' && c==0){
            v.clear();
            v.push_back(i);
            return v;
        }
        if(s[i]=='^' && c==0) v.push_back(i);
        if(s[i]=='v' && c==0) v.push_back(i);
    }
    return v;
}

void expand_branch(string s,int father){
    int num=branch_num;
    vector<string> result;
    vector<int> out_oprt;
    bool flg=false;
    int dis_junc;
    result.push_back(str(num));
    result.push_back(str(father));
    result.push_back(s);
    string other="";
    string befor="",after="";
    while(!isleaf(s)){
        out_oprt = find_outer_oprt(s);
        for(int i=0;i<out_oprt.size();i++){
            if(s[out_oprt[i]]=='v'){
                dis_junc=out_oprt[i];
                flg=true;
                break;
            }

            else if(s[out_oprt[i]]=='^'){
                s[out_oprt[i]]=',';
                result.push_back(s);
            }

            else if(s[out_oprt[i]]=='>'){
                other=give_other(s,out_oprt[i]);
                befor="",after="";
                int j=out_oprt[i]-1;
                while(s[j]!=','){
                    befor=s[j] + befor;
                    j--;
                    if(j<0) break;
                }
                j=out_oprt[i]+1;
                while(s[j]!=','){
                    after+=s[j];
                    j++;
                    if(j>=s.size()) break;
                }
                s="("+negation(befor)+")v("+after+")"+","+other;
            }
        }
        if(flg) break;
        s=dlt_par_btw(s);
    }
    v.push_back(result);
    if(flg){
        befor="",after="";
        string s1;
        s1=give_other(s,dis_junc);
        int i=dis_junc-1;
        while(s[i]!=','){
            befor=s[i] + befor;
            i--;
            if(i<0) break;
        }
        i=dis_junc+1;
        while(s[i]!=','){
            after+=s[i];
            i++;
            if(i>=s.size()) break;
        }
        if(s1!=""){
            befor+=','+s1;
            after+=','+s1;
        }
        branch_num++;
        expand_branch(befor,num);
        branch_num++;
        expand_branch(after,num);
    }
}

string give_other(string s,int a){
    bool flg=false;
    for(int i=0;i<s.size();i++) if(s[i]==',') flg=true;
    if(flg==false) return "";
    int i=0;
    string re="",s1="";
    while(i<s.size()){
        s1+=s[i];
        if(s[i]==','){
            re+=s1;
            s1="";
        }
        if(i==a){
            while(s[i]!=','){
                i++;
                if(i==s.size()) break;
            }
            s1="";
        }
        i++;
    }
    if(s1=="") re=re.substr(0,re.size()-1);
    return re+s1;
}

bool isleaf(string s){
    for(int i=0;i<s.size();i++)
        if(s[i]=='^' || s[i]=='v' || s[i]=='>') return false;
    return true;
}

string dlt_par_btw(string s){
    vector<int> comma_pose;
    string re;
    for(int i=0;i<s.size();i++){
        if(s[i]==',') comma_pose.push_back(i);
    }
    int start=0,len;
    for(int i=0;i<comma_pose.size();i++){
        len=comma_pose[i]-start;
        re+=dlt_outer_parantes(s.substr(start,len))+',';
        start=comma_pose[i]+1;
    }
    re+=dlt_outer_parantes(s.substr(start,s.size()-start));
    return re;
}

void display(){
    bool flag=true;
    for(int i=0;i<v.size();i++){
        cout<<"branch num:"<<v[i][0]<<",branch father:"<<v[i][1]<<">>  ";
        for(int j=2;j<v[i].size();j++){
            if(j!=v[i].size()-1)
                cout<<dlt_par_btw(v[i][j])<<" -- ";
            else{
                cout<<dlt_par_btw(v[i][j]);
                if(isleaf(v[i][j])){
                    if(isopen(v[i][j])){
                        cout<<" o";
                        flag=false;
                    }
                    else cout<<" x";
                }
            }
        }
        cout<<endl;
    }
    if(flag) cout<<"\nthe proposition is tautology.";
    else cout<<"\nthe proposition isn't tautology.";
}


bool isopen(string s){
    string s1="";
    bool re=true;
    vector<string> v;
    for(int i=0;i<s.size();i++){
        if(s[i]!=',') s1+=s[i];
        else{
            v.push_back(dlt_outer_parantes(s1));
            s1="";
        }
    }
    v.push_back(dlt_outer_parantes(s1));

    for(int i=0;i<v.size();i++){
        for(int j=0;j<v.size();j++){
            if(v[i].size()==2){
                if(v[i][1]==v[j][0] && v[j].size()==1) re=false;
            }
            else{
                if(v[j].size()==2)
                    if(v[j][1]==v[i][0]) re=false;
            }
        }
    }
    return re;
}
