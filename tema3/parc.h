#ifndef PARC_H_INCLUDED
#define PARC_H_INCLUDED
#include <bits/stdc++.h>
#include <exception>
using namespace std;
class LungimeGresita:exception
{
public:
    const char* what(){return "lungimea nu corespunde cu tipul masinii\n";}
};
class Masina{
protected:
    string nume;
    int pret;
    float lungime;
    int litraj;
public:
    Masina(string n="",float l=0, int li=0,int p=0):nume(n),lungime(l),litraj(li),pret(p){}
    static Masina* create(float l,float li);
    virtual string masina()=0;
    virtual ~Masina()=0;
    virtual void afisare(ostream&);
    virtual void citire(istream&);
    friend ostream & operator <<(ostream & out,Masina& ob);
    friend istream & operator >>(istream & in,Masina& ob);
    virtual void check()=0;
    string get_name() const{return nume;}
    int get_pret() const{return pret;}
    bool operator ==(const Masina& a){return nume==a.nume;}
};
class Mini: public Masina{
public:
    Mini(string nume="",float l=0, int li=0):Masina(nume,l,li){}
    string masina(){return "mini";}
    void check(){if(lungime>4){LungimeGresita a;throw a;}}
};
class Mica: public Masina{
public:
    Mica(string nume="",float l=0, int li=0):Masina(nume,l,li){}
    string masina(){return "mica";}
    void check(){if(lungime<3.85||lungime>4.1){LungimeGresita a;throw a;}}
};
class Compacta: public Masina{
    string tip;
public:
    Compacta(string nume="",float l=0, int li=0,string t=""):Masina(nume,l,li),tip(t){}
    string masina(){return "compacta";}
    void citire(istream&);
    void afisare(ostream&);
    void check(){if(lungime<4.2||lungime>4.5){LungimeGresita a;throw a;}}
};
class Monovolum: public Masina{
    unsigned vechime;
public:
    Monovolum(string nume="",float l=0, int li=0,unsigned v=0):Masina(nume,l,li),vechime(v){}
    void discount(unsigned);
    void citire(istream&);
    void afisare(ostream&);
    void check(){}
    bool sh(){return vechime;}
    string masina(){return "monovolum";}
};
template <class T>
class Vanzare{
    unsigned nr_stoc,nr_vandute;
    vector<pair<T*,bool>> *stoc = new vector<pair<T*,bool>>,*vandute = new vector<pair<T*,bool>>;
public:
    Vanzare():nr_stoc(0),nr_vandute(0){}
    void vanzare(T* a)
    {
        auto it = stoc->begin();
        while(it != stoc->end())
            if(*it->first==*a){
                vandute->push_back(*it);
                it = stoc->erase(it);
                nr_stoc--;
                nr_vandute++;
                break;
            }
            else
                it++;
    }
    void actualizare(T* a){
        stoc->push_back(make_pair(a,true));
        nr_stoc++;
    }
    void afis_stoc()
    {
        for(auto it:*stoc)
            if(Mini *a = dynamic_cast<Mini*>(it.first))
                cout<<*a;
            else if(Mica *a = dynamic_cast<Mica*>(it.first))
                cout<<*a;
            else if(Compacta *a = dynamic_cast<Compacta*>(it.first))
                cout<<*a;
    }
    void afis_vandute()
    {
        for(auto it:*vandute)
                if(Mini *a = dynamic_cast<Mini*>(it.first))
                    cout<<*a;
                else if(Mica *a = dynamic_cast<Mica*>(it.first))
                    cout<<*a;
                else if(Compacta *a = dynamic_cast<Compacta*>(it.first))
                    cout<<*a;
    }
};
template<>
class Vanzare<Monovolum>{
    unsigned nr_stoc,nr_vandute;
public:
    vector<pair<Monovolum,bool>> *stoc = new vector<pair<Monovolum,bool>>,*vandute = new vector<pair<Monovolum,bool>>;
    Vanzare():nr_stoc(0),nr_vandute(0){}
    void vanzare(Monovolum a)
    {
        unsigned luna;
        cout<<"luna:";cin>>luna;
        auto it = stoc->begin();
        while(it != stoc->end())
            if(it->first==a){
                it->first.discount(luna);
                vandute->push_back(*it);
                it = stoc->erase(it);
                nr_stoc--;
                nr_vandute++;
                break;
            }
            else
                it++;
    }
    void actualizare(Monovolum& a){
        if(a.sh())
            stoc->push_back(make_pair(a,false));
        else
            stoc->push_back(make_pair(a,true));
        nr_stoc++;
    }
    void afis_stoc(){for(auto it:*stoc)cout<<it.first;}
    void afis_vandute(){for(auto it:*vandute)cout<<it.first;}
};
class User
{
    Vanzare<Masina> masini;
    Vanzare<Monovolum> mono;
    static User* instance;
    User(){};
    User(const User&) = delete;
    User& operator =(const User&) = delete;
public:
    static User* get_instance()
    {
        if(!instance)
            instance = new User;
        return instance;
    }
    void create()
    {
        int n;
        cin>>n;
        for(int i=0;i<n;i++){
            string tip;
            cout<<"tip:";cin>>tip;
            if(tip == "mini")
            {
                Masina *a = new Mini;
                cin>>*a;
                if(a->get_name()!= "")
                    masini.actualizare(a);
            }
            else if(tip == "mica")
            {
                Masina *a = new Mica;
                cin>>*a;
                if(a->get_name()!= "")
                    masini.actualizare(a);
            }
            else if(tip == "compacta")
            {
                Masina *a = new Compacta;
                cin>>*a;
                if(a->get_name()!= "")
                    masini.actualizare(a);
            }
            else if(tip == "monovolum")
            {
                Monovolum a;
                cin>>a;
                mono.actualizare(a);
            }
        }
    }
    void vanzare()
    {
        cout<<"VANZARE"<<endl;
        string tip,nume;
        cout<<"tip:";cin>>tip;
        cout<<"nume:";cin>>nume;
            if(tip == "mini")
            {
                Masina *a = new Mini(nume);
                masini.vanzare(a);
            }
            else if(tip == "mica")
            {
                Masina *a = new Mica(nume);
                masini.vanzare(a);
            }
            else if(tip == "compacta")
            {
                Masina *a = new Compacta(nume);
                masini.vanzare(a);
            }
            else if(tip == "monovolum")
            {
                Monovolum a(nume);
                mono.vanzare(a);
            }

    }
    void stoc()
    {
        cout<<"Masini:";masini.afis_stoc();
        cout<<endl<<"Monovolume:";mono.afis_stoc();
        cout<<endl<<"Masini vandute:";masini.afis_vandute();
        cout<<endl<<"Monovolume vandute:";mono.afis_vandute();
    }
};
#endif // PARC_H_INCLUDED
