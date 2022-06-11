#include <iostream>

using namespace std;

double gx1(double seq[4], double x[3])
{
	double  a = seq[0];		double x1 = x[0];
	double  b = seq[1];		double x2 = x[1];
	double  c = seq[2];		double x3 = x[2];
	double  d = seq[3];		
	return (1 / a) * (d - b * x2 - c * x3);
}

double gx2(double seq[4], double x[3])
{
	double  a = seq[0];		double x1 = x[0];
	double  b = seq[1];		double x2 = x[1];
	double  c = seq[2];		double x3 = x[2];
	double  d = seq[3];
	return (1 / b) * (d - a * x1 - c * x3);
}

double gx3(double seq[4], double x[3])
{
	double  a = seq[0];		double x1 = x[0];
	double  b = seq[1];		double x2 = x[1];
	double  c = seq[2];		double x3 = x[2];
	double  d = seq[3];
	return (1 / c) * (d - a * x1 - b * x2);
}

double gx4(double seq[4], double x[3])
{
	double  a = seq[0];		double x1 = x[0];
	double  b = seq[1];		double x2 = x[1];
	double  c = seq[2];		double x3 = x[2];
	double  d = seq[3];
	return (1 / c) * (d - a * x1 - b * x2);
}

void Jacobi(double eq[3][4])
{
	double x[3] = { 1,1,1 };
	double xtmp[3] = { 0 };
	int i=0;
	cout << "Jacobi迭代结果: \n" << endl;
	while (i++<42)
	{
		xtmp[0] = gx1(eq[0], x);
		xtmp[1] = gx2(eq[1], x);
		xtmp[2] = gx3(eq[2], x);
		x[0] = xtmp[0];
		x[1] = xtmp[1];
		x[2] = xtmp[2];
		cout << "x1: " << x[0] << "\tx2: " << x[1] << "\tx3: " << x[2] << endl;
	}
}

void Seidel(double eq[3][4])
{
	double x[3] = { 1,1,1 };
	double xtmp[3] = { 0 };
	int i = 0;
	cout << "Seidel迭代结果: \n" << endl;
	while (i++ < 24)
	{
		x[0] = gx1(eq[0], x);
		x[1] = gx2(eq[1], x);
		x[2] = gx3(eq[2], x);
		cout <<"x1: " << x[0] << "\tx2: " << x[1] << "\tx3: " << x[2] << endl;
	}
}

int main()
{
	double eq[3][4] = { {4,2,1,23},{2,5,-2,29},{1,-2,7,0} };
	Jacobi(eq);
	Seidel(eq);
}


	public double eigenvectors[][]; // used to be v[][].
	public double eigenvalues[];    // used to be d[].
	private double g, h;
	private int  n, nrot, sweeps;
	private final static double UNDERFLOW_LIMIT = 0.0000000000000000000000001; // Value used for threshold of underflow.

	/**
	 * Create the eigenvectors and eigenvalues that will be computed.
	 */
	public Jacobi(int n)
	{
		this.n = n;
		eigenvectors = new double[n][n];
		eigenvalues = new double[n];
	}
	/*
	    The comment below applies to C source, but I kept it in for historical reasons.
      Note: Numerical Recipes source uses a #define for function "rotate".
      On some compilers this may result in a nonfunctional jacobi
      (took me 3 days to find this out).
	    So please keep "rotate" as a function!
  */
    private void rotate(double a[][], int i, int j, int k, int l, double s, double tau)
    {
        // modifies class variables g and h.
	      g = a[i][j];
	      h = a[k][l];
	      a[i][j] = g - s *(h + g * tau);
	      a[k][l] = h + s *(g - h * tau);
    } /* rotate */
    public int nrot()
    { return nrot; }
    public int sweeps()

    { return sweeps; }
    public void compute(float [][] aa)
    {
          double [][] daa = new double[aa.length][aa.length];
          for (int i = 0; i < aa.length; i++)
                for (int j = 0; j < aa.length; j++)
                      daa[i][j] = aa[i][j];
          compute(daa);
    }
    /*
        Compute all eigenvalues and eigenvectors of a real symmetric matrix
        aa[N][N].
        eigenvalues (d) [N] returns the eigenvalues of a. eigenvalues (v)[N][N] is a matrix whose columns
        contain, on output, the normalized eigenvectors of a.
        From: Press et al., Numerical Recipes in C, 2nd ed., pp 467-
    */
    public void compute(double [][] aa)
    {
        nrot = sweeps = 0;
        double [][] a = new double[n][n];  // matrix of which you want the E-values.
        for (int ip = 0; ip < n; ip++)
	      {
	  	      for (int iq = 0; iq < n; iq++)
            {
                  eigenvectors[ip][iq] = 0;   // v[ip][iq] = 0
	  		          a[ip][iq] = aa[ip][iq];     // Copy aa
            }
            eigenvectors[ip][ip] = 1; // make v an identity matrix.
        }
	      /* Initialize b and d (eigenvalues) to the diagonals of a */
	      double [] b = new double[n]; double [] z = new double[n];
        for (int ip = 0; ip < n; ip++)
	      {
		        b[ip] = eigenvalues[ip] = a[ip][ip];
		        z[ip] = 0;
	      }
	      for (int i = 0; i < 100; i++)
	      {
		          double sm = 0;
              double thresh;

		          for (int ip = 0; ip < (n-1); ip++)
		          {
			                for (int iq = ip + 1; iq < n; iq++)
				                  sm += Math.abs(a[ip][iq]);
		          }
              if (sm == 0)
              {
                    sweeps = i;
                    return;
              }
              /*
			              Macintosh PowerPC and 68xxx does not underflow to zero.
                    IEEE compliant code:
                    if  (sm <= UNDERFLOW_LIMIT)
                    {
                          FlowInterface.write("Jacobi: return on underflow");
                          return;
                    }
		          */
              if (i < 3) thresh = 0.2 * sm /(n * n); /* on the first three sweeps */
		          else thresh = 0.0; /* the rest of the sweeps */
 		          for (int ip = 0; ip < (n-1); ip++)
		          {
			  for (int iq = ip + 1; iq < n; iq++)
			  {
				  // g is class variable.
          g = 100.0 * Math.abs(a[ip][iq]);
				  /* After 4 sweeps skip the rotation if the
			 	    off diagonal element is small */
				  if (i  > 3 && (Math.abs(eigenvalues[ip]) + g) == Math.abs(eigenvalues[ip])
			 	      && (Math.abs(eigenvalues[iq]) + g) == Math.abs(eigenvalues[iq]))
			 	        a[ip][iq] = 0;
				  else if (Math.abs(a[ip][iq]) > thresh)
				  {
				  	double t;

            // h is class variable.
            h = eigenvalues[iq] - eigenvalues[ip];
					  if  ((Math.abs(h) + g)  ==  Math.abs(h))
              t = (a[ip][iq]) / h;
					  else
				 	  {
				  		  double theta = 0.5 * h / a[ip][iq];
				  		  t = 1.0 / (Math.abs(theta) + Math.sqrt(1.0 + theta * theta));
				  		  if (theta < 0) t = -t;
				  	}
					  double c = 1.0 / Math.sqrt(1.0 + t*t);
					  double s = t *  c;
					  double tau = s / (1.0 + c);
					  h = t * a[ip][iq];
					  z[ip] -= h;
					  z[iq] += h;
					  eigenvalues[ip] -= h;
					  eigenvalues[iq] += h;
					  a[ip][iq] = 0;
					  for (int j = 0; j < ip-1; j++)
						  rotate(a, j, ip, j, iq, s, tau);
					  for (int j = ip + 1; j< iq - 1; j++)
						  rotate(a, ip, j, j, iq, s, tau);
					  for (int j = iq + 1; j < n; j++)
						  rotate(a, ip, j, iq, j, s, tau);
					  for (int j = 0; j < n; j++)
						  rotate(eigenvectors, j, ip, j, iq, s, tau);
					  nrot++;
				  }
			  }
		  }
		  for (int ip = 0; ip < n; ip++)
		  {
			 b[ip] += z[ip];
			 eigenvalues[ip] = b[ip];
			 z[ip] = 0;
		 }
        if (i >= 50)
            IJ.write("i >= "+i+" in jacobi");
    }  // for
  } /* jacobi */
  public void sort()
  /* Sort the eigenvalues and the corresponding eigenvectors */
  {
        /* Most likely, already ordered				   */
        if (eigenvalues.length > 2)   // 3 eigenvalues.
        {
                if (eigenvalues[0] < eigenvalues[2]) /* Largest eigenvalue first */
                {
                      /* swap eigenvalues */
                      double t = eigenvalues[0]; eigenvalues[0] = eigenvalues[2]; eigenvalues[2] = t;
                      /* swap eigenvectors components */
                      t = eigenvectors[0][0]; eigenvectors[0][0] = eigenvectors[0][1]; eigenvectors[0][1] = t;
                      t = eigenvectors[2][0]; eigenvectors[2][0] = eigenvectors[2][1]; eigenvectors[2][1] = t;
                }
        }
        if (eigenvalues[0] < eigenvalues[1]) // 2 or 3 eigenvalues
        {
                  /* swap eigenvalues */
                  double t = eigenvalues[0]; eigenvalues[0] = eigenvalues[1]; eigenvalues[1] = t;
                  /* swap eigenvectors components */
                  t = eigenvectors[0][0]; eigenvectors[0][0] = eigenvectors[0][1]; eigenvectors[0][1] = t;
                  t = eigenvectors[1][0]; eigenvectors[1][0] = eigenvectors[1][1]; eigenvectors[1][1] = t;
        }
        if (eigenvalues.length > 2)   // 3 eigenvalues.
        {

                if (eigenvalues[1] < eigenvalues[2]) /* Largest eigenvalue first */
                {
                      /* swap eigenvalues */
                      double t = eigenvalues[1]; eigenvalues[1] = eigenvalues[2]; eigenvalues[2] = t;
                      /* swap eigenvectors components */
                      t = eigenvectors[1][0]; eigenvectors[1][0] = eigenvectors[1][1]; eigenvectors[1][1] = t;
                      t = eigenvectors[2][0]; eigenvectors[2][0] = eigenvectors[2][1]; eigenvectors[2][1] = t;
                }
        }
   }
	public int check(double [][] mm)
    /*
          check eigenvalues, eigenvectors.
          returns 0 if ok.
    */
    {
        double diff1[], diff2[], length;

        diff1 = new double[n];
        diff2 = new double[n];
        /* Compute angle between two eigenvectors - should be orthogonal */
        double angle = Math.acos((eigenvectors[0][0]*eigenvectors[0][1]+eigenvectors[1][0]*eigenvectors[1][1])/
	        ( Math.sqrt(eigenvectors[0][0]*eigenvectors[0][0]+eigenvectors[1][0]*eigenvectors[1][0])*
	          Math.sqrt(eigenvectors[0][1]*eigenvectors[0][1]+eigenvectors[1][1]*eigenvectors[1][1])))*180.0/Math.PI;
        if (angle < 89.5 || angle > 90.5)
            return 1;

        /* eigenvectors test */
        diff1[0] = mm[0][0]*eigenvectors[0][0]+mm[0][1]*eigenvectors[1][0];
        diff1[1] = mm[1][0]*eigenvectors[0][0]+mm[1][1]*eigenvectors[1][0];
        diff1[0] = diff1[0] - eigenvalues[0]*eigenvectors[0][0];
        diff1[1] = diff1[1] - eigenvalues[0]*eigenvectors[1][0];
        if((length = Math.sqrt(diff1[0]*diff1[0]+diff1[1]*diff1[1])) > 0.1)
              return 2;
        diff2[0] = mm[0][0]*eigenvectors[0][1]+mm[0][1]*eigenvectors[1][1];
        diff2[1] = mm[1][0]*eigenvectors[0][1]+mm[1][1]*eigenvectors[1][1];
        diff2[0] = diff2[0] - eigenvalues[1]*eigenvectors[0][1];
        diff2[1] = diff2[1] - eigenvalues[1]*eigenvectors[1][1];
        if((length = Math.sqrt(diff2[0]*diff2[0]+diff2[1]*diff2[1])) > 0.1)
              return 3;
        if (nrot > 50)
              return 4;
        return 0;
  }
	public String toString()
	{
		StringBuffer sb = new StringBuffer();
		sb.append("eigenvalues:\teigenvectors:\n");
		for (int i = 0; i < n; i++)
		{
			sb.append(eigenvalues[i]);
			sb.append(" {");
			for (int j = 0; j < n; j++)
			{
				sb.append(eigenvectors[i][j]);
				sb.append(", ");
			}
			sb.append("} \n");
		}
		return sb.toString();
	}


	integral += (fxSimpsonExm(a) - fxSimpsonExm(b));
    double p = a;
    for(int k=1; k<=n; k++)
	{
		p += h/2;
		integral += 4*fxSimpsonExm(x);
		p += h/2;
		integral += 2*fxSimpsonExm(x);
	}
	integral=(integral*h)/6;



int main()
{
	double a, b, precision, h, x;//a,b为区间的端点，presicion为给定的积分精度，h是每个积分小区间的长度
	double S;
	var:Double T1, T2, S1, S2, C1, C2, R1, R2;
	int k;	//
	printf("请输入区间端点a，b\n");
	scanf("%lf%lf", &a, &b);
	printf("请输入精度\n");
	scanf("%lf", &precision);


var T1:Double= 
    var T2:Double
    var S1:Double
    var S2:Double
    var C1:Double
    var C2:Double
    var R1:Double
    var R2:Double


	h = b - a;
	T1 = h / 2.0 * (f(a) + f(b));
	k = 1;
	while (true) {
		x = a + h / 2.0;
		while (1) {	//计算从i = 1到i = n-1的f（xi）的和，并且用S存储
			S = S + f(x);
			x = x + h;
			if (x >= b)	//若已超过区间右端点
				break;
		}
		T2 = T1 / 2.0 + h / 2.0 * S;
		S2 = T2 + 1.0 / 3.0 * (T2 - T1);
		if (k == 1) {
			k++;
			h = h / 2.0;
			T1 = T2;
			S1 = S2;
			continue;
		}
		C2 = S2 + 1.0 / 15.0 * (S2 - S1);
		if (k == 2) {
			k++;
			h = h / 2.0;
			C1 = C2;
			T1 = T2;
			S1 = S2;
			continue;
		}
		R2 = C2 + 1.0 / 63.0 * (C2 - C1);
		if (k == 3) {
			k++;
			h = h / 2.0;
			R1 = R2;
			C1 = C2;
			T1 = T2;
			S1 = S2;
			continue;
		}
		if (fabs(R2 - R1) >= precision) {
			k++;
			h = h / 2.0;
			R1 = R2;
			C1 = C2;
			T1 = T2;
			S1 = S2;
			continue;
		}
		break;
	}
	printf("精度为%lf的积分结果为：%.9lf\n", precision, R2);
}



    var T= mutableListOf(0.0)
    var S= mutableListOf(0.0)
    var C= mutableListOf(0.0)
    var R= mutableListOf(0.0)

    T[1]=(fxRombergExm(left) + fxRombergExm(right)) * step / 2.0

    var k = 1
    var integral = 0.0

    while (true) {
        x = left + step / 2.0
        //计算从i = 1到i = n-1的f（xi）的和，并且用integral存储
        while (true) {
            integral  += fxRombergExm(x)
            x += step;
            if (x>right)	//若已超过区间右端点
                break;
        }
        T[2] = T[1] / 2 + step / 2 * integral;
        S[2] = T[2] + 1.0 / 3.0 * (T[2] - T[1]);
        if (k == 1) {
            k++;
            step /= 2.0
            T[1] = T[2]
            S[1] = S[2]
            continue;
        }
        C[2] = S[2] + 1.0 / 15.0 * (S[2] - S[1]);
        if (k == 2) {
            k++;
            step /= 2.0;
            C[1] = C[2];
            T[1] = T[2];
            S[1] = S[2];
            continue;
        }
        R[2] = C[2] + 1.0 / 63.0 * (C[2] - C[1]);
        if (k == 3) {
            k++;
            step /= 2.0;
            R[1] = R[2]
            C[1] = C[2]
            T[1] = T[2]
            S[1] = S[2]
            continue;
        }
        if (abs(R[2] - R[1]) >= ep) {
            k++
            step /= 2.0
            R[1] = R[2]
            C[1] = C[2]
            T[1] = T[2]
            S[1] = S[2]
            continue;
        }
        break;
    }


    var ep=eps+1
    var m=0
    while(ep>=eps){
        m += 1
        var t=0.0
        for (i in 0..(2.0.pow(m-1)-1).toInt()){
            t= (t+fxRombergExm(left+(2*(i+1)-1)*h/2.0.pow(m))*h/2.0.pow(m))
        }
        t += T[1] / 2
        T.add(t)
        if (m>=1){
            S.add((4.0.pow(m)*T[1]-T[2])/(4.0.pow(m)-1))
        }
        if (m>=2){
            C.add((4.0.pow(m)*S[1]-S[2])/(4.0.pow(m)-1))
        }
        if (m>=3){
            R.add((4.0.pow(m)*C[1]-C[2])/(4.0.pow(m)-1))
        }
        if (m>4){
            ep=abs(10*(R[1]-R[2]))
        }


	