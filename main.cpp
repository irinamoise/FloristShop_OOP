#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace std;

class ArticolDuplicatException : public exception {
public:
    const char* what() const throw() {
        return "Articolul exista deja in lista!";
    }
};

class OptiuneInvalidaException: public runtime_error {
public:
    OptiuneInvalidaException(): runtime_error("Optiune invalida!\nVa rugam sa introduceti una dintre \noptiunile disponibile (afisate mai sus)") {}
};

class Interface{ /// Interfata pentru citire si afisare
public:
    virtual ostream& Afisare(ostream& out)const = 0;
    virtual istream& Citire(istream& in) = 0;
};

class Articol: public Interface { /// Clasa de baza (abstracta)
protected:
    string numeArticol;
    double pret;
    int nrBucati;
public:
    /// Constructori
    Articol();
    Articol(string, double, int);
    Articol(const Articol &);

    /// Operatorul =
    Articol &operator=(const Articol &);

    friend ostream &operator<<(ostream &, const Articol &);
    friend istream &operator>>(istream &, Articol &);

    istream &Citire(istream &in);
    ostream &Afisare(ostream &out) const;

    virtual ~Articol();

    virtual void Promotie() = 0; /// Funtia virtuala pura pentru abstractizarea clasei

    virtual double valTVA()const; /// Functie virtuala - cand am pointeri se poate apela varianta redef in cls derivata

    ///Setteri si getteri
    string getNumeArticol() const {return numeArticol;}

    double getPret() const {return pret;}

    int getNrBucati() const {return nrBucati;}

    void setNumeArticol(string &numeArticol) {this->numeArticol = numeArticol;}

    void setPret(double pret) {this->pret = pret;}

    void setNrBucati(int nrBucati) {this->nrBucati = nrBucati;}


};

Articol::Articol(): numeArticol(""), pret(0), nrBucati(0) {}

Articol::Articol(string numeArticol, double pret, int nrBucati): numeArticol(numeArticol),
                                                                 pret(pret), nrBucati(nrBucati) {}

Articol::Articol(const Articol &obj): numeArticol(obj.numeArticol), pret(obj.pret), nrBucati(obj.nrBucati) {}

Articol& Articol::operator=(const Articol &obj) {
    if (this != &obj){
        this->numeArticol = obj.numeArticol;
        this->pret = obj.pret;
        this->nrBucati = obj.nrBucati;
    }
    return *this;
}

ostream &operator<<(ostream &out, const Articol &obj) {
    return obj.Afisare(out);
}

istream &operator>>(istream &in, Articol &obj) {
    return obj.Citire(in);
}

istream& Articol::Citire(istream &in) {
    bool validData = false;
    while (!validData) { // Cat timp datele introduse sunt invalide, se repeta citirea
        try {
            cout << "Nume articol: ";
            getline(in, numeArticol);
            if (numeArticol.empty()) { // Se arunca exceptie daca numele articolului este gol
                throw invalid_argument("Numele articolului nu poate fi gol.");
            }

            cout << "Pret (lei): ";
            string tempPretStr;
            getline(in, tempPretStr); // Citirea pretului ca string
            try {
                double tempPret = stod(tempPretStr); // Se incearca conversia la double cu fct stod
                pret=tempPret;

            } catch (invalid_argument& e) { // Se arunca exceptie daca pretul nu este un numar
                throw invalid_argument("Pretul trebuie sa fie un numar.");
            } catch (out_of_range& e) { // Se arunca exceptie daca valoarea nu poate fi stocata intr-o variabila double
                throw invalid_argument("Valoare invalida (in afara domeniului de valori double)");}


            if (pret < 0) {
                throw invalid_argument("Pretul nu poate fi negativ.");
            }

            cout << "Numar bucati: ";
            string tempNrBucatiStr;
            getline(in, tempNrBucatiStr);
            try {
                int tempNrBucati = stoi(tempNrBucatiStr); // Se incearca conversia la int cu fct stoi(string to integer)

                nrBucati = tempNrBucati;
            } catch (invalid_argument& e) { //stoi aruncă excepție invalid_argument dacă șirul de caractere nu poate fi convertit într-un număr întreg.
                throw invalid_argument("Numarul de bucati trebuie sa fie un numar intreg.");
            }catch (out_of_range& e) { // stoi aruncă o excepție out_of_range dacă numărul este prea mare sau prea mic pentru a fi reprezentat ca un int.
                throw invalid_argument("Valoare invalida (in afara domeniului de valori intregi)");
            }

            if (nrBucati < 0) {
                throw invalid_argument("Numarul de bucati nu poate fi negativ.");
            }

            validData = true; // Daca toate datele sunt valide, se iese din while
        } catch (invalid_argument& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti datele.\n";
        }
        catch(...) { // Prinderea oricarei alte exceptii
            cout << "Eroare la citire. Va rugam sa reintroduceti datele.\n";
        }
    }
    return in;
}

ostream& Articol::Afisare(ostream &out) const {
    out << "Nume articol: " << numeArticol << endl;
    out << "Pret (lei): " << pret << endl;
    out << "Numar bucati: " << nrBucati << endl;
    return out;
}

Articol::~Articol() {}

double Articol::valTVA()const{
    return pret*0.19;
}


class Planta: virtual public Articol { /// Mostenire virtuala
protected:
    string caracteristici;
    string taraOrigine;
    string modIntretinere;
public:
    Planta();
    Planta(string, double, int, string, string, string);
    Planta(string, string, string); /// Constructor cu 3 param -> folosit in lista de init a constr lui floare si plantaLaGhiveci
    Planta(const Planta &);

    Planta &operator=(const Planta &);

    istream &Citire(istream &);
    ostream &Afisare(ostream &) const;


    ~Planta();

    void Promotie();

    void Import() const;

    Planta&  operator++(); /// Incrementarea stocului

};

Planta::Planta(): caracteristici(""), taraOrigine(""), modIntretinere("") {}
Planta::Planta(string nume, double pret, int nrBucati, string caracteristici, string taraOrigine, string modIntretinere):
        Articol(nume, pret, nrBucati), caracteristici(caracteristici), taraOrigine(taraOrigine), modIntretinere(modIntretinere) {}
Planta::Planta( string caracteristici, string taraOrigine, string modIntretinere):
        caracteristici(caracteristici), taraOrigine(taraOrigine),modIntretinere(modIntretinere) {}

Planta::Planta(const Planta &obj): Articol(obj){
    this->caracteristici = obj.caracteristici;
    this->taraOrigine = obj.taraOrigine;
    this->modIntretinere = obj.modIntretinere;
}

Planta& Planta::operator=(const Planta &obj) {
    if (this != &obj){
        Articol::operator=(obj);
        this->caracteristici = obj.caracteristici;
        this->taraOrigine = obj.taraOrigine;
        this->modIntretinere = obj.modIntretinere;
    }
    return *this;
}

istream& Planta::Citire(istream &in) {
    Articol::Citire(in);

    bool validData = false;
    while (!validData) {

        try {
            cout << "Caracteristici: ";
            getline(in, caracteristici);
            if (caracteristici.empty()) { // Se arunca exceptie daca caracteristicile sunt goale
                throw invalid_argument("Caracteristicile nu pot fi goale.");
            }

            cout << "Tara de origine: ";
            getline(in, taraOrigine);
            cout << "Mod de intretinere: ";
            getline(in, modIntretinere);
            validData = true; // Daca toate datele sunt valide, se iese din while
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti datele.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti datele.\n";
        }
    }
    return in;
}

ostream& Planta::Afisare(ostream &out) const {
    Articol::Afisare(out);
    out << "Caracteristici: " << caracteristici << endl;
    out << "Tara de origine: " << taraOrigine << endl;
    out << "Mod de intretinere: " << modIntretinere << endl;
    return out;
}


Planta::~Planta() {}

void Planta::Promotie() {
    cout<<"La achizitionarea a doua plante, se va aplica un discount de 10%!"; /// Implementare metoda virtuala pura
}

void Planta::Import() const /// Metoda cononst - nu modifica obiectul (this) (aici -> atributele numeArticol sau taraOrigine)
{
    string tara = "Romania";
    if(taraOrigine != tara)
        cout<<numeArticol<<" - importat din "<<taraOrigine<<endl;
}

Planta& Planta::operator++() {
    this->nrBucati++;
    return *this;
}

class Floare: public Planta { /// Clasa tip nepot : articol - planta - floare
private:
    set<int> zileAprovizionare; // 1-28
    int durataViata;
public:
    Floare();
    Floare(string, double, int, string, string, string, set<int>, int);
    Floare(const Floare &);

    Floare &operator=(const Floare &);

    istream &Citire(istream &);
    ostream &Afisare(ostream &) const;

    ~Floare();

    void Promotie();
    friend void creeazaBuchet();

};

Floare::Floare(): Articol(), Planta(), durataViata(0) {}
Floare::Floare(string numeArticol, double pret, int nrBucati, string caracteristici, string taraOrigine,
               string modIntretinere, set<int> zileAprovizionare ,  int durataViata): Articol(numeArticol, pret, nrBucati),
                                                                         Planta(caracteristici, taraOrigine, modIntretinere),
                                                                         zileAprovizionare(zileAprovizionare), durataViata(durataViata) {}

Floare::Floare(const Floare &obj): Articol(obj), Planta(obj), durataViata(obj.durataViata) {
    this->zileAprovizionare = obj.zileAprovizionare;
}

Floare& Floare::operator=(const Floare &obj) {
    if (this != &obj){
        Articol::operator=(obj);
        Planta::operator=(obj);
        this->zileAprovizionare = obj.zileAprovizionare;
        this->durataViata = obj.durataViata;
    }
    return *this;
}

istream& Floare::Citire(istream &in) {
    Planta::Citire(in);
    bool validData = false;
    while (!validData) {
        try {
            cout << "De cate ori pe luna se face aprovizionarea: ";
            string tempZileStr;
            getline(in, tempZileStr);
            try {
                int nrZile = stoi(tempZileStr); // Se incearca conversia la int cu fct stoi
                if(nrZile < 0) { // Se arunca exceptie daca numarul de zile este negativ
                    cout<<"";
                    throw invalid_argument("");
                }
                else if(nrZile > 28) {
                    cout<<"";
                    throw invalid_argument("");// Se arunca exceptie daca numarul e mai mare decat 28
                }
            } catch (invalid_argument &e) { // Se arunca exceptie daca numarul de zile nu este un numar
                throw invalid_argument("Numarul de zile trebuie sa fie un numar intreg, intre 1 si 28.");
            } catch (out_of_range &e) {
                throw invalid_argument("Valoare invalida (in afara domeniului de valori intregi)");
            }
            int nrZile = stoi(tempZileStr);

            cout << "Introduceti zilele (1-28):\n";
            for(int i = 0; i < nrZile; i++) {
                bool validZile = false;
                while(!validZile)
                {
                    cout<<"   - ";
                    string testZi;
                    in >> testZi;
                    try{
                        int zi = stoi(testZi);
                        if(zi < 1 || zi > 28) {
                            throw invalid_argument("Ziua trebuie sa fie intre 1 si 28");
                        }
                        zileAprovizionare.insert(zi);
                        validZile = true;
                    } catch (invalid_argument &e) { // Se arunca exceptie daca ziua nu este intre 1 si 28
                        cout << "Eroare la citire: " << e.what() << ". Va rugam sa reintroduceti data.\n";
                    } catch(out_of_range &e) {
                        cout << "Eroare la citire: " << e.what() << ". Va rugam sa reintroduceti data.\n";
                    } catch(...) {
                        cout << "Eroare la citire. Va rugam sa reintroduceti data.\n";
                    }

                }

            }

            cout << "Durata de viata (zile): ";
            string tempDurataViataStr;
            in.get();
            getline(in, tempDurataViataStr);
            try {
                int tempDurataViata = stoi(tempDurataViataStr); // Se incearca conversia la int cu fct stoi
                durataViata = tempDurataViata;
            } catch (invalid_argument &e) { // exceptie daca stoi nu poate converti stringul in int
                throw invalid_argument("Durata de viata trebuie sa fie un numar intreg.");
            } catch (out_of_range &e) { // exceptie daca valoarea nu poate fi stocata intr-un int
                throw invalid_argument("Valoare invalida (in afara domeniului de valori intregi)");
            }

            if(durataViata < 0) {
                throw invalid_argument("Durata de viata nu poate fi negativa.");
            }
            validData = true; // Daca toate datele sunt valide, se iese din while
        } catch (invalid_argument& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti datele.\n";
        }catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti datele.\n";
        }
    }
    return in;
}

ostream& Floare::Afisare(ostream &out) const {
    Planta::Afisare(out);
    out << "Zilele din luna in care se face aprovizionarea: ";
    for(auto zi : zileAprovizionare) {
        out << zi << " ";
    }

    out << endl<< "Durata de viata (zile): " << durataViata << endl;
    return out;
}


void Floare::Promotie() {
    cout<<"La achizitionarea a mai mult de 5 fire, se va aplica un discount de 20%!"; /// Implementare metoda virtuala pura
}

Floare::~Floare() {}

//------------------------------------------------------------------------------------------------------

template <class T>
class Comanda{
private:
    static int contor;
    const int idComanda;
    int nrSpecii;
    map<T*, int> continut;
    double pretComanda;
public:

    Comanda();
    Comanda(int, map<T*, int>, double);
    Comanda(const Comanda &);
    Comanda& operator=(const Comanda &);

    void adaugaArticol(T*, int);
    double getPretComanda() const;

    ostream& Afisare(ostream &) const;
    istream& Citire(istream &, list<Articol*>);

    template <class U> friend ostream& operator<<(ostream &, const Comanda<U> &);
    template <class U> friend istream& operator>>(istream &, Comanda<U> &);

    int getIdComanda() const {return idComanda;}

    ~Comanda() = default;


};

// Constructorul fara parametri
template <class T> Comanda<T>::Comanda(): idComanda(contor++),nrSpecii(0) , pretComanda(0) {}

// Constructorul cu toti parametrii
template <class T> Comanda<T>::Comanda(int nrSpecii, map<T*, int> continut, double pretComanda)
        : idComanda(contor++), nrSpecii(nrSpecii), continut(continut), pretComanda(pretComanda) {}

// Constructorul de copiere
template <class T> Comanda<T>::Comanda(const Comanda &obj): idComanda(obj.idComanda), nrSpecii(obj.nrSpecii),
                             continut(obj.continut), pretComanda(obj.pretComanda) {}

// Operatorul =
template <class T> Comanda<T>& Comanda<T>::operator=(const Comanda &obj) {
    if (this != &obj){
        this->idComanda = obj.idComanda;
        this->nrSpecii = obj.nrSpecii;
        this->continut = obj.continut;
        this->pretComanda = obj.pretComanda;
    }
    return *this;
}

// Initializarea variabilei statice
template <class T> int Comanda<T>::contor = 1;


// Metoda pentru adaugarea unui articol in comanda
template <class T> void Comanda<T>::adaugaArticol(T* articol, int nrBucati) {
    continut[articol] = nrBucati;
    pretComanda += articol->getPret() * nrBucati;
}


template <class T> double Comanda<T>::getPretComanda() const {
    return pretComanda;
}

template <class T> ostream& Comanda<T>::Afisare(ostream &out) const {
    out <<endl<< "Comanda " << idComanda << endl;
    for(auto& articol : continut) {
        out <<"   - "<<articol.first->getNumeArticol() << " x " << articol.second <<" bucati"<< endl;
    }
    out << "Pret comanda: " << pretComanda << endl;
    return out;
}

template <class T> istream& Comanda<T>:: Citire(istream &in, list<Articol*> articole) {
    bool validData = false;
    while(!validData) {

        try {
            cout << "Introduceti numarul de specii: ";
            string tempNrSpeciiStr;
            getline(in, tempNrSpeciiStr);
            try {
                int tempNrSpecii = stoi(tempNrSpeciiStr); // Se incearca conversia la int cu fct stoi
                nrSpecii = tempNrSpecii;
            } catch (invalid_argument &e) { // Se arunca exceptie daca numarul de specii nu este un numar
                throw invalid_argument("Numarul de specii trebuie sa fie un numar intreg.");
            } catch (out_of_range &e) {
                throw invalid_argument("Valoare invalida (in afara domeniului de valori intregi)");
            }

            if (nrSpecii <= 0) {
                throw invalid_argument("Numarul de specii trebuie sa fie mai mare decat 0.");
            }

            cout << "Introduceti numele speciilor dorite:\n";
            for (int i = 0; i < nrSpecii; i++) {
                cout  << "Specia " << i + 1 << ":" ;
                string numeArticol;
                getline(in, numeArticol);
                T *articol = NULL;
                for (auto &art: articole) {
                    if (art->getNumeArticol() == numeArticol) {
                        articol = dynamic_cast<T *>(art);
                        break;
                    }
                }

                if (articol) {
                    string tempNrBucatiStr;
                    cout << "Introduceti numarul de bucati pentru " << numeArticol << ": ";
                    getline(in, tempNrBucatiStr);
                    try {
                        int tempNrBucati = stoi(tempNrBucatiStr); // Se incearca conversia la int cu fct stoi
                        if (tempNrBucati <= 0) {
                            throw invalid_argument("Numarul de bucati trebuie sa fie mai mare decat 0.");
                        }
                        else if (tempNrBucati > articol->getNrBucati()) {
                            throw invalid_argument("Stoc insuficient.");
                        }
                        adaugaArticol(articol, tempNrBucati);
                    } catch (invalid_argument &e) { // Se arunca exceptie daca numarul de bucati nu este un numar
                        throw invalid_argument(e.what());
                    } catch (out_of_range &e) {
                        throw invalid_argument("Valoare invalida (in afara domeniului de valori intregi)");
                    }
                } else {
                    throw invalid_argument("Articolul " + numeArticol + " nu a fost gasit.");
                }

            }
            validData = true; // Daca toate datele sunt valide, se iese din while
        } catch (const invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti datele.\n";
        } catch (const bad_alloc &e) {
            cout << "Eroare la alocarea memoriei: " << e.what() << " Va rugam sa reintroduceti datele.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti datele.\n";
        }
    }
    cout << "Comanda a fost creata cu succes.\n";
    return in;
}


/// Supraincarcarea operatorilor de citire si afisare
template <class T> ostream& operator<<(ostream &out, const Comanda<T> &comanda) {
    return comanda.Afisare(out);
}

template <class T> istream& operator>>(istream &in, Comanda<T> &comanda) {
    return comanda.Citire(in);
}


//------------------------------------------------------------------------------------------------------

//Singleton
class AdminFlorarie{
private:
    static AdminFlorarie* instance;
    list<Articol*> articole; /// Implementare container STL list
    vector<Comanda<Planta>*> comenziPlante; /// Implementare container STL vector
    vector<Comanda<Floare>*> comenziFlori;
    AdminFlorarie() {}
public:
    static AdminFlorarie* getInstance() {
        if (!instance)
            instance = new AdminFlorarie;
        return instance;
    }

    void adaugaArticol(Articol*);
    void adaugaComandaPlante(Comanda<Planta>*);
    void adaugaComandaFlori(Comanda<Floare>*);

    Articol* findArticol(string&);
    void updateArticol(string&);
    void deleteArticol(string&);

    void creeazaComandaPlante();
    void creeazaComandaFlori();
    void afiseazaComenziPlante();
    void afiseazaComenziFlori();
    void afiseazaArticole();

    double calculeazaVenituriPlante();
    double calculeazaVenituriFlori();

    void afiseazaPlanteStoc();
    void afiseazaFloriStoc();

    ~AdminFlorarie();



    void meniuArticole();
    void meniuComenzi();
    void meniuStocuri();
    void meniuIncasari();
    void meniuPrincipal();

};

void AdminFlorarie::adaugaArticol(Articol* articol) {
    try{
        if(articol == NULL) {
            throw invalid_argument("Ati incercat sa adaugati un articol inexistent.");
        }
        for(auto& articolExistent : articole) {
            if(articolExistent->getNumeArticol() == articol->getNumeArticol()) {
                throw ArticolDuplicatException();
            }
        }
        articole.push_back(articol);
    } catch(const invalid_argument& e) {
        cout << "Eroare la adaugarea articolului: " << e.what() << "\nVeti fi redirectionat catre meniu.\n";
    } catch(const ArticolDuplicatException& e) {
        cout << "Eroare la adaugarea articolului: " << e.what() << "\nVeti fi redirectionat catre meniu.\n";
    } catch(...) {
        cout << "Eroare la adaugarea articolului.\nVeti fi redirectionat catre meniu.\n";
    }

}

void AdminFlorarie::adaugaComandaPlante(Comanda<Planta>* comanda) {
    try{
        if(comanda == NULL) {
            throw invalid_argument("Ati incercat sa adaugati o comanda inexistenta.");
        }
        for(auto comandaExistent : comenziPlante) {
            if(comandaExistent->getIdComanda() == comanda->getIdComanda() ) {
                throw invalid_argument("Comanda a fost deja adaugata!");
            }
        }
        comenziPlante.push_back(comanda);
    } catch(const invalid_argument& e) {
        cout << "Eroare la adaugarea comenzii: " << e.what() << "\nVeti fi redirectionat catre meniu.\n";
    } catch(...) {
        cout << "Eroare la adaugarea comenzii.\nVeti fi redirectionat catre meniu.\n";
    }

}

void AdminFlorarie::adaugaComandaFlori(Comanda<Floare>* comanda) {
    try{
        if(comanda == NULL) {
            throw invalid_argument("Ati incercat sa adaugati o comanda inexistenta.");
        }
        for(auto comandaExistent : comenziFlori) {
            if(comandaExistent->getIdComanda() == comanda->getIdComanda() ) {
                throw invalid_argument("Comanda a fost deja adaugata!");
            }
        }
        comenziFlori.push_back(comanda);
    } catch(const invalid_argument& e) {
        cout << "Eroare la adaugarea comenzii: " << e.what() << " Veti fi redirectionat catre meniu.\n";
    } catch(...) {
        cout << "Eroare la adaugarea comenzii.\nVeti fi redirectionat catre meniu.\n";
    }
}

Articol* AdminFlorarie::findArticol(string& numeArticol) {
    for(auto& articol : articole) {
        if(articol->getNumeArticol() == numeArticol) {
            return articol;
        }
    }
    return NULL;
}

/// Metoda pentru a actualiza un articol
void AdminFlorarie::updateArticol( string& numeArticol) {
    Articol* articol = findArticol(numeArticol);
    if(articol) {
        string newPretStr;
        cout << "Introduceti noul pret: ";
        try{
            getline(cin, newPretStr);
            double newPret = stod(newPretStr);
            if(newPret < 0) {
                throw invalid_argument("Pretul nu poate fi negativ.");
            }
            articol->setPret(newPret);
            cout << "Articolul a fost actualizat." << endl;
        } catch(invalid_argument& e) {
            cout << "Eroare la citire: " << e.what() << " Veti fi redirectionat catre meniu.\n";
        } catch(out_of_range& e) {
            cout << "Eroare la citire: " << e.what() << " Veti fi redirectionat catre meniu.\n";
        } catch(...) {
            cout << "Eroare la citire.\nVeti fi redirectionat catre meniu.\n";
        }

    } else {
        cout << "Articolul nu a fost gasit.\nVeti fi redirectionat catre meniu/\n" << endl;
    }
}

/// Metoda pentru a șterge un articol
void AdminFlorarie::deleteArticol(string& numeArticol) {
    for(auto it = articole.begin(); it != articole.end(); ) {
        if((*it)->getNumeArticol() == numeArticol) {
            delete *it; // Eliberez memoria (destructorul este apelat automat)
            it = articole.erase(it); // Șterg pointerul din listă și actualizez iteratorul
            cout << "Articolul a fost sters." << endl;
            return; //incheie executia metodei
        } else {
            ++it;
        }
    }
    cout << "Articolul nu a fost gasit." << endl;

}

void AdminFlorarie::creeazaComandaPlante() {
    if(!articole.empty()) {
        Comanda<Planta>* comanda = new Comanda<Planta>;
        comanda->Citire(cin, articole);
        comenziPlante.push_back(comanda);
    } else {
        cout << "Nu exista plante in stoc.\nVa rugam sa adaugati plante inainte de a crea o comanda.\nVeti fi redirectionat catre meniu.\n\n";
    }

}

void AdminFlorarie::creeazaComandaFlori() {
    if(!articole.empty()) {
        Comanda<Floare>* comanda = new Comanda<Floare>;
        comanda->Citire(cin, articole);
        comenziFlori.push_back(comanda);
    } else {
        cout << "Nu exista flori in stoc.\nVa rugam sa adaugati flori inainte de a crea o comanda.\nVeti fi redirectionat catre meniu.\n\n";
    }
}

void AdminFlorarie::afiseazaComenziPlante() {
    if(comenziPlante.empty()) {
        cout << "Nu exista comenzi de plante.\n";
    } else {
        for(auto& comanda : comenziPlante) {
            cout << *comanda <<"-----------------------------------------"<< endl;
        }
    }
}

void AdminFlorarie::afiseazaComenziFlori() {
    if(comenziFlori.empty()) {
        cout << "Nu exista comenzi de flori.\n";
    } else {
        for(auto& comanda : comenziFlori) {
            cout << *comanda <<"-----------------------------------------"<< endl;
        }
    }
}

void AdminFlorarie::afiseazaArticole() {
    if(articole.empty()) {
        cout << "Nu exista articole in stoc.\n";
    }
    else{
        cout<<"\nArticolele din stoc sunt: \n\n";
        for(auto& articol : articole) {
            cout << *articol << "-----------------------------------------" << endl;
        }
    }

}

double AdminFlorarie::calculeazaVenituriPlante() {
    try{
        if(comenziPlante.empty()) {
            throw invalid_argument("Nu exista comenzi de plante.");
        }
        double venituri = 0;
        for(auto& comanda : comenziPlante) {
            venituri += comanda->getPretComanda();
            if (venituri > numeric_limits<double>::max()) {
                throw overflow_error("Valoarea totala a veniturilor depaseste domeniul double.\nVeti fi redirectionat catre meniu.");
            }
        }
        return venituri;
    }catch(const invalid_argument& e) {
        cout << "Eroare la calculul veniturilor: " << e.what() << endl;
        return 0;
    }catch(const overflow_error& e) {
            cout << "Eroare la calculul veniturilor: " << e.what() << "Veti fi redirectionat catre meniu.";
            return 0;
    }catch(...) {
        cout << "Eroare la calculul veniturilor.\nVeti fi redirectionat catre meniu.\n";
        return 0;
        }

}

double AdminFlorarie::calculeazaVenituriFlori() {
    try{
        if(comenziFlori.empty()) {
            throw invalid_argument("Nu exista comenzi de flori.");
        }
        double venituri = 0;
        for(auto& comanda : comenziFlori) {
            venituri += comanda->getPretComanda();
            if (venituri > numeric_limits<double>::max()) {
                throw overflow_error("Valoarea totala a veniturilor depaseste domeniul double.\nVeti fi redirectionat catre meniu.");
            }
        }
        return venituri;
    }catch(const invalid_argument& e) {
        cout << "Eroare la calculul veniturilor: " << e.what() << endl;
        return 0;
    }catch(const overflow_error& e) {
            cout << "Eroare la calculul veniturilor: " << e.what() << "Veti fi redirectionat catre meniu.";
            return 0;
    }catch(...) {
        cout << "Eroare la calculul veniturilor.\nVeti fi redirectionat catre meniu.\n";
        return 0;
        }
}

void AdminFlorarie::afiseazaPlanteStoc() {
    if(articole.empty()) {
        cout << "Nu exista plante in stoc.\n";
    } else {
        bool gasit = false;
        for(auto& articol : articole) {
            Planta* planta = dynamic_cast<Planta*>(articol);
            Floare* floare = dynamic_cast<Floare*>(articol);
            if(planta && !floare) {
                gasit = true;
                cout << *planta << "-----------------------------------------" << endl;
            }
        }
        if(!gasit) {
            cout << "Nu exista plante in stoc.\n";
        }
    }
}

void AdminFlorarie::afiseazaFloriStoc() {
    if(articole.empty()) {
        cout << "Nu exista flori in stoc.\n";

    }else{
        bool gasit = false;
        for(auto& articol : articole) {
            Floare* floare = dynamic_cast<Floare*>(articol);
            if(floare) {
                gasit = true;
                cout << *floare << "-----------------------------------------" << endl;
            }
        }
        if(!gasit) {
            cout << "Nu exista flori in stoc.\n";
        }
    }
}

void AdminFlorarie::meniuArticole() {
    string tempOptiune;
    int optiune;
    do{
        try {
            cout << "\n\nMeniu articole\n\n";
            cout << "1. Adauga planta\n";
            cout << "2. Adauga floare\n";
            cout << "3. Actualizeaza articol\n";
            cout << "4. Sterge articol\n";
            cout << "5. Inapoi\n";

            cout << "\nOptiunea dvs: ";
            getline(cin, tempOptiune);

            optiune = stoi(tempOptiune);

            switch (optiune) {
                case (1): {
                    cout << "\nIntroduceti datele pentru planta: \n";
                    Planta *planta = new Planta;
                    planta->Citire(cin);
                    adaugaArticol(planta);
                    break;
                }
                case (2): {
                    cout << "\nIntroduceti datele pentru floare: \n";
                    Floare *floare = new Floare;
                    floare->Citire(cin);
                    adaugaArticol(floare);
                    break;
                }
                case (3): {
                    string numeArticol;
                    cout << "Introduceti numele articolului pe care doriti sa il actualizati: ";
                    getline(cin, numeArticol);
                    updateArticol(numeArticol);
                    break;
                }
                case (4): {
                    string numeArticol;
                    cout << "Introduceti numele articolului pe care doriti sa il stergeti: ";
                    getline(cin, numeArticol);
                    deleteArticol(numeArticol);
                    break;
                }
                case (5): {
                    break;
                }
                default: {
                    throw OptiuneInvalidaException();
                }

            }
        }catch(OptiuneInvalidaException& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (out_of_range &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti optiunea.\n";
        }
    }while(optiune != 5);
}

void AdminFlorarie::meniuComenzi() {
    string tempOptiune;
    int optiune;
    do {
        try {
            cout << "\n\nMeniu comenzi\n\n";
            cout << "1. Creeaza comanda plante\n";
            cout << "2. Creeaza comanda flori\n";
            cout << "3. Afiseaza comenzi plante\n";
            cout << "4. Afiseaza comenzi flori\n";
            cout << "5. Inapoi\n";

            cout << "\nOptiunea dvs: ";
            getline(cin, tempOptiune);

            optiune = stoi(tempOptiune);

            switch (optiune) {
                case (1): {
                    creeazaComandaPlante();
                    break;
                }
                case (2): {
                    creeazaComandaFlori();
                    break;
                }
                case (3): {
                    afiseazaComenziPlante();
                    break;
                }
                case (4): {
                    afiseazaComenziFlori();
                    break;
                }
                case (5): {
                    break;
                }
                default: {
                    throw OptiuneInvalidaException();
                }

            }
        }catch(OptiuneInvalidaException& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (out_of_range &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti optiunea.\n";
        }
    }while(optiune != 5);

}

void AdminFlorarie::meniuStocuri() {
    string tempOptiune;
    int optiune;
    do{
        try {
            cout << "\n\nMeniu stocuri\n\n";
            cout << "1. Afiseaza toate articolele de pe stoc\n";
            cout << "2. Afiseaza plantele de pe stoc\n";
            cout << "3. Afiseaza florile de pe stoc\n";
            cout << "4. Inapoi\n";

            cout << "\nOptiunea dvs: ";
            getline(cin, tempOptiune);

            optiune = stoi(tempOptiune);

            switch (optiune) {
                case (1): {
                    afiseazaArticole();
                    break;
                }
                case (2): {
                    afiseazaPlanteStoc();
                    break;
                }
                case (3): {
                    afiseazaFloriStoc();
                    break;
                }
                case (4): {
                    break;
                }
                default: {
                    throw OptiuneInvalidaException();
                }

            }
        }catch(OptiuneInvalidaException& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (out_of_range &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti optiunea.\n";
        }
    }while(optiune != 4);
}

void AdminFlorarie::meniuIncasari() {
    string tempOptiune;
    int optiune;
    do{
        try {
            cout << "\n\nMeniu incasari\n\n";
            cout << "1. Calculeaza veniturile din vanzarea plantelor\n";
            cout << "2. Calculeaza veniturile din vanzarea florilor\n";
            cout << "3. Calculeaza veniturile totale\n";
            cout << "4. Inapoi\n";

            cout << "\nOptiunea dvs: ";
            getline(cin, tempOptiune);

            optiune = stoi(tempOptiune);

            switch (optiune) {
                case (1): {
                    double venituriPlante = calculeazaVenituriPlante();
                    cout << "Veniturile din vanzarea plantelor sunt: " << venituriPlante << endl;
                    break;
                }
                case (2): {
                    double venituriFlori = calculeazaVenituriFlori();
                    cout << "Veniturile din vanzarea florilor sunt: " << venituriFlori << endl;
                    break;
                }
                case (3): {
                    double venituriPlante = calculeazaVenituriPlante();
                    double venituriFlori = calculeazaVenituriFlori();
                    cout << "Veniturile totale sunt: " << venituriPlante + venituriFlori << endl;
                    break;
                }
                case (4): {
                    break;
                }
                default: {
                    throw OptiuneInvalidaException();
                }

            }
        }catch(OptiuneInvalidaException& e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (out_of_range &e) {
            cout << "Eroare la citire: " << e.what() << " Va rugam sa reintroduceti optiunea.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti optiunea.\n";
        }
    }while(optiune != 4);
}

void AdminFlorarie::meniuPrincipal() {
    string tempOptiune;
    int optiune;
    do{
        try {
            cout << "\n\nMeniu principal\n\n";
            cout << "1. Meniu articole\n";
            cout << "2. Meniu comenzi\n";
            cout << "3. Meniu stocuri\n";
            cout << "4. Meniu incasari\n";
            cout << "5. Iesire\n";

            cout << "\nOptiunea dvs: ";
            getline(cin, tempOptiune);

            optiune = stoi(tempOptiune);

            switch (optiune) {
                case (1): {
                    meniuArticole();
                    break;
                }
                case (2): {
                    meniuComenzi();
                    break;
                }
                case (3): {
                    meniuStocuri();
                    break;
                }
                case (4): {
                    meniuIncasari();
                    break;
                }
                case (5): {
                    break;
                }
                default: {
                    throw OptiuneInvalidaException();
                }

            }
        }catch(OptiuneInvalidaException& e) {
            cout << "Eroare la citire: " << e.what();
        } catch (invalid_argument &e) {
            cout << "Eroare la citire: " << e.what() << " .Valoarea introdusa trebuie sa fie un numar.\nVa rugam sa reintroduceti optiunea.\n";
        } catch (out_of_range &e) {
            cout << "Eroare la citire: " << e.what() << " .Valoare invalida!\nVa rugam sa reintroduceti optiunea.\n";
        } catch (...) {
            cout << "Eroare la citire. Va rugam sa reintroduceti optiunea.\n";
        }
    }while(optiune != 5);

}



AdminFlorarie::~AdminFlorarie() {
    for(auto& articol : articole) {
        delete articol;
    }
    for(auto& comanda : comenziPlante) {
        delete comanda;
    }
    for(auto& comanda : comenziFlori) {
        delete comanda;
    }
}

// Initializarea variabilei statice
AdminFlorarie* AdminFlorarie::instance = 0;


int main() {
    AdminFlorarie* admin = AdminFlorarie::getInstance();

    // Crearea obiectelor Planta
    Planta* planta1 = new Planta("Ficus", 50.0, 10, "Planta de interior", "India", "Udare moderata");
    Planta* planta2 = new Planta("Cactus", 30.0, 15, "Planta de desert", "Mexic", "Udare rara");
    Planta* planta3 = new Planta("Aloe", 120.0, 7, "Planta de interior", "Brazilia", "Udare o data pe saptamana");

// Crearea obiectelor Floare
    set<int> zileAprovizionare1 = {1, 7, 14, 21, 28};
    Floare* floare1 = new Floare("Trandafir", 10.0, 50, "Floare de gradina", "Bulgaria", "Ii place la umbra", zileAprovizionare1, 7);

    set<int> zileAprovizionare2 = {2, 9, 16, 23, 30};
    Floare* floare2 = new Floare("Lalea", 5.0, 100, "Floare de gradina", "Olanda", "Se schimba apa zilnic", zileAprovizionare2, 5);

    set<int> zileAprovizionare3 = {3, 10, 17, 24};
    Floare* floare3 = new Floare("Crin", 8.0, 80, "Floare de gradina", "Romania", "Ii place la soare", zileAprovizionare3, 6);


    // Adaugarea obiectelor in lista de articole
    admin->adaugaArticol(planta1);
    admin->adaugaArticol(planta2);
    admin->adaugaArticol(planta3);
    admin->adaugaArticol(floare1);
    admin->adaugaArticol(floare2);
    admin->adaugaArticol(floare3);

    // Crearea unor comenzi
    Comanda<Planta>* comandaPlante = new Comanda<Planta>;
    Comanda<Floare>* comandaFlori = new Comanda<Floare>;

    // Adaugarea articolelor in comenzi
    comandaPlante->adaugaArticol(planta1, 2);
    comandaPlante->adaugaArticol(planta2, 1);
    comandaPlante->adaugaArticol(planta3, 3);
    comandaFlori->adaugaArticol(floare1, 10);
    comandaFlori->adaugaArticol(floare2, 20);
    comandaFlori->adaugaArticol(floare3, 15);

    // Adaugarea comenzilor in listele de comenzi
    admin->adaugaComandaPlante(comandaPlante);
    admin->adaugaComandaFlori(comandaFlori);

    admin->meniuPrincipal();
    return 0;
}

