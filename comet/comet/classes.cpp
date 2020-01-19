class planet
{
	private:
	// physical parametrs
	
		double mass;	// planet mass in kg
	
	// position and velocity
		
		double X,Y,Z;		// in au
		double VX,VY,VZ;	// in m/s
	
	public:
		planet(double m, double x, double y, double z, double vx, double vy, double vz)
		{
			mass = m;
			   X = x;
			   Y = y;
			   Z = z;
			  VX = vx;
			  VY = vy;
			  VZ = vz;
		}
		
		double get_mass()	{ return mass; }
		double get_X()		{ return X; }
		double get_Y()		{ return Y; }
		double get_Z()		{ return Z; }
		double get_VX()		{ return VX; }
		double get_VY()		{ return VY; }
		double get_VZ()		{ return VZ; }
		
		void set_mass(double x)		{ mass = x; }
		void set_X(double x)		{ X = x; }
		void set_Y(double x)		{ Y = x; }
		void set_Z(double x)		{ Z = x; }
		void set_VX(double x)		{ VX = x; }
		void set_VY(double x)		{ VY = x; }
		void set_VZ(double x)		{ VZ = x; }
	
};

class comet
{
	private:
	// physical parametrs
	
		double mass;	// comet mass in kg
	
	// position and velocity
		
		double X,Y,Z;		// in au
		double VX,VY,VZ;	// in m/s
		
	// non-gravitational acceleration parametrs
	
		double R,T,N;	// in au/d^2
	
	public:
		comet(double m, double x, double y, double z, double vx, double vy, double vz, double r, double t, double n)
		{
			mass = m;
			   X = x;
			   Y = y;
			   Z = z;
			  VX = vx;
			  VY = vy;
			  VZ = vz;
			   R = r;
			   T = t;
			   N = n;
		}
		
		double get_mass()	{ return mass; }
		double get_X()		{ return X; }
		double get_Y()		{ return Y; }
		double get_Z()		{ return Z; }
		double get_VX()		{ return VX; }
		double get_VY()		{ return VY; }
		double get_VZ()		{ return VZ; }
		double get_R()		{ return R; }
		double get_T()		{ return T; }
		double get_N()		{ return N; }
		
		void set_mass(double x)		{ mass = x; }
		void set_X(double x)		{ X = x; }
		void set_Y(double x)		{ Y = x; }
		void set_Z(double x)		{ Z = x; }
		void set_VX(double x)		{ VX = x; }
		void set_VY(double x)		{ VY = x; }
		void set_VZ(double x)		{ VZ = x; }
	
};
