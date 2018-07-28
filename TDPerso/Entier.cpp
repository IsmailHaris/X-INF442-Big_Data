class Entier { 

	private :
		int base, sgn, l; int *c;

	public :
		Entier() {}
		∼Entier() {}

		Entier::Entier(int n, int b){
			base = b;
			sgn = (0>n) ? -1 : 1;
			n *= sgn;
			l = (n==0) ? 1 : ceil(log(n+1)/log(b));
			c = new int[l];

			for (int i = 0; i < l; i++) {
				c[i] = n%b;
				n/=b;
			}
		}

		static Entier Conversion(int n, int b){
			return Entier (n,b);
		}

		static bool zero(){
			return l==1& & c[0]=0;
		}

		void moins(){
			sgn *= -1;
		}

		void print(){
			std::cout << "[′" << base << "]" << c[l-1];
			for (int i=l-2; i>=0; i--) {
				std::cout << "|" << c[i];
			}
			std::cout << std::endl;
		}


		int get_base() {
			return base ;
		}
		int get_sign() {
			return sgn ;
		}

		void send(int &tag, int root) {
			MPI_Send(&l, 1, MPI_INT, oot, tag, MPI_COMM_WORLD); tag++;
			MPI_Send(&sgn, 1, MPI_INT, root, tag, MPI_COMM_WORLD); tag++;
			MPI_Send(&c,l, MPI_INT, root, tag, MPI_COMM_WORLD); tag++;
		}

		static Entier receiveEntier(int origin, int &tag) {
			MPI_Status flag ;
			Entier e ;
			MPI_Recv(&(e.l), 1, MPI_INT, origin, tag, MPI_COMM_WORLD, &flag); tag++ ;
			MPI_Recv(&(e.sgn), 1, MPI_INT, origin, tag, MPI_COMM_WORLD, &flag); tag++ ;
			int tab[l]; e.c = tab;
			MPI_Recv(e.c, l, MPI_INT, origin, tag, MPI_COMM_WORLD, &flag); tag++ ;
			return e ;
		};

		static Entier PGCD(Entier p, Entier q) {
			Entier &a = p, &b = q ;
			if (p.get base() != q.get base()) {throw ;}
			if (a.get sign() == -1) {a.moins() ;}
			if (b.get sign() == -1) {b.moins() ;}
			while (!zero(a) && !zero(b)) {
				if (b < a) {a = a % b ;}
				else {b = b % a ;}
			}
		    return (zero(a) ? b : a);
		}

		// Surcharge d'opérateur : on suppose que les foncttions sont déjà fournies
		friend Entier operator+(Entier lhs, const Entier &rhs) {return addition(lhs,rhs) ;}
		friend Entier operator-(Entier lhs, const Entier &rhs) {return soustraction(lhs,rhs) ;}
		friend Entier operator*(Entier lhs, const Entier &rhs) {return multiplication(lhs,rhs) ;}
		friend Entier operator/(Entier lhs, const Entier &rhs) {return divisionQuotient(lhs,rhs) ;}
		friend Entier operator%(Entier lhs, const Entier &rhs) {return divisionReste(lhs,rhs) ;}
		friend bool operator<(Entier lhs, const Entier &rhs) {
			Entier dif = lhs - rhs;
			return (dif.sgn == 1) && (dif.l > 1 || dif.c[0] != 0) ;
		}
		friend std::ostream& operator<<(std::ostream& flux, const Entier &rhs){
			return divisionReste(lhs,rhs) ;
		}

};


class Fraction {
	private :
		int b ;
		Entier Num, Den ;

	public :
		Fraction() {}
		∼Fraction() {}

		Fraction(int base, int num, int den) {
			b = base;
			Num = Entier::Conversion((den > 0) ? num : -num, base):
			Den = Entier::Conversion((den > 0) ? den : -den, base);
		}

		void add(Fraction &f){
			if (b != f.Num.get_base()) {throw ;}
			Entier num = Num*f.Den + Den*f.Num
			Entier den = Den*f.Den;
			Fraction simple = Simplification(num, den);
			Num = simpl.Num;
			Den = simpl.Den;
		}

		void inverse() {
			if(Num.get sign() == -1) {
				Num.moins(); 
				Den.moins() ;
			}
			Entier n = Num;
			Num = Den;
			Den = n;
		}

		static Fraction Simplification(Entier &p, Entier &q) {
			if (p.get base() != q.get base()) {throw ;}
			Entier gcd = PGCD(p,q) ;
			Fraction frac ;
			frac.b = p.get base() ;
			frac.Num=p/gcd;
			frac.Den=q/gcd;
			return frac ;
		}

		static Fraction Harmonique(int k){
			if (k==0) return Fraction(10,0,1);
			Fraction res(10,1,1);
			for(int i=2; i<=k; i++){
				res.add(Fraction(10,0,i));
			}
			return res;
		}

		static Fraction HarmoniqueMPI(int a) {
			int size, rank; int root = 0;
			MPI_Comm_size(MPI COMM WORLD, &size) ;
			MPI_Comm_rank(MPI COMM WORLD, &rank) ;
			Fraction frac = Fraction(10,0,1) ;
			
			for (int j=0 ; j<a ; j++) frac.add(Fraction(10,1,rank+1+j*size));
			
			if (rank == root) {
				for (int i = 1; i < size; i++) {
					int tag = 0 ;
					Entier num = Entier::receiveEntier(i,tag) ;
					Entier den = Entier::receiveEntier(i,tag) ;
					Fraction other = Simplification(num,den) ;
					frac.add(other) ;
				}
			}else {
				int tag = 0; 
				frac.Num.send(tag,root);
				frac.Den.send(tag,root);
			}

			return frac ;
		}


		friend bool operator<(Fraction lhs, const Fraction &rhs) {
			return (lhs.Num * rhs.Den) < (rhs.Num * lhs.Den) ; 
		}
		friend bool operator+(Fraction lhs, const Fraction &rhs) {
			Entier p = lhs.Num;
			Entier q = lhs.Den;
			Fraction frac = Simplification(p,q);
			frac.add(rhs);
			return frac;
		}

}

class FractionContinue{

	public:
		FractionContinue();
		~FractionContinue();

		Fraction evalue(){
			Entier un = Entier::Conversion(1,10) ;
			Fraction frac = Fraction::Simplification(coefficient,un) ;
			if (reste != 0) {
			    Fraction other = reste->evalue();
			    other.inverse();
				frac.add(other);
			}
			return frac ;
		}

	private:
		Entier coefficient;
		FractionContinue* reste;
}
