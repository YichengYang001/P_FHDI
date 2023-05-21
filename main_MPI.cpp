//-----------------------------------------------------------------
//Parallel Fractional Hot Deck Imputation(P-FHDI)
//
//A general-purpose, assumption-free imputation software for curing big-n (large instance) incomplete data sets
//
//Version: 1.0
//Last release date: Sep 10, 2019
//Developers: Yicheng Yang, Dr.Jae-Kwang Kim, Dr.In-Ho Cho (Iowa State University)
//Contact: icho@iastate.edu
//License: GPL >= 2
//Depend: Intel MPI module ver.15.0.2 through 19.4
//Repository: https://sites.google.com/site/ichoddcse2017/home/type-of-trainings
//References:
//Serial version R package FHDI, https://cran.r-project.org/packages=FHDI.
//Jongho Im, In Ho Cho, and Jaekwang Kim, 2018, The R Journal, Vol.10(1), 140-154. [https://journal.r-project.org/archive/2018/RJ-2018-020/index.html].
//-----------------------------------------------------------------

using namespace std;    //this is very important for cluster sys.

#include <mpi.h>
#include <iostream>
#include "matrix_utility_FHDI.cc" //for local matrix utilities
#include <limits> // For NaN 
#include <iostream> // For cout, cerr, endl, and flush 
#include <assert.h> // For assert
#include <algorithm> // For sort 
#include <string>		//For string array
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <strstream>
#include <iomanip>

//Fn===========================================================================
//matrix_utility_FHDI.cc-------------------------------------------------------
//Fn===========================================================================
//below local functions for avoiding error for other compilers 
int    fabs_FHDI(int x)    { if(x>=0)   {return x;} if(x<0)   {return x*-1;}   return x;}
double fabs_FHDI(double x) { if(x>=0.0) {return x;} if(x<0.0) {return x*-1.0;} return x;}

bool isnan_FHDI(double x) { return x!=x; } //added to avoid error regarding std::isnan


#include "StringStream_utility.cpp"
#include "ReadInput_FHDI_MPI.cc"
#include "CheckInputdata.cc"
#include "base_FHDI_MPI.cc"
#include "List_FHDI_MPI.cc"
#include "List_string_FHDI_MPI.cc"
#include "rbind_FHDI_MPI.cc"
#include "Rfn_test_MPI.cc" 




int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	//==============
	//MPI initial setting
	//==============
	int totalnodes, mynode;
	double d_begin_MPI, d_end_MPI, d_begin_MPI_temp; //checking time

	MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
	MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
	
	MPI_Barrier(MPI_COMM_WORLD);
	d_begin_MPI = MPI_Wtime();
	if(mynode == 0) d_begin_MPI_temp = MPI_Wtime();

	//debug
	/*MPI_Bcast(&d_begin_MPI_temp, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	cout << "beginning time difference at node " << mynode << " = " << d_begin_MPI - d_begin_MPI_temp << endl;*/
	
	//if (mynode == 0) {
	//	cout << "\n \n ======= run time check!! ========= \n \n";
	//	cout << "checking time precision =" << MPI_Wtick() << endl;
	//}

	//==============================
	//data input
	//==============================
	char buffer[256]; //storage for one line


	ifstream* pFileInput = new ifstream
		("./input.txt");

	//============
	//for Window
	//============   
	//ofstream TestOut("./debug.txt", ios::out);
	//ofstream TestOut1("./debug_FEFI.txt", ios::out);
	ofstream TestOut;
	ofstream TestOut_Slave1;
	ofstream TestOut_Slave2;
	ofstream TestOut_Slave3;

	if (mynode == 0) 
	{
		TestOut.open("./debug.txt", ios::out);

		TestOut_Slave1.open("./simp.data.txt", ios::out);

		TestOut_Slave2.open("./fimp.data.txt", ios::out);

		TestOut_Slave3.open("./summary.data.txt", ios::out);
	}

	if (pFileInput->bad()) //cf. opposite member fn= good()
	{
		TestOut << "Can't open input file" << endl;
		return 0;
	}

	//on P0 write on TestOut
	//on all others, write on cout with 
	
	int    i_option_read_data = 0;
	int    i_option_perform = 0;
	int    i_option_imputation = 0;
	int    i_option_variance = 0;
	int    i_option_merge = 0;
	int    nrow_x = 0;
	int	   ncol_x = 0; //pointers to integer sizes of x
	int    M = 0; //number of donors for FHDI
	int    i_user_defined_datz = 0; 
	int    i_option_collapsing = 0;//number of selected most correlated values
	int    i_option_SIS_type = 0;//number of selected most correlated values

	ReadInput_FHDI_MPI(pFileInput, i_option_read_data,
		i_option_perform, nrow_x,
		ncol_x, i_option_imputation,
		i_option_variance,
		i_option_merge,
		M,
		i_user_defined_datz, i_option_collapsing, i_option_SIS_type,
		TestOut);


	//cout << "Running time after ReadInput_FHDI_MPI at node " << mynode << " = " << MPI_Wtime() - d_begin_MPI << endl;

	double **x = New_dMatrix(nrow_x, ncol_x);          	//pointer to double vector x[col*row] that contains all data with missing units
	int    **r = New_iMatrix(nrow_x, ncol_x); 			//pointer to an integer vector r[n_total_x] that contains indices of 0 and 1
	double **z = New_dMatrix(nrow_x, ncol_x);			//not used as of April, 2017
	double *k = new double[ncol_x];			//category for each column 
	double *d = new double[nrow_x];  		//weight for each row 
	int    *id = new int[nrow_x]; 			//id of each row 
	int    *NonCollapsible_categorical = new int[ncol_x]; 

	double **x_temp = New_dMatrix(nrow_x, ncol_x);          	//pointer to double vector x[col*row] that contains all data with missing units
	int    **r_temp = New_iMatrix(nrow_x, ncol_x); 			//pointer to an integer vector r[n_total_x] that contains indices of 0 and 1
	double **z_temp = New_dMatrix(nrow_x, ncol_x);			//not used as of April, 2017
	double *k_temp = new double[ncol_x];			//category for each column 
	double *d_temp = new double[nrow_x];  		//weight for each row 
	int    *id_temp = new int[nrow_x]; 			//id of each row 
	int    *NonCollapsible_categorical_temp = new int[ncol_x];

	if ( (mynode == 0) && (i_option_read_data == 0)) {
		InputData(pFileInput,
			i_option_perform, nrow_x,
			ncol_x, i_option_imputation,
			i_option_variance,
			i_option_merge,
			i_user_defined_datz,

			x_temp, r_temp, z_temp, k_temp, d_temp, NonCollapsible_categorical_temp,
			TestOut);
	}

	if ((mynode == 0) && (i_option_read_data == 1)) {

		ifstream* pFileInput_daty = new ifstream
		("./daty.txt");
		ifstream* pFileInput_datr = new ifstream
		("./datr.txt");
		//ifstream* pFileInput_datz = new ifstream
		//("./datz.txt");

		if (pFileInput_daty->bad()) //cf. opposite member fn= good()
		{
			TestOut << "Can't open daty.txt file" << endl;
			return 0;
		}

		if (pFileInput_datr->bad()) //cf. opposite member fn= good()
		{
			TestOut << "Can't open datr.txt file" << endl;
			return 0;
		}

		InputData_Seperate(pFileInput, pFileInput_daty, pFileInput_datr,
			i_option_perform, nrow_x,
			ncol_x, i_option_imputation,
			i_option_variance,
			i_option_merge,
			i_user_defined_datz,

			x_temp, r_temp, z_temp, k_temp, d_temp, NonCollapsible_categorical_temp,
			TestOut);
	}

	for (int i = 0; i < nrow_x; i++) {
		id_temp[i] = i + 1;
	}

	MPI_Bcast(x_temp[0], nrow_x*ncol_x, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(r_temp[0], nrow_x*ncol_x, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(z_temp[0], nrow_x*ncol_x, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(k_temp, ncol_x, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(d_temp, nrow_x, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(id_temp, nrow_x, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(NonCollapsible_categorical_temp, ncol_x, MPI_INT, 0, MPI_COMM_WORLD);

	Copy_dMatrix(x_temp, nrow_x, ncol_x, x);
	Copy_iMatrix(r_temp, nrow_x, ncol_x, r);
	Copy_dMatrix(z_temp, nrow_x, ncol_x, z);
	Copy_dVector(k_temp, ncol_x, k);
	Copy_dVector(d_temp, nrow_x, d);
	Copy_iVector(id_temp, nrow_x, id);
	Copy_iVector(NonCollapsible_categorical_temp, ncol_x, NonCollapsible_categorical);
	

	//cout << "Running time after InputData at node " << mynode << " = " << MPI_Wtime() - d_begin_MPI << endl;

	//testout
	//TestOut << " Input reading has finished successfully ==================" << endl;
	//TestOut1 << "Mama" << endl;
	
	//----------------
	//Basic Error Check
	//as of April 6, 2017
	//----------------
	//M
	if (M < 1) { TestOut << "Error! M is less than 1 " << endl; return(0); }
	if (M > nrow_x) { TestOut << "Error! M is larger than total rows of data " << endl; return(0); }
	
	//k
	for (int i = 0; i<ncol_x; i++)
	{
		if (k[i] < 1) { TestOut << "Error! some k is less than 1 " << endl; return(0); }
		if (k[i] > 35) { TestOut << "Error! some k is larger than 35 " << endl; return(0); }
	}

	//NonCollapsible_categorical
	for (int i = 0; i < ncol_x; i++) {
		if (NonCollapsible_categorical[i] > 1) {
			TestOut<<"Error! the non-Collapsible category option should not be larger than 1"<<endl;
			return(0);
		}
	}

	//user-defined datz
	if (i_option_perform == 3) //CellProb only
	{
		if (i_user_defined_datz != 1) 
		{
			TestOut << "Error! for CellProb only option, must have i_user_defined_datz = 1 and [datz]" << endl; 
		}
	}
	if (i_option_perform == 4) //do all with uder-defined z 
	{
		if (i_user_defined_datz != 1)
		{
			TestOut << "Error! for i_option_perform=4 option, must have i_user_defined_datz = 1 and [datz]" << endl;
		}
	}

	if (i_option_perform > 4) { TestOut << "Error! No such option for i_option_perform" << endl; return(0); }
	if (i_option_imputation > 2) { TestOut << "Error! No such option for i_option_imputation" << endl; return(0); }
	if (i_option_merge > 1) { TestOut << "Error! No such option for i_option_merge" << endl; return(0); }
	if (i_user_defined_datz > 1) { TestOut << "Error! No such option for i_user_defined_datz" << endl; return(0); }
	if (i_option_collapsing > ncol_x) { TestOut << "Error! The number of selected most correlated values should not be larger than the total number of variables" << endl; return(0); }
	if (i_option_SIS_type!= 1 && i_option_SIS_type!=2 && i_option_SIS_type!=3) { TestOut << "Error! The SIS should be 1, 2 , or 3" << endl; return(0); }

	CheckInputdata(x,r,nrow_x,ncol_x, TestOut);
	//-------------
	//NA -> long number in x, i.e., daty
	//-------------
	for (int i = 0; i < nrow_x; i++)
		for (int j = 0; j < ncol_x; j++)
		{
			if (r[i][j] == 0) x[i][j] = 1234567899; 
		}


	//--------
	//prep return variables
	//--------
	rbind_FHDI  rbind_ipmat_FEFI_return(4 + ncol_x ); //column size is 4+ncol
	rbind_FHDI  rbind_Resp_FEFI_return(ncol_x  + 1);  //separate response matrix 
	rbind_FHDI  rbind_irmat_FEFI_return(5 + ncol_x ); //column size is 5+ncol    
	rbind_FHDI  rbind_ipmat_FHDI_return(4 + ncol_x ); //column size is 4+ncol
	rbind_FHDI  rbind_Resp_FHDI_return(ncol_x  + 1);  //separate response matrix 
	rbind_FHDI  rbind_irmat_FHDI_return(5 + ncol_x ); //column size is 5+ncol    
	rbind_FHDI  rbind_vrst_FEFI_return(nrow_x );    //variance estimates of FEFI
	rbind_FHDI  rbind_vrst_FHDI_return(nrow_x );    //variance estimates of FHDI

													  //below is for output for Cell Make only option
	rbind_FHDI  rbind_uox_return(ncol_x ); //unique observed patterns
	rbind_FHDI  rbind_mox_return(ncol_x ); //unique observed patterns
	rbind_FHDI  rbind_category_return(ncol_x ); //cagetorized matrix 

												  //below is for output for Cell Prob only option
	std::vector<std::string> jp_name_return_CellProb;   //name of the joint probability table
	std::vector<double> jp_prob_return_CellProb; //the latest joint probability 	

	int i_op_p_temp = 1;
	if (i_option_perform  == 4) { i_op_p_temp = 4; }

	//-------------
	//Non-collapsible case consideration
	//-------------
	//int* NonCollapsible_categorical = new int[ncol_x]; 
	//Fill_iVector(NonCollapsible_categorical, ncol_x, 0); //default setting all zero. To be activated later
	//cout<<"Begin of Rfn_test-----------------------------"<<endl;
	Rfn_test_MPI(x, r, nrow_x, ncol_x, k, d, M,
		i_option_imputation, i_option_variance, i_option_collapsing, i_option_SIS_type,
		id, z,
		NonCollapsible_categorical,
		rbind_ipmat_FEFI_return, rbind_Resp_FEFI_return, rbind_irmat_FEFI_return,
		rbind_ipmat_FHDI_return, rbind_Resp_FHDI_return, rbind_irmat_FHDI_return,
		rbind_vrst_FEFI_return, rbind_vrst_FHDI_return,

		rbind_uox_return, rbind_mox_return, rbind_category_return,

		jp_name_return_CellProb, jp_prob_return_CellProb,

		i_option_perform,
		i_option_merge,
		TestOut, TestOut_Slave1,TestOut_Slave2, TestOut_Slave3, d_begin_MPI);

	//-----------------------
	//Deallocation
	//-----------------------
	Del_dMatrix(x, nrow_x, ncol_x);          	//pointer to double vector x[col*row] that contains all data with missing units
	Del_iMatrix(r, nrow_x, ncol_x); 			//pointer to an integer vector r[n_total_x] that contains indices of 0 and 1
	Del_dMatrix(z, nrow_x, ncol_x);			//not used as of April, 2017
	delete[] k;
	delete[] d;
	delete[] id;
	delete[] NonCollapsible_categorical;

	Del_dMatrix(x_temp, nrow_x, ncol_x);          	//pointer to double vector x[col*row] that contains all data with missing units
	Del_iMatrix(r_temp, nrow_x, ncol_x); 			//pointer to an integer vector r[n_total_x] that contains indices of 0 and 1
	Del_dMatrix(z_temp, nrow_x, ncol_x);			//not used as of April, 2017
	delete[] k_temp;
	delete[] d_temp;
	delete[] id_temp;
	delete[] NonCollapsible_categorical_temp;

	//system("PAUSE");
	//MPI_Barrier(MPI_COMM_WORLD);
	d_end_MPI = MPI_Wtime();
	//if (mynode == 0) cout << "YYC Total running time = " << d_end_MPI - d_begin_MPI;
	//if (mynode == 0) cout<<"Total Time is "<< d_end_MPI - d_begin_MPI <<endl;
	//if (mynode == 0) printf(" YYC Total Runtime =%f senconds\n", d_end_MPI - d_begin_MPI);

	MPI_Finalize();
	TestOut.close();
	TestOut_Slave1.close();
	TestOut_Slave2.close();
	TestOut_Slave3.close();
	return 0; 
}																																												 

