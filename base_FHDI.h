namespace FHDI
{

//------------------------
// Definitions of local base functions
// for FHDI
//------------------------

//-------------------------
//make a table with a given array
// like R's table function
//-------------------------
void table_cpp( std::string cn[], const int nrow, 
		std::vector<std::string> &v_table_row1, std::vector<int> &v_table_row2);
void table_cpp( double* d_source, const int nrow, 
		        std::vector<double> &v_table_row1, std::vector<int> &v_table_row2);
		
//-------------------------
//make a condensed string expression with a given array
//-------------------------
void Trans(double** z, const int nrow, const int ncol, std::string cn[]); //many rows case
void Trans1(double* z, const int n, std::string &cn); //one row case

//-------------------------
//local function for "which" of R 
//-------------------------
// return ACTUAL location having the same integer as i_target 
//-------------------------
void which(int* i_vector, const int n, const int i_target, std::vector<int> &v_location);
void which(double* d_vector, const int n, const double d_target, std::vector<int> &v_location);
void which(std::vector<std::string> s_vector, const  std::string s_target, 
           std::vector<int> &v_location);
void which(std::string s_array[], const int n, const  std::string s_target, 
           std::vector<int> &v_location);
void whichINV(int* i_vector, const int n, const int i_target, 
           std::vector<int> &v_location);
void whichINV(double* d_vector, const int n, const double d_target, 
           std::vector<int> &v_location);

//------------------------------- 
// Rprint: print out double matrix on output file 
//-------------------------------
void RPrint_Yicheng(double** d_debug, const int nrow, const int ncol, std::ofstream &Testout1);

//------------------------------- 
// Rprint: print out double matrix on output file 
//-------------------------------
void RPrint(double** d_debug, const int nrow, const int ncol, std::ofstream &Testout); 

//------------------------------- 
// Rprint: print out double matrix on R console
//-------------------------------
void RPrint(double** d_debug, const int nrow, const int ncol); 

//------------------------------- 
// Rprint: print out double vector on R console
//-------------------------------
void RPrint(double* d_debug, const int n) ;

//------------------------------- 
// Rprint: print out integer vector on R console
//-------------------------------
void RPrint(int* i_debug, const int n) ;

//------------------------------- 
// Rprint: print out vector of integer on R console
//-------------------------------
void RPrint(std::vector<int> i_debug) ;

//------------------------------- 
// Rprint: print out vector of double on R console
//-------------------------------
void RPrint(std::vector<double> d_debug) ;

//------------------------------- 
// Rprint: print out string array on R console
//-------------------------------
void RPrint(std::string s_debug[], const int n) ;

//------------------------------- 
// Rprint: print out string vector on R console
//-------------------------------
void RPrint(std::vector<std::string> v_sdebug) ;

//------------------------------- 
// Rprint: print one integer on R console
//-------------------------------
void RPrint(const int i_target) ;

//------------------------------- 
// Rprint: print one double on R console
//-------------------------------
void RPrint(const double d_target) ;

//------------------------------- 
// Rprint: print out string on R console
//-------------------------------
void RPrint(const char *vString) ;

//-------------------------------
//basic tools for vector, array
//-------------------------------
int sum_FHDI(std::vector<int> i_source);
int sum_FHDI(int* i_source, const int n_size);
int min_FHDI(std::vector<int> i_source);
int max_FHDI(std::vector<int> i_source);

double max_FHDI(double* k, const int n);
double min_FHDI(double* k, const int n);

int max_FHDI(int* k, const int n);
int min_FHDI(int* k, const int n);

//--------------------------------
//calculate absolute distance^2 between Matrix's row entities and a double 
//--------------------------------
void distance2(double** d_mat, const int nrow, const int ncol, const double d_origin, 
               double* d_distance);
			   
//-------------------
//return order of POSITIVE array in ascending magnitude 
//-------------------
void order_FHDI(int* i_original, const int n);	
void order_FHDI(double* d_original, const int n, int* i_return); 		   

//-------------------
// calculate joint probability using the given weights
//-------------------
void wpct_FHDI(std::string s_0[], const int n, const double* w, 
               std::vector<std::string> &jp_name, std::vector<double> &jp_prob);
			   
//---------------------
//  calculate covariance of matrix in a column-to-column manner
//---------------------
void cov_FHDI(double** x, const int nrow, const int ncol, double** cov);

//---------------------
//return the first matches
//---------------------
void match_FHDI(std::string cn[], const int nrow, std::string cn_large[], const int nrow_large, 
		       std::vector<int> &v_match); 
void match_FHDI(std::string cn[], const int nrow, 
                std::vector<std::string> v_cn_large, 
		        std::vector<int> &v_match);			   
void match_FHDI(std::vector<int> v_cn, std::vector<int> v_cn_large,  
		       std::vector<int> &v_match);
void match_FHDI(int* i_cn[], const int nrow, int* i_cn_large[], const int nrow_large, 
		       std::vector<int> &v_match);
void match_FHDI(double* d_cn[], const int nrow, double* d_cn_large[], const int nrow_large, 
		       std::vector<int> &v_match);			   
//------------------
//cumulative sum of array
//------------------
void cumsum_FHDI(double* d_original, const int n, double* d_return);
			   
} //end of namespace