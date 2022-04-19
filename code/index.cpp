#include <iostream>
#include <string>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#define TestirajIzuzetak(izuzetak, tijelo) \
  try tijelo catch(izuzetak e) { std::cout << e.what() << std::endl; }
class ApstraktniStudent{
	private:
	std::string ime;
	std::string prezime;
	int broj_indeksa;
	int broj_polozenih_ispita;
	double prosjecna_ocjena;
	public:
	ApstraktniStudent(std::string ime, std::string prezime, int broj_indeksa):ime(ime), prezime(prezime), broj_indeksa(broj_indeksa), broj_polozenih_ispita(0), prosjecna_ocjena(5) {}
	std::string DajIme() const{
		return ime;
	}
	std::string DajPrezime() const{
		return prezime;
	}
	int DajBrojIndeksa() const{
		return broj_indeksa;
	}
	int DajBrojPolozenih() const{
		return broj_polozenih_ispita;
	}
	double DajProsjek() const{
		return prosjecna_ocjena;
	}
	void RegistrirajIspit(int ocjena){
		if(ocjena<5 || ocjena>10) throw std::domain_error("Neispravna ocjena");
		if(ocjena!=5){
			broj_polozenih_ispita++;
			prosjecna_ocjena=((prosjecna_ocjena*(broj_polozenih_ispita-1))+ocjena)/broj_polozenih_ispita;
		}
	}
	virtual ~ApstraktniStudent(){}
	void PonistiOcjene(){
		broj_polozenih_ispita=0;
        prosjecna_ocjena=5;		
	}
	virtual void IspisiPodatke() const=0;
	virtual ApstraktniStudent* DajKopiju() const=0;
};
class StudentBachelor: public ApstraktniStudent{
	public:
	StudentBachelor(std::string ime, std::string prezime, int broj_indeksa):ApstraktniStudent(ime, prezime,broj_indeksa){ 
	}
	void IspisiPodatke() const override{
		std::cout<<"Student bachelor studija "<<this->DajIme()<<" "<<this->DajPrezime()<<", sa brojem indeksa "<<this->DajBrojIndeksa()<<",\nima prosjek "<<this->DajProsjek()<<".\n"; 
	}
	ApstraktniStudent* DajKopiju() const override{return new StudentBachelor(*this);}
	
};
class StudentMaster: public ApstraktniStudent{
	private:
   int godina_zavrsetka;
   public:
   StudentMaster(std::string ime, std::string prezime, int broj_indeksa, int godina_zavrsetka):ApstraktniStudent(ime,prezime,broj_indeksa){ StudentMaster::godina_zavrsetka=godina_zavrsetka;}
   void IspisiPodatke() const override{
   	std::cout<<"Student master studija "<<this->DajIme()<<" "<<this->DajPrezime()<<", sa brojem indeksa "<<this->DajBrojIndeksa()<<",\nzavrsio bachelor studij godine "<<godina_zavrsetka<<",ima prosjek "<<this->DajProsjek()<<".\n";
   }
   ApstraktniStudent* DajKopiju() const override{return new StudentMaster(*this);}
	
};
class Fakultet{
	std::vector<ApstraktniStudent*> studenti;
	public:
	~Fakultet(){
		for(int i=0;i<studenti.size();i++){
			delete studenti[i];
		}
	}
	Fakultet(){}
	Fakultet(const Fakultet & f){
		studenti.resize(f.studenti.size());
		
		for(int i=0;i<f.studenti.size();i++){
			studenti[i]=f.studenti[i]->DajKopiju();
		}
	
		
	}
	Fakultet& operator=(const Fakultet& f){
		std::vector<ApstraktniStudent*>  s(f.studenti.size());
	
			for(int i=0;i<f.studenti.size();i++){
				s[i]=f.studenti[i]->DajKopiju();
			}
		
		
		
		for(int i=0;i<studenti.size();i++) delete studenti[i];
		studenti=s;
		return *this;
	}
	Fakultet(Fakultet &&f){
	
		std::swap(studenti,f.studenti);
	}
	Fakultet& operator =(Fakultet && f){
		for(int i=0;i<studenti.size();i++) delete studenti[i];
		studenti=f.studenti;
		f.studenti.resize(0);
		return *this;
	}
	void UpisiStudenta(std::string ime, std::string prezime, int broj_indeksa, int godina_zavrsetka){
		int i(0);
		for( i=0;i<studenti.size();i++){
			if(studenti[i]->DajBrojIndeksa()==broj_indeksa) throw std::logic_error("Student sa zadanim brojem indeksa vec postoji");
		}
		studenti.resize(studenti.size()+1);
		studenti[studenti.size()-1]=new StudentMaster(ime,prezime,broj_indeksa,godina_zavrsetka);
	}
	void UpisiStudenta(std::string ime, std::string prezime, int broj_indeksa){
		int i(0);
		for( i=0;i<studenti.size();i++){
				if(studenti[i]->DajBrojIndeksa()==broj_indeksa) throw std::logic_error("Student sa zadanim brojem indeksa vec postoji");
		}
		studenti.resize(studenti.size()+1);
		studenti[studenti.size()-1]=(new StudentBachelor(ime,prezime,broj_indeksa));
	}
	void ObrisiStudenta(int broj_indeksa){
		bool nema_ga(true);
		int i(0);
		for( i=0;i<studenti.size();i++){
			if(studenti[i]->DajBrojIndeksa()==broj_indeksa) {nema_ga=false; break;}
		}
		if(nema_ga){
			throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
		}
		else{
			delete studenti[i]; 
			
		studenti.erase(studenti.begin()+i);
		}
	}
	 ApstraktniStudent& operator[](int broj_indeksa) {
		bool nema_ga(true);
		int i(0);
		for( i=0;i<studenti.size();i++){
			if(studenti[i]->DajBrojIndeksa()==broj_indeksa) {nema_ga=false; break;}
		}
		if(nema_ga){
			throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
		}
		return *studenti[i];
	}
	const ApstraktniStudent& operator[](int broj_indeksa) const{
		bool nema_ga(true);
		int i(0);
		for( i=0;i<studenti.size();i++){
			if(studenti[i]->DajBrojIndeksa()==broj_indeksa) {nema_ga=false; break;}
		}
		if(nema_ga){
			throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
		}
		return *studenti[i];
	}
	void IspisiSveStudente() const{
		Fakultet pomocna(*this);
		std::sort(pomocna.studenti.begin(), pomocna.studenti.end(), [](ApstraktniStudent* s1, ApstraktniStudent* s2){
			if(s1->DajProsjek()==s2->DajProsjek()){
				return s1->DajBrojIndeksa()<s2->DajBrojIndeksa();
			}
			return s1->DajProsjek()>s2->DajProsjek();});
		for(int i=0;i<studenti.size();i++){
			pomocna.studenti[i]->IspisiPodatke();
		}
	}
	
};
int main ()
{
	Fakultet f1;
  f1.UpisiStudenta("Meho", "Mehic", 3412);
  f1.UpisiStudenta("Ibro", "Ibric", 3124, 2015);
  f1.ObrisiStudenta(3412);
  f1.IspisiSveStudente();
  TestirajIzuzetak(std::logic_error, { f1.ObrisiStudenta(6789); });
	return 0;
}
