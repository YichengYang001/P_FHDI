#include "List_FHDI_MPI.h"
//======================
//Compact LIST class declaration with the KNOWN ROW numbers
//to replace "R" list class
//
//
//Note: 1. only the given number of elements are stored in the compact storage
//      2. but the access can happen through the normal index (i,j)  like c++, starting from 0
//      3. all operation on an element outside the size of each list entity is null
//
// Last update: Oct 12, 2016
//
// by Dr. Cho, I. 
// All rights reservd
//======================

//================
//implementation of List_FHDI class
//================


void List_FHDI::initialize(int new_size_row)
{
	_size_row = new_size_row; 
	
	_n_each_row_size = NULL;
	_n_each_row_size = new int[new_size_row];
	for(int i=0; i<new_size_row; i++) _n_each_row_size[i] = 0 ; 
	
	_v_block.clear(); //return this to size 0
}

void List_FHDI::unlist(std::vector<double> & d_value)
//Description==================================
//	get all the stored non-null values from _v_block 
//  to d_value[]
//  like R's "unlist()" 
//
//IN   : int  i_row    = target row in the list
//OUT  : double d_value[n_size]  where n_size must be known before calling this fn. 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_size = _v_block.size();
		
	for(int i=0; i<n_size; i++) {d_value.push_back(_v_block[i]);} 

    return;
}

void List_FHDI::put_entire_block(std::vector<double> d_value)
//Description==================================
//	put the entire block into the storage _v_block 
//  from d_value[]
//
//IN   : double d_value[n_size]  where n_size must be known before calling this fn. 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_size = d_value.size();
		
	for(int i=0; i<n_size; i++) {_v_block.push_back(d_value[i]);} 

    return;
}

void List_FHDI::get_block(const int i_row, double* d_value)
//Description==================================
//	get stored block at the i_row of the list 
//
//IN   : int  i_row    = target row in the list
//OUT  : double d_value[n_size]  where n_size must be known before calling this fn. 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_size = _n_each_row_size[i_row]; 
	
	//accumulated size of all the previous rows in the list
	int i_sum = 0; for(int k=0; k<i_row; k++) {i_sum += _n_each_row_size[k];}
	
	for(int i=0; i<n_size; i++) {d_value[i] = _v_block[i_sum + i];} 

    return;
}

void List_FHDI::get_block(const int i_row, const int n_size_row, const int n_size_col, 
                          double** d_value)
//Description==================================
//	get stored MATRIX block at the i_row of the list 
//  that was stored by row-first rule
//
//IN   : int  i_row    = target row in the list
//OUT  : double d_value[n_size_row][n_size_col]  where n_size must be known before calling this fn. 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_size = _n_each_row_size[i_row]; 
	if(n_size != n_size_row*n_size_col) {cout<<"Error! matrix size is wrong in List_FHDI"<<endl; return;}

	//accumulated size of all the previous rows in the list
	int i_sum = 0; for(int k=0; k<i_row; k++) {i_sum += _n_each_row_size[k];}
	
	for(int i=0; i<n_size_col; i++) 
	{
		for(int j=0; j<n_size_row; j++)
			d_value[j][i] = _v_block[i_sum++];
	} 

    return;
}


void List_FHDI::put_block(const int i_row, const int n_size, double * d_value)
//Description==================================
//	put the new row into block
//  Note: 1. if current row i_row was not stored before, just append it by using push_back
//        2. if this row has been stored before, replacement takes place at the row
//
//IN  : int i_row  = target row number of the list (from 0 like c++ index)
//IN  : int n_size = ACTUAL size of the current row 
//IN  : double d_value[i_size] 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_existing_size = _n_each_row_size[i_row]; 
	//----------------
	//first time input
	//----------------
	if(n_existing_size == 0)
	{
		//----------------
		//store the new data into the _v_block 
		//----------------
		for(int i=0; i<n_size; i++) _v_block.push_back(d_value[i]);
		
		//-----------------
		//update the size of current row of list
		//-----------------
		_n_each_row_size[i_row] = n_size; 
			
	}
	//---------------
	//replace existing stored data
	//---------------
	if(n_existing_size > 0)
	{
		//accumulated size of all the previous rows in the list
		int i_sum = 0; for(int k=0; k<i_row; k++) {i_sum += _n_each_row_size[k];}
		
		for(int i=0; i<n_size; i++) {_v_block[i_sum + i] = d_value[i];} 
	}

    return;
}


void List_FHDI::put_block(const int i_row, const int n_size_row, const int n_size_col, 
                          double ** d_value)
//Description==================================
//	put the new row matrix into block
//  by using row-first rule 
//
//  Note: 1. if current row i_row was not stored before, just append it by using push_back
//        2. if this row has been stored before, replacement takes place at the row
//
//IN  : int i_row  = target row number of the list (from 0 like c++ index)
//IN  : double d_value[n_size_row, n_size_col] 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	int n_existing_size = _n_each_row_size[i_row]; 
	
	const int n_size = n_size_row*n_size_col; //total values of matrix 
	
	//----------------
	//first time input
	//----------------
	if(n_existing_size == 0)
	{
		for(int j=0; j<n_size_col; j++) 
		{
			for(int k=0; k<n_size_row; k++)
				_v_block.push_back(d_value[k][j]); 
		}
		
		//-----------------
		//update the size of current row of list
		//-----------------
		_n_each_row_size[i_row] = n_size; 
			
	}
	//---------------
	//replace existing stored data
	//---------------
	if(n_existing_size > 0)
	{
		//accumulated size of all the previous rows in the list
		int i_sum = 0; for(int k=0; k<i_row; k++) {i_sum += _n_each_row_size[k];}
		
		for(int j=0; j<n_size_col; j++) 
		{
			for(int k=0; k<n_size_row; k++)
				_v_block[i_sum++] = d_value[k][j];
		} 
	}

    return;
}

void List_FHDI::put_block(const int i_row, std::vector<double> v_value)
//Description==================================
//	put the new row store in double vector into block
//  Note: 1. if current row i_row was not stored before, just append it by using push_back
//        2. if this row has been stored before, replacement takes place at the row
//
//IN  : int i_row  = target row number of the list (from 0 like c++ index)
//IN  : std::vector<double> d_value[i_size] 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	const int n_size = (int)v_value.size(); 
	
	int n_existing_size = _n_each_row_size[i_row]; 
	//----------------
	//first time input
	//----------------
	if(n_existing_size == 0)
	{
		//----------------
		//store the new data into the _v_block 
		//----------------
		for(int i=0; i<n_size; i++) _v_block.push_back(v_value[i]);
		
		//-----------------
		//update the size of current row of list
		//-----------------
		_n_each_row_size[i_row] = n_size; 
			
	}
	//---------------
	//replace existing stored data
	//---------------
	if(n_existing_size > 0)
	{
		//accumulated size of all the previous rows in the list
		int i_sum = 0; for(int k=0; k<i_row; k++) {i_sum += _n_each_row_size[k];}
		
		for(int i=0; i<n_size; i++) {_v_block[i_sum + i] = v_value[i];} 
	}

    return;
}


void List_FHDI::get_a_row_size(const int i_row, int & i_value)
//Description==================================
//	get stored _n_each_row_size of a row at i_row 
//
//IN   : int i_row 	= the row number of the list
//OUT  : int i_value 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	i_value = _n_each_row_size[i_row]; 

    return;
}


void List_FHDI::put_a_row_size(const int i_row, int i_value)
//Description==================================
//	put the new size of the list into _n_each_row_size
//  
//
//IN  : int i_row   = the row number of the list
//IN  : int i_value 
//
//Note: all variable preceded by '_' are private of Grid
//=============================================
{
	_n_each_row_size[i_row] = i_value; 

    return;
}

//=====================
//print out List_FHDI
//=====================
void List_FHDI::print_List_FHDI()
{
	int n_row = (*this).size_row(); 
	for(int i=0; i<n_row; i++)
	{
		int i_temp = 0; (*this).get_a_row_size(i, i_temp);
		if(i_temp>0) //only for meaningful row 
		{
			double* d_temp = new double[i_temp];
			(*this).get_block(i, d_temp);
			RPrint(i);
			RPrint_Yicheng(d_temp, i_temp);
			delete[] d_temp; 
		}
	}
	return; 
}

//=====================
//print out ONE Row of List_FHDI
//=====================
void List_FHDI::print_one_List_FHDI(const int i_row)
{
	int n_row = (*this).size_row(); 
	
	if(i_row < n_row) 
	{
		int i = i_row; //target row number
		
		int i_temp = 0; (*this).get_a_row_size(i, i_temp);
		if(i_temp>0) //only for meaningful row 
		{
			double* d_temp = new double[i_temp];
			(*this).get_block(i, d_temp);
			//RPrint(i);
			RPrint(d_temp, i_temp);
			delete[] d_temp; 
		}
	}
	return; 
}
//=====================
//Constructor
//=====================
List_FHDI::List_FHDI(int size_row)
: _size_row(size_row), _n_each_row_size(new int[size_row])
{
	//=======
	//initialize with 0
	//=======
	//cout<<"constructor is called"<<endl;
	for(int i=0; i<size_row; i++) _n_each_row_size[i] = 0 ; 

}

//=====================
//Destructor
//=====================
List_FHDI::~List_FHDI()
{

	//cout << "destructor is called" << endl;

	delete[] _n_each_row_size ; 

	_v_block.clear();// clear the contents, but the capacity is still the same

	std::vector<double>().swap(_v_block);// swap with an empty vector to resize the capacity of vector as 0
}
